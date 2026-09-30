#include "file_dialog.h"

#include "../editor_context.h"
#include "../textures/texgen.h"
#include "../../utils/log.h"

#include <imgui.h>

#include <Windows.h>

#include <algorithm>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <filesystem>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

namespace fs = std::filesystem;

namespace editor::ui::filedlg
{
    namespace
    {
        constexpr const char* POPUP_ID = "bfve_file_dialog";

        constexpr int REQUESTS_PER_FRAME = 8;
        constexpr int UPLOADS_PER_FRAME = 8;
        constexpr int THUMB_DECODE_SIZE = 128;
        constexpr size_t MAX_PREVIEWS = 1024; // ~64 MB of 128px thumbs

        struct Entry
        {
            std::string name;
            std::string path;
            bool isDir = false;
            uintmax_t size = 0;
            long long mtime = 0;
        };

        struct Preview
        {
            void* view = nullptr; // ID3D11ShaderResourceView*
            int width = 0;
            int height = 0;
            bool failed = false;
            bool pending = false;
            uintmax_t size = 0; // file size at decode
            long long mtime = 0;
        };

        std::string g_title;
        Mode g_mode = Mode::Open;
        std::vector<std::string> g_extensions;
        bool g_open = false;
        bool g_needsOpen = false;
        bool g_showAll = false;
        bool g_thumbs = true;
        float g_thumbSize = 96.0f;
        int g_selected = -1;
        bool g_confirmOverwrite = false;

        std::string g_dir;
        char g_dirBuf[1024] = {};
        char g_nameBuf[512] = {};
        char g_filter[128] = {};
        std::string g_listError;

        int g_drawnFrame = -1;

        std::vector<Entry> g_entries;
        std::unordered_map<std::string, Preview> g_previews;

        struct Decoded
        {
            std::string path;
            textures::gen::FileImage image;
            bool ok = false;
        };
        std::mutex g_workMutex;
        std::condition_variable g_workCv;
        std::deque<std::string> g_jobs;
        std::deque<Decoded> g_done;
        std::thread g_worker;
        bool g_quit = false;

        void workerMain()
        {
            for (;;)
            {
                std::string path;
                {
                    std::unique_lock<std::mutex> lock(g_workMutex);
                    g_workCv.wait(lock, [] { return g_quit || !g_jobs.empty(); });
                    if (g_quit)
                        return;
                    path = std::move(g_jobs.front());
                    g_jobs.pop_front();
                }
                Decoded d;
                d.path = std::move(path);
                std::string err;
                d.ok = textures::gen::loadFileImage(d.path, THUMB_DECODE_SIZE, d.image, err);
                std::lock_guard<std::mutex> lock(g_workMutex);
                g_done.push_back(std::move(d));
            }
        }

        void ensureWorker()
        {
            if (!g_worker.joinable())
                g_worker = std::thread(workerMain);
        }

        void stopWorker()
        {
            {
                std::lock_guard<std::mutex> lock(g_workMutex);
                g_quit = true;
            }
            g_workCv.notify_all();
            if (g_worker.joinable())
                g_worker.join();
            std::lock_guard<std::mutex> lock(g_workMutex);
            g_quit = false;
            g_jobs.clear();
            g_done.clear();
        }

        void dropQueuedJobs()
        {
            std::lock_guard<std::mutex> lock(g_workMutex);
            for (const std::string& p : g_jobs)
                g_previews.erase(p);
            g_jobs.clear();
        }

        // render thread
        void uploadDecoded()
        {
            for (int n = 0; n < UPLOADS_PER_FRAME; ++n)
            {
                Decoded d;
                {
                    std::lock_guard<std::mutex> lock(g_workMutex);
                    if (g_done.empty())
                        return;
                    d = std::move(g_done.front());
                    g_done.pop_front();
                }
                auto it = g_previews.find(d.path);
                if (it == g_previews.end() || !it->second.pending)
                    continue; // evicted meanwhile
                Preview& p = it->second;
                p.pending = false;
                std::string err;
                p.view = d.ok ? textures::gen::createFileView(d.image, err) : nullptr;
                p.width = d.image.fullW;
                p.height = d.image.fullH;
                p.failed = (p.view == nullptr);
            }
        }

        std::string lower(std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            return s;
        }

        std::string extensionOf(const std::string& name)
        {
            const size_t dot = name.find_last_of('.');
            return dot == std::string::npos ? std::string{} : lower(name.substr(dot + 1));
        }

        bool isImage(const std::string& ext)
        {
            static const char* kImage[] = { "png", "dds", "jpg", "jpeg", "bmp", "tga", "gif",
                                            "tif", "tiff" };
            for (const char* e : kImage)
                if (ext == e)
                    return true;
            return false;
        }

        bool passesFilter(const Entry& e)
        {
            if (e.isDir)
                return true;

            if (g_filter[0])
            {
                if (lower(e.name).find(lower(g_filter)) == std::string::npos)
                    return false;
            }

            if (g_showAll || g_extensions.empty())
                return true;

            const std::string ext = extensionOf(e.name);
            for (const std::string& want : g_extensions)
                if (ext == want)
                    return true;

            return false;
        }

        void releaseAllPreviews()
        {
            stopWorker();
            for (auto& [path, p] : g_previews)
                textures::gen::releaseFileView(p.view);
            g_previews.clear();
        }

        void evictPreviews()
        {
            if (g_previews.size() < MAX_PREVIEWS)
                return;
            std::unordered_set<std::string> keep;
            for (const Entry& e : g_entries)
                keep.insert(e.path);
            for (auto it = g_previews.begin(); it != g_previews.end();)
            {
                if (it->second.pending || keep.count(it->first))
                {
                    ++it;
                    continue;
                }
                textures::gen::releaseFileView(it->second.view);
                it = g_previews.erase(it);
            }
        }

        void listDirectory(const std::string& dir)
        {
            g_entries.clear();
            g_listError.clear();
            g_selected = -1;

            dropQueuedJobs();

            std::error_code ec;
            const fs::path root(dir);

            if (!fs::exists(root, ec) || !fs::is_directory(root, ec))
            {
                g_listError = "no such folder";
                return;
            }

            fs::directory_iterator it(root, fs::directory_options::skip_permission_denied, ec);
            if (ec)
            {
                g_listError = ec.message();
                return;
            }

            for (const fs::directory_entry& de : it)
            {
                std::error_code e2;
                Entry entry;
                entry.isDir = de.is_directory(e2);
                entry.name = de.path().filename().string();
                entry.path = de.path().string();

                if (!entry.isDir)
                {
                    entry.size = de.file_size(e2);
                    if (e2)
                        entry.size = 0;

                    const auto t = de.last_write_time(e2);
                    entry.mtime = e2 ? 0 : t.time_since_epoch().count();
                }

                g_entries.push_back(std::move(entry));
            }

            std::sort(g_entries.begin(), g_entries.end(),
                [](const Entry& a, const Entry& b)
                {
                    if (a.isDir != b.isDir)
                        return a.isDir; // folders first
                    return _stricmp(a.name.c_str(), b.name.c_str()) < 0;
                });
            evictPreviews();
        }

        void navigate(const std::string& dir)
        {
            std::error_code ec;
            const fs::path p = fs::absolute(fs::path(dir), ec);
            g_dir = ec ? dir : p.lexically_normal().string();

            std::snprintf(g_dirBuf, sizeof(g_dirBuf), "%s", g_dir.c_str());
            listDirectory(g_dir);
        }

        const Preview* previewFor(const Entry& e, bool request, int& budget)
        {
            if (e.isDir || !isImage(extensionOf(e.name)))
                return nullptr;

            if (auto it = g_previews.find(e.path); it != g_previews.end())
            {
                Preview& p = it->second;
                if (p.pending)
                    return nullptr;
                if (p.size == e.size && p.mtime == e.mtime)
                    return p.failed ? nullptr : &p;
                textures::gen::releaseFileView(p.view);
                g_previews.erase(it);
            }

            if (!request || budget <= 0 || g_previews.size() >= MAX_PREVIEWS)
                return nullptr;
            --budget;

            Preview p;
            p.pending = true;
            p.size = e.size;
            p.mtime = e.mtime;
            g_previews.emplace(e.path, p);
            ensureWorker();
            {
                std::lock_guard<std::mutex> lock(g_workMutex);
                g_jobs.push_back(e.path);
            }
            g_workCv.notify_one();
            return nullptr;
        }

        std::string sizeLabel(uintmax_t bytes)
        {
            char buf[64];
            if (bytes >= 1024ull * 1024ull)
                std::snprintf(buf, sizeof(buf), "%.1f MB", double(bytes) / (1024.0 * 1024.0));
            else if (bytes >= 1024ull)
                std::snprintf(buf, sizeof(buf), "%.0f KB", double(bytes) / 1024.0);
            else
                std::snprintf(buf, sizeof(buf), "%llu B", static_cast<unsigned long long>(bytes));
            return buf;
        }

        void renderPlaces()
        {
            struct Place { const char* label; std::string path; };

            std::vector<Place> places;
            places.push_back({ "Dumps", getDumpsDir() });
            places.push_back({ "Editor", getEditorRoot() });
            places.push_back({ "Documents", getConfigDir() });

            for (const Place& pl : places)
            {
                if (ImGui::Selectable(pl.label, g_dir == pl.path))
                    navigate(pl.path);
            }

            ImGui::Separator();

            const DWORD mask = GetLogicalDrives();
            for (int i = 0; i < 26; ++i)
            {
                if (!(mask & (1u << i)))
                    continue;

                char label[8];
                std::snprintf(label, sizeof(label), "%c:", 'A' + i);

                char path[8];
                std::snprintf(path, sizeof(path), "%c:\\", 'A' + i);

                if (ImGui::Selectable(label, g_dir == path))
                    navigate(path);
            }
        }

        bool commit(std::string& outPath)
        {
            if (g_nameBuf[0] == 0)
                return false;

            fs::path chosen(g_nameBuf);
            if (!chosen.is_absolute())
                chosen = fs::path(g_dir) / chosen;

            std::error_code ec;

            if (g_mode == Mode::Open)
            {
                if (!fs::exists(chosen, ec))
                {
                    g_listError = "no such file";
                    return false;
                }
            }
            else if (fs::exists(chosen, ec) && !g_confirmOverwrite)
            {
                g_confirmOverwrite = true;
                return false;
            }

            outPath = chosen.lexically_normal().string();
            g_open = false;
            ImGui::CloseCurrentPopup();
            return true;
        }
    }

    void open(const char* title, Mode mode, const std::string& startDir,
              const std::string& suggestedName, std::vector<std::string> extensions)
    {
        g_title = title ? title : "Choose a file";
        g_mode = mode;
        g_extensions = std::move(extensions);
        for (std::string& e : g_extensions)
            e = lower(e);

        g_open = true;
        g_needsOpen = true;
        g_showAll = false;
        g_filter[0] = 0;
        g_confirmOverwrite = false;

        std::snprintf(g_nameBuf, sizeof(g_nameBuf), "%s", suggestedName.c_str());

        // a folder in suggestedName sets the start dir
        std::string dir = startDir;
        if (!suggestedName.empty())
        {
            const fs::path sn(suggestedName);
            if (sn.has_parent_path())
            {
                dir = sn.parent_path().string();
                std::snprintf(g_nameBuf, sizeof(g_nameBuf), "%s",
                    sn.filename().string().c_str());
            }
        }

        if (dir.empty())
            dir = getDumpsDir();

        std::error_code ec;
        fs::create_directories(fs::path(dir), ec);
        navigate(dir);
    }

    bool isOpen() { return g_open; }

    void releasePreviews() { releaseAllPreviews(); }

    bool draw(std::string& outPath)
    {
        if (!g_open)
            return false;

        const int frame = ImGui::GetFrameCount();
        if (g_drawnFrame == frame)
            return false;
        g_drawnFrame = frame;

        if (g_needsOpen)
        {
            g_needsOpen = false;
            ImGui::OpenPopup(POPUP_ID);
            ImGui::SetNextWindowSize(ImVec2(880.0f, 560.0f), ImGuiCond_Appearing);
        }

        bool picked = false;

        if (!ImGui::BeginPopupModal(POPUP_ID, nullptr, ImGuiWindowFlags_NoSavedSettings))
        {
            // closed by Escape
            if (g_open && !ImGui::IsPopupOpen(POPUP_ID))
                g_open = false;
            return false;
        }

        uploadDecoded();

        ImGui::TextUnformatted(g_title.c_str());
        ImGui::Separator();

        if (ImGui::Button("Up"))
        {
            const fs::path p(g_dir);
            if (p.has_parent_path() && p.parent_path() != p)
                navigate(p.parent_path().string());
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh"))
            listDirectory(g_dir);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);
        if (ImGui::InputText("##dir", g_dirBuf, sizeof(g_dirBuf),
                             ImGuiInputTextFlags_EnterReturnsTrue))
            navigate(g_dirBuf);

        ImGui::Checkbox("thumbnails", &g_thumbs);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        ImGui::SliderFloat("size", &g_thumbSize, 48.0f, 192.0f, "%.0f");
        ImGui::SameLine();
        if (!g_extensions.empty())
        {
            std::string label = "all files (";
            for (size_t i = 0; i < g_extensions.size(); ++i)
                label += (i ? ", ." : ".") + g_extensions[i];
            label += " only)";
            ImGui::Checkbox(label.c_str(), &g_showAll);
            ImGui::SameLine();
        }
        ImGui::SetNextItemWidth(180.0f);
        ImGui::InputTextWithHint("##filter", "filter", g_filter, sizeof(g_filter));

        ImGui::Separator();

        const float footer = ImGui::GetFrameHeightWithSpacing() * 2.0f + 8.0f;

        ImGui::BeginChild("places", ImVec2(150.0f, -footer), true);
        renderPlaces();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("files", ImVec2(0.0f, -footer), true);

        if (!g_listError.empty())
            ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.0f), "%s", g_listError.c_str());

        std::string pendingNav;

        int budget = REQUESTS_PER_FRAME;

        std::vector<int> shown;
        shown.reserve(g_entries.size());
        for (int i = 0; i < int(g_entries.size()); ++i)
            if (passesFilter(g_entries[i]))
                shown.push_back(i);

        if (g_thumbs)
        {
            const float cell = g_thumbSize + 16.0f;
            const float avail = ImGui::GetContentRegionAvail().x;
            const int perRow = (std::max)(1, int(avail / cell));

            for (size_t n = 0; n < shown.size(); ++n)
            {
                const int i = shown[n];
                const Entry& e = g_entries[i];

                if (n % size_t(perRow) != 0)
                    ImGui::SameLine();

                ImGui::PushID(i);
                ImGui::BeginGroup();

                const bool visible = ImGui::IsRectVisible(ImVec2(g_thumbSize + 8.0f, g_thumbSize + 8.0f));
                const Preview* pv = previewFor(e, visible, budget);
                const bool isSel = (g_selected == i);

                if (isSel)
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.85f));

                bool clicked;
                if (pv && pv->view)
                {
                    clicked = ImGui::ImageButton("t", reinterpret_cast<ImTextureID>(pv->view),
                                                 ImVec2(g_thumbSize, g_thumbSize));
                }
                else
                {
                    clicked = ImGui::Button(e.isDir ? "[dir]" : "...",
                                            ImVec2(g_thumbSize + 8.0f, g_thumbSize + 8.0f));
                }

                if (isSel)
                    ImGui::PopStyleColor();

                if (clicked)
                {
                    g_selected = i;
                    g_confirmOverwrite = false;
                    if (!e.isDir)
                        std::snprintf(g_nameBuf, sizeof(g_nameBuf), "%s", e.name.c_str());
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    if (e.isDir)
                        pendingNav = e.path;
                    else if (commit(outPath))
                        picked = true;

                    ImGui::EndGroup();
                    ImGui::PopID();
                    break;
                }

                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + g_thumbSize);
                ImGui::TextDisabled("%s", e.name.c_str());
                ImGui::PopTextWrapPos();

                ImGui::EndGroup();

                if (ImGui::IsItemHovered() && !e.isDir)
                {
                    if (pv && pv->width)
                        ImGui::SetTooltip("%s\n%dx%d  %s", e.name.c_str(), pv->width, pv->height,
                            sizeLabel(e.size).c_str());
                    else
                        ImGui::SetTooltip("%s\n%s", e.name.c_str(), sizeLabel(e.size).c_str());
                }

                ImGui::PopID();

                if (picked)
                    break;
            }
        }
        else
        {
            for (int i : shown)
            {
                const Entry& e = g_entries[i];

                char label[600];
                std::snprintf(label, sizeof(label), "%s%s##f%d",
                    e.isDir ? "[ " : "", e.name.c_str(), i);

                if (ImGui::Selectable(label, g_selected == i,
                                      ImGuiSelectableFlags_AllowDoubleClick))
                {
                    g_selected = i;
                    g_confirmOverwrite = false;
                    if (!e.isDir)
                        std::snprintf(g_nameBuf, sizeof(g_nameBuf), "%s", e.name.c_str());

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        if (e.isDir)
                        {
                            pendingNav = e.path;
                            break;
                        }
                        if (commit(outPath))
                        {
                            picked = true;
                            break;
                        }
                    }
                }

                if (!e.isDir)
                {
                    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 80.0f);
                    ImGui::TextDisabled("%s", sizeLabel(e.size).c_str());
                }
            }
        }

        ImGui::EndChild();

        if (!pendingNav.empty())
            navigate(pendingNav);

        if (!picked)
        {
            ImGui::SetNextItemWidth(-260.0f);
            if (ImGui::InputText("##name", g_nameBuf, sizeof(g_nameBuf),
                                 ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (commit(outPath))
                    picked = true;
            }
            if (ImGui::IsItemEdited())
                g_confirmOverwrite = false;

            ImGui::SameLine();
            const char* action = g_mode == Mode::Save
                ? (g_confirmOverwrite ? "Overwrite" : "Save")
                : "Open";

            if (ImGui::Button(action, ImVec2(110.0f, 0.0f)))
            {
                if (commit(outPath))
                    picked = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(110.0f, 0.0f)))
            {
                g_open = false;
                ImGui::CloseCurrentPopup();
            }

            if (g_confirmOverwrite)
            {
                ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.3f, 1.0f),
                    "that file exists - press Overwrite to replace it");
            }
            else if (!g_listError.empty() && g_entries.empty())
            {
                ImGui::TextDisabled("%s", g_listError.c_str());
            }
            else
            {
                ImGui::TextDisabled("%zu item(s)%s", shown.size(),
                    g_previews.size() >= MAX_PREVIEWS ? "  (preview cache full)" : "");
            }
        }

        ImGui::EndPopup();
        return picked;
    }
}
