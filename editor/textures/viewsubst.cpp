#include "viewsubst.h"

#include <Windows.h>
#include <d3d11.h>
#include <MinHook.h>
#include <atomic>
#include <cstring>
#include <deque>
#include <mutex>

namespace editor::textures::subst
{
    namespace
    {
        using SetViews_t = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const*);

        constexpr size_t MAX_PAIRS = 8;
        struct Table
        {
            size_t count = 0;
            Pair pairs[MAX_PAIRS] = {};
        };

        // tables are never freed: a bind on another thread may still read the previous one
        std::mutex g_mutex;
        std::deque<Table> g_tables;
        std::atomic<const Table*> g_table{ nullptr };
        bool g_init = false;

        SetViews_t oPS = nullptr, oVS = nullptr, oPSD = nullptr, oVSD = nullptr;

        template <SetViews_t* O>
        void STDMETHODCALLTYPE hkSetViews(ID3D11DeviceContext* ctx, UINT start, UINT n, ID3D11ShaderResourceView* const* views)
        {
            const Table* t = g_table.load(std::memory_order_acquire);
            if (t && views && n <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
            {
                ID3D11ShaderResourceView* local[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
                bool copied = false;
                for (UINT i = 0; i < n; ++i)
                    for (size_t p = 0; p < t->count; ++p)
                        if (views[i] == t->pairs[p].from)
                        {
                            if (!copied) { std::memcpy(local, views, n * sizeof(*views)); copied = true; }
                            local[i] = t->pairs[p].to;
                            break;
                        }
                if (copied) return (*O)(ctx, start, n, local);
            }
            (*O)(ctx, start, n, views);
        }

        // ID3D11DeviceContext: 8 PSSetShaderResources, 25 VSSetShaderResources
        void hook(void** vt, SetViews_t* ps, SetViews_t* vs, void* hps, void* hvs)
        {
            MH_CreateHook(vt[8], hps, reinterpret_cast<LPVOID*>(ps));
            MH_CreateHook(vt[25], hvs, reinterpret_cast<LPVOID*>(vs));
            MH_EnableHook(vt[8]);
            MH_EnableHook(vt[25]);
        }
    }

    void init(ID3D11Device* device, ID3D11DeviceContext* context)
    {
        if (!device || !context || g_init)
            return;
        g_init = true;
        void** vt = *reinterpret_cast<void***>(context);
        hook(vt, &oPS, &oVS, reinterpret_cast<void*>(&hkSetViews<&oPS>), reinterpret_cast<void*>(&hkSetViews<&oVS>));

        ID3D11DeviceContext* deferred = nullptr;
        if (SUCCEEDED(device->CreateDeferredContext(0, &deferred)) && deferred)
        {
            void** dvt = *reinterpret_cast<void***>(deferred);
            if (dvt[8] != vt[8])
                hook(dvt, &oPSD, &oVSD, reinterpret_cast<void*>(&hkSetViews<&oPSD>), reinterpret_cast<void*>(&hkSetViews<&oVSD>));
            deferred->Release();
        }
    }

    void set(const Pair* pairs, size_t count)
    {
        if (!count || !pairs)
        {
            g_table.store(nullptr, std::memory_order_release);
            return;
        }
        std::lock_guard<std::mutex> lock(g_mutex);
        Table& t = g_tables.emplace_back();
        t.count = count < MAX_PAIRS ? count : MAX_PAIRS;
        for (size_t i = 0; i < t.count; ++i)
            t.pairs[i] = pairs[i];
        g_table.store(&t, std::memory_order_release);
    }
}
