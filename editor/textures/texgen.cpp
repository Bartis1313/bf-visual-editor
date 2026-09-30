#include "texgen.h"
#include "../ui/file_dialog.h"

#include "../../SDK/fb.h"

#include "../../hooks/functions.h"
#include "../../utils/log.h"
#include "../editor_context.h"

#include <filesystem>
#include <fstream>

#include <Windows.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <wincodec.h>
#include <magic_enum/magic_enum.hpp>

#include "tint_vs.h"
#include "tint_ps.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstring>
#include <future>
#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <new>
#include <vector>

#pragma comment(lib, "windowscodecs.lib")

namespace editor::textures::gen
{
    namespace
    {
#if defined(BFVE_GAME_BF4)
        static_assert(sizeof(fb::DxTexture) == 0xC0, "fb::DxTexture layout");
#else
        static_assert(sizeof(fb::DxTexture) == 0x7C, "fb::DxTexture layout");
#endif

        struct Override
        {
            void* originalSrv0 = nullptr;
            void* originalSrv1 = nullptr;
            ID3D11Texture2D* tex = nullptr;
            ID3D11ShaderResourceView* srvLinear = nullptr;
            ID3D11ShaderResourceView* srvSrgb = nullptr;
            ID3D11ShaderResourceView* baseSrv = nullptr;
            Params params;

            bool installed = false;

            uint32_t width = 0, height = 0, mips = 0, shaderFormat = 0;
            bool identified = false;
            uint16_t handle = 0;
            bool hasHandle = false;

            std::string sourceFile;
            bool tinted = false;
            std::string assetPath;
        };

        PathOfTexture g_pathOf = nullptr;
        TextureOfPath g_textureOf = nullptr;
        std::vector<std::pair<std::string, EditInfo>> g_pendingReplays;

        std::mutex g_overrideMutex;
        std::unordered_map<void*, Override> g_overrides;
        std::vector<std::pair<void*, void*>> g_clones; // { copy, source }

        fb::DxTexture* asTex(void* p) { return static_cast<fb::DxTexture*>(p); }

        bool isClonedLocked(void* p)
        {
            for (const auto& [copy, src] : g_clones)
                if (copy == p)
                    return true;
            return false;
        }

        struct UiState
        {
            Params p;
            std::string lastError;
            std::string lastInfo;
            std::string lastSeed;
            char imagePath[512] = {};
            int fileFormat = 0; // 0 = DDS, 1 = PNG
            bool seeded = false;
        };
        static std::unordered_map<void*, UiState> g_ui;

        void tintSize(const fb::DxTexture* tex, int& w, int& h)
        {
            w = h = 512;
            if (tex->m_width && tex->m_height)
            {
                w = int((std::min)((std::max)(tex->m_width, 4u), 4096u));
                h = int((std::min)((std::max)(tex->m_height, 4u), 4096u));
            }
        }

        bool isSrgbFormat(DXGI_FORMAT f)
        {
            switch (f)
            {
            case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
            case DXGI_FORMAT_BC1_UNORM_SRGB:
            case DXGI_FORMAT_BC2_UNORM_SRGB:
            case DXGI_FORMAT_BC3_UNORM_SRGB:
            case DXGI_FORMAT_BC7_UNORM_SRGB:
                return true;
            default:
                return false;
            }
        }

        bool srvDesc(void* srv, D3D11_SHADER_RESOURCE_VIEW_DESC& out)
        {
            if (!srv)
                return false;
            static_cast<ID3D11ShaderResourceView*>(srv)->GetDesc(&out);
            return true;
        }

        bool viewIsSrgb(void* srv)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC d{};
            return srvDesc(srv, d) && isSrgbFormat(d.Format);
        }

        void slotEncodings(fb::DxTexture* tex, const Override* o, bool& srgb0, bool& srgb1)
        {
            void* s0 = (o && o->srvLinear) ? static_cast<void*>(o->srvLinear) : tex->m_shaderViews[0];
            void* s1 = (o && o->srvSrgb) ? static_cast<void*>(o->srvSrgb) : tex->m_shaderViews[1];
            srgb0 = viewIsSrgb(s0);
            srgb1 = viewIsSrgb(s1);
        }

        bool loadImageFile(const std::string& path, int& outW, int& outH,
                           std::vector<uint32_t>& rgba, std::string& err, int maxSide = 0)
        {
            const int wlen = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
            if (wlen <= 0) { err = "bad path"; return false; }
            std::wstring wpath(size_t(wlen), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath.data(), wlen);

            IWICImagingFactory* factory = nullptr;
            HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                          IID_PPV_ARGS(&factory));
            if (FAILED(hr) || !factory)
            {
                CoInitializeEx(nullptr, COINIT_MULTITHREADED);
                hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                      IID_PPV_ARGS(&factory));
                if (FAILED(hr) || !factory) { err = "WIC unavailable"; return false; }
            }

            IWICBitmapDecoder* decoder = nullptr;
            IWICBitmapFrameDecode* frame = nullptr;
            IWICFormatConverter* conv = nullptr;
            IWICBitmapScaler* scaler = nullptr;
            bool ok = false;

            do
            {
                if (FAILED(factory->CreateDecoderFromFilename(wpath.c_str(), nullptr, GENERIC_READ,
                        WICDecodeMetadataCacheOnDemand, &decoder))) { err = "cannot open image"; break; }
                if (FAILED(decoder->GetFrame(0, &frame))) { err = "no frame"; break; }

                UINT w = 0, h = 0;
                frame->GetSize(&w, &h);
                if (!w || !h) { err = "empty image"; break; }

                UINT dw = (std::min)((std::max)(w, 4u), 4096u);
                UINT dh = (std::min)((std::max)(h, 4u), 4096u);

                if (maxSide > 0 && (dw > UINT(maxSide) || dh > UINT(maxSide)))
                {
                    const double s = double(maxSide) / double((std::max)(dw, dh));
                    dw = (std::max)(UINT(double(dw) * s), 1u);
                    dh = (std::max)(UINT(double(dh) * s), 1u);
                }

                if (FAILED(factory->CreateFormatConverter(&conv))) { err = "no converter"; break; }
                if (FAILED(conv->Initialize(frame, GUID_WICPixelFormat32bppRGBA,
                        WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom)))
                { err = "convert failed"; break; }

                if (FAILED(factory->CreateBitmapScaler(&scaler))) { err = "no scaler"; break; }
                if (FAILED(scaler->Initialize(conv, dw, dh, WICBitmapInterpolationModeFant)))
                { err = "scale failed"; break; }

                rgba.resize(size_t(dw) * size_t(dh));
                if (FAILED(scaler->CopyPixels(nullptr, dw * 4, UINT(rgba.size() * 4),
                        reinterpret_cast<BYTE*>(rgba.data()))))
                { err = "copy failed"; break; }

                outW = int(dw);
                outH = int(dh);
                ok = true;
            } while (false);

            if (scaler) scaler->Release();
            if (conv) conv->Release();
            if (frame) frame->Release();
            if (decoder) decoder->Release();
            factory->Release();
            return ok;
        }

        void buildMips(const std::vector<uint32_t>& base, int width, int height,
                       std::vector<std::vector<uint32_t>>& levels)
        {
            levels.clear();
            levels.push_back(base);

            int w = width, h = height;
            while (w > 1 || h > 1)
            {
                const std::vector<uint32_t>& src = levels.back();
                const int hw = (std::max)(w / 2, 1);
                const int hh = (std::max)(h / 2, 1);
                std::vector<uint32_t> dst(size_t(hw) * size_t(hh));

                for (int y = 0; y < hh; ++y)
                {
                    for (int x = 0; x < hw; ++x)
                    {
                        uint32_t acc[4] = { 0, 0, 0, 0 };
                        for (int dy = 0; dy < 2; ++dy)
                            for (int dx = 0; dx < 2; ++dx)
                            {
                                const int sy = (std::min)(y * 2 + dy, h - 1);
                                const int sx = (std::min)(x * 2 + dx, w - 1);
                                const uint32_t s = src[size_t(sy) * size_t(w) + size_t(sx)];
                                acc[0] += s & 0xFF;
                                acc[1] += (s >> 8) & 0xFF;
                                acc[2] += (s >> 16) & 0xFF;
                                acc[3] += (s >> 24) & 0xFF;
                            }
                        dst[size_t(y) * size_t(hw) + size_t(x)] =
                            (acc[0] / 4) | ((acc[1] / 4) << 8) | ((acc[2] / 4) << 16) | ((acc[3] / 4) << 24);
                    }
                }
                levels.push_back(std::move(dst));
                w = hw;
                h = hh;
            }
        }

        struct TintCB
        {
            float tint[4]; // rgb multiply, a scales alpha
            float params[4]; // x = saturation, y = brightness, z = sharpness, w = upscale factor
            float texel[4]; // xy = 1 / source size, zw = source size
            float enhance[4]; // x = detail, y = mode: 0 plain, 1 enhance, 2 mip
        };

        ID3D11VertexShader* g_tintVSObj = nullptr;
        ID3D11PixelShader* g_tintPSObj = nullptr;
        ID3D11SamplerState* g_tintSampler = nullptr;
        ID3D11Buffer* g_tintCB = nullptr;
        ID3D11BlendState* g_tintBlend = nullptr;
        ID3D11DepthStencilState* g_tintDepth = nullptr;
        ID3D11RasterizerState* g_tintRast = nullptr;

        bool ensureTintPipeline(std::string& err)
        {
            if (g_tintVSObj && g_tintPSObj && g_tintSampler && g_tintCB)
                return true;

            if (!g_pDevice)
            {
                err = "no D3D11 device";
                return false;
            }

            if (!g_tintVSObj &&
                FAILED(g_pDevice->CreateVertexShader(g_tintVS, sizeof(g_tintVS), nullptr, &g_tintVSObj)))
            {
                err = "CreateVertexShader failed";
                return false;
            }
            if (!g_tintPSObj &&
                FAILED(g_pDevice->CreatePixelShader(g_tintPS, sizeof(g_tintPS), nullptr, &g_tintPSObj)))
            {
                err = "CreatePixelShader failed";
                return false;
            }

            if (!g_tintSampler)
            {
                D3D11_SAMPLER_DESC sd{};
                sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                sd.MaxLOD = D3D11_FLOAT32_MAX;
                if (FAILED(g_pDevice->CreateSamplerState(&sd, &g_tintSampler)))
                {
                    err = "CreateSamplerState failed";
                    return false;
                }
            }

            if (!g_tintCB)
            {
                D3D11_BUFFER_DESC bd{};
                bd.ByteWidth = sizeof(TintCB);
                bd.Usage = D3D11_USAGE_DEFAULT;
                bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                if (FAILED(g_pDevice->CreateBuffer(&bd, nullptr, &g_tintCB)))
                {
                    err = "CreateBuffer failed";
                    return false;
                }
            }

            if (!g_tintBlend)
            {
                D3D11_BLEND_DESC bd{};
                bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
                g_pDevice->CreateBlendState(&bd, &g_tintBlend);
            }
            if (!g_tintDepth)
            {
                D3D11_DEPTH_STENCIL_DESC dd{};
                g_pDevice->CreateDepthStencilState(&dd, &g_tintDepth);
            }
            if (!g_tintRast)
            {
                D3D11_RASTERIZER_DESC rd{};
                rd.FillMode = D3D11_FILL_SOLID;
                rd.CullMode = D3D11_CULL_NONE;
                g_pDevice->CreateRasterizerState(&rd, &g_tintRast);
            }
            return true;
        }

        void sourceSize(ID3D11ShaderResourceView* srv, int& w, int& h)
        {
            ID3D11Resource* res = nullptr;
            srv->GetResource(&res);
            ID3D11Texture2D* t2d = nullptr;
            if (res && SUCCEEDED(res->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&t2d))) && t2d)
            {
                D3D11_TEXTURE2D_DESC d{};
                t2d->GetDesc(&d);
                w = int(d.Width);
                h = int(d.Height);
                t2d->Release();
            }
            if (res)
                res->Release();
        }

        void drawTintQuad(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv,
                          int width, int height, const TintCB& cb)
        {
            g_pContext->UpdateSubresource(g_tintCB, 0, nullptr, &cb, 0, 0);
            D3D11_VIEWPORT vp{};
            vp.Width = float(width);
            vp.Height = float(height);
            vp.MaxDepth = 1.0f;
            g_pContext->OMSetRenderTargets(1, &rtv, nullptr);
            g_pContext->RSSetViewports(1, &vp);
            g_pContext->PSSetShaderResources(0, 1, &srv);
            g_pContext->Draw(3, 0);
            ID3D11ShaderResourceView* nullSrv = nullptr;
            g_pContext->PSSetShaderResources(0, 1, &nullSrv);
            ID3D11RenderTargetView* nullRtv = nullptr;
            g_pContext->OMSetRenderTargets(1, &nullRtv, nullptr);
        }

        bool renderTintPass(void* sourceSrv, int width, int height, const Params& p,
                            bool srgb0, bool srgb1, Override& out, std::string& err,
                            int scale = 1, bool mips = false)
        {
            if (!ensureTintPipeline(err))
                return false;
            if (!g_pContext || !sourceSrv)
            {
                err = "no context or source view";
                return false;
            }

            const DXGI_FORMAT fmt0 = srgb0 ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;

            D3D11_TEXTURE2D_DESC td{};
            td.Width = UINT(width);
            td.Height = UINT(height);
            td.MipLevels = mips ? 0 : 1;
            td.ArraySize = 1;
            td.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_DEFAULT;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

            ID3D11Texture2D* dst = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&td, nullptr, &dst)) || !dst)
            {
                err = "CreateTexture2D failed";
                return false;
            }

            // _SRGB target encodes on write
            D3D11_RENDER_TARGET_VIEW_DESC rd{};
            rd.Format = fmt0;
            rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

            ID3D11RenderTargetView* rtv = nullptr;
            if (FAILED(g_pDevice->CreateRenderTargetView(dst, &rd, &rtv)) || !rtv)
            {
                dst->Release();
                err = "CreateRenderTargetView failed";
                return false;
            }

            auto* srv = static_cast<ID3D11ShaderResourceView*>(sourceSrv);
            int sw = width, sh = height;
            sourceSize(srv, sw, sh);

            const bool enhance = scale > 1 || p.sharpness > 0.0f || p.detail > 0.0f;
            TintCB cb{};
            cb.tint[0] = p.colorA[0]; cb.tint[1] = p.colorA[1];
            cb.tint[2] = p.colorA[2]; cb.tint[3] = p.colorA[3];
            cb.params[0] = 1.0f;
            cb.params[1] = p.brightness;
            cb.params[2] = p.sharpness;
            cb.params[3] = float(scale);
            cb.texel[0] = 1.0f / float(sw);
            cb.texel[1] = 1.0f / float(sh);
            cb.texel[2] = float(sw);
            cb.texel[3] = float(sh);
            cb.enhance[0] = p.detail;
            cb.enhance[1] = enhance ? 1.0f : 0.0f;

            ID3D11RenderTargetView* oldRtv = nullptr;
            ID3D11DepthStencilView* oldDsv = nullptr;
            g_pContext->OMGetRenderTargets(1, &oldRtv, &oldDsv);
            UINT numVp = 1;
            D3D11_VIEWPORT oldVp{};
            g_pContext->RSGetViewports(&numVp, &oldVp);

            const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            g_pContext->RSSetState(g_tintRast);
            g_pContext->OMSetBlendState(g_tintBlend, blendFactor, 0xFFFFFFFF);
            g_pContext->OMSetDepthStencilState(g_tintDepth, 0);
            g_pContext->IASetInputLayout(nullptr);
            g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            g_pContext->VSSetShader(g_tintVSObj, nullptr, 0);
            g_pContext->PSSetShader(g_tintPSObj, nullptr, 0);
            g_pContext->PSSetSamplers(0, 1, &g_tintSampler);
            g_pContext->PSSetConstantBuffers(0, 1, &g_tintCB);
            g_pContext->GSSetShader(nullptr, nullptr, 0);
            g_pContext->HSSetShader(nullptr, nullptr, 0);
            g_pContext->DSSetShader(nullptr, nullptr, 0);

            drawTintQuad(srv, rtv, width, height, cb);
            rtv->Release();

            if (mips)
            {
                D3D11_TEXTURE2D_DESC got{};
                dst->GetDesc(&got);
                TintCB mc{};
                mc.tint[0] = mc.tint[1] = mc.tint[2] = mc.tint[3] = 1.0f;
                mc.params[0] = mc.params[1] = 1.0f;
                mc.params[2] = p.sharpness;
                mc.params[3] = 1.0f;
                mc.enhance[1] = 2.0f;
                for (UINT level = 1; level < got.MipLevels; ++level)
                {
                    const int pw = (std::max)(width >> (level - 1), 1), ph = (std::max)(height >> (level - 1), 1);
                    const int lw = (std::max)(width >> level, 1), lh = (std::max)(height >> level, 1);
                    mc.texel[0] = 1.0f / float(pw);
                    mc.texel[1] = 1.0f / float(ph);
                    mc.texel[2] = float(pw);
                    mc.texel[3] = float(ph);

                    D3D11_SHADER_RESOURCE_VIEW_DESC pd{};
                    pd.Format = fmt0;
                    pd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    pd.Texture2D.MostDetailedMip = level - 1;
                    pd.Texture2D.MipLevels = 1;
                    ID3D11ShaderResourceView* prev = nullptr;
                    rd.Texture2D.MipSlice = level;
                    ID3D11RenderTargetView* lrtv = nullptr;
                    if (SUCCEEDED(g_pDevice->CreateShaderResourceView(dst, &pd, &prev)) && prev &&
                        SUCCEEDED(g_pDevice->CreateRenderTargetView(dst, &rd, &lrtv)) && lrtv)
                        drawTintQuad(prev, lrtv, lw, lh, mc);
                    if (prev) prev->Release();
                    if (lrtv) lrtv->Release();
                }
            }

            g_pContext->OMSetRenderTargets(1, &oldRtv, oldDsv);
            if (numVp)
                g_pContext->RSSetViewports(1, &oldVp);
            if (oldRtv) oldRtv->Release();
            if (oldDsv) oldDsv->Release();

            D3D11_SHADER_RESOURCE_VIEW_DESC sd{};
            sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            sd.Texture2D.MipLevels = mips ? UINT(-1) : 1;
            sd.Format = fmt0;

            ID3D11ShaderResourceView* view0 = nullptr;
            if (FAILED(g_pDevice->CreateShaderResourceView(dst, &sd, &view0)) || !view0)
            {
                dst->Release();
                err = "CreateShaderResourceView failed";
                return false;
            }

            ID3D11ShaderResourceView* view1 = nullptr;
            if (srgb1 != srgb0)
            {
                sd.Format = srgb1 ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
                g_pDevice->CreateShaderResourceView(dst, &sd, &view1);
            }
            if (!view1)
            {
                view1 = view0;
                view0->AddRef();
            }

            out.tex = dst;
            out.srvLinear = view0;
            out.srvSrgb = view1;
            return true;
        }

        void releaseBase(Override& o)
        {
            if (o.baseSrv)
            {
                o.baseSrv->Release();
                o.baseSrv = nullptr;
            }
        }

        // viewRefs 1 = ours, 2 = slot ref too
        void releaseObjects(ID3D11Texture2D* tex, ID3D11ShaderResourceView* a,
                            ID3D11ShaderResourceView* b, int viewRefs)
        {
            for (int i = 0; i < viewRefs; ++i)
            {
                if (a) a->Release();
                if (b) b->Release();
            }
            if (tex) tex->Release();
        }

        void releaseOverrideObjects(Override& o, bool alsoSlotRef)
        {
            releaseObjects(o.tex, o.srvLinear, o.srvSrgb, alsoSlotRef ? 2 : 1);
            o.srvLinear = nullptr;
            o.srvSrgb = nullptr;
            o.tex = nullptr;
        }

        bool build(const std::vector<uint32_t>& pixels, int width, int height,
                   bool srgb0, bool srgb1, Override& o, std::string& err)
        {
            if (!g_pDevice) { err = "no D3D11 device"; return false; }

            std::vector<std::vector<uint32_t>> levels;
            buildMips(pixels, width, height, levels);

            std::vector<D3D11_SUBRESOURCE_DATA> init(levels.size());
            int w = width;
            for (size_t i = 0; i < levels.size(); ++i)
            {
                init[i].pSysMem = levels[i].data();
                init[i].SysMemPitch = UINT(w) * 4;
                init[i].SysMemSlicePitch = 0;
                w = (std::max)(w / 2, 1);
            }

            D3D11_TEXTURE2D_DESC td{};
            td.Width = UINT(width);
            td.Height = UINT(height);
            td.MipLevels = UINT(levels.size());
            td.ArraySize = 1;
            td.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_IMMUTABLE;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            ID3D11Texture2D* tex = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&td, init.data(), &tex)) || !tex)
            {
                err = "CreateTexture2D failed";
                return false;
            }

            D3D11_SHADER_RESOURCE_VIEW_DESC sd{};
            sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            sd.Texture2D.MostDetailedMip = 0;
            sd.Texture2D.MipLevels = td.MipLevels;

            ID3D11ShaderResourceView* view0 = nullptr;
            sd.Format = srgb0 ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
            if (FAILED(g_pDevice->CreateShaderResourceView(tex, &sd, &view0)) || !view0)
            {
                tex->Release();
                err = "CreateShaderResourceView failed";
                return false;
            }

            ID3D11ShaderResourceView* view1 = nullptr;
            if (srgb1 != srgb0)
            {
                sd.Format = srgb1 ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
                g_pDevice->CreateShaderResourceView(tex, &sd, &view1);
            }
            if (!view1)
            {
                view1 = view0;
                view0->AddRef();
            }

            o.tex = tex;
            o.srvLinear = view0;
            o.srvSrgb = view1;
            return true;
        }

        bool savePng(const std::string& path, const std::vector<uint32_t>& px, int w, int h,
                     std::string& err)
        {
            const int wlen = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
            if (wlen <= 0) { err = "bad path"; return false; }
            std::wstring wpath(size_t(wlen), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath.data(), wlen);

            IWICImagingFactory* factory = nullptr;
            if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                    IID_PPV_ARGS(&factory))) || !factory)
            {
                CoInitializeEx(nullptr, COINIT_MULTITHREADED);
                if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                        IID_PPV_ARGS(&factory))) || !factory)
                {
                    err = "WIC unavailable";
                    return false;
                }
            }

            IWICStream* stream = nullptr;
            IWICBitmapEncoder* encoder = nullptr;
            IWICBitmapFrameEncode* frame = nullptr;
            bool ok = false;

            do
            {
                if (FAILED(factory->CreateStream(&stream))) { err = "CreateStream failed"; break; }
                if (FAILED(stream->InitializeFromFilename(wpath.c_str(), GENERIC_WRITE)))
                { err = "cannot write file"; break; }
                if (FAILED(factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder)))
                { err = "CreateEncoder failed"; break; }
                if (FAILED(encoder->Initialize(stream, WICBitmapEncoderNoCache)))
                { err = "encoder init failed"; break; }
                if (FAILED(encoder->CreateNewFrame(&frame, nullptr))) { err = "CreateNewFrame failed"; break; }
                if (FAILED(frame->Initialize(nullptr))) { err = "frame init failed"; break; }
                if (FAILED(frame->SetSize(UINT(w), UINT(h)))) { err = "SetSize failed"; break; }

                WICPixelFormatGUID fmt = GUID_WICPixelFormat32bppRGBA;
                if (FAILED(frame->SetPixelFormat(&fmt))) { err = "SetPixelFormat failed"; break; }
                if (FAILED(frame->WritePixels(UINT(h), UINT(w) * 4, UINT(px.size() * 4),
                        reinterpret_cast<BYTE*>(const_cast<uint32_t*>(px.data())))))
                { err = "WritePixels failed"; break; }
                if (FAILED(frame->Commit())) { err = "frame commit failed"; break; }
                if (FAILED(encoder->Commit())) { err = "encoder commit failed"; break; }
                ok = true;
            } while (false);

            if (frame) frame->Release();
            if (encoder) encoder->Release();
            if (stream) stream->Release();
            factory->Release();
            return ok;
        }

        bool readbackTexture(void* srv, int width, int height, bool srgb,
                             std::vector<uint32_t>& out, std::string& err)
        {
            Params identity;
            identity.sharpness = 0.0f;
            identity.detail = 0.0f;

            Override tmp;
            if (!renderTintPass(srv, width, height, identity, srgb, srgb, tmp, err))
                return false;

            D3D11_TEXTURE2D_DESC sd{};
            sd.Width = UINT(width);
            sd.Height = UINT(height);
            sd.MipLevels = 1;
            sd.ArraySize = 1;
            sd.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
            sd.SampleDesc.Count = 1;
            sd.Usage = D3D11_USAGE_STAGING;
            sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

            ID3D11Texture2D* staging = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&sd, nullptr, &staging)) || !staging)
            {
                releaseOverrideObjects(tmp, false);
                err = "staging CreateTexture2D failed";
                return false;
            }

            g_pContext->CopyResource(staging, tmp.tex);

            D3D11_MAPPED_SUBRESOURCE mapped{};
            if (FAILED(g_pContext->Map(staging, 0, D3D11_MAP_READ, 0, &mapped)))
            {
                staging->Release();
                releaseOverrideObjects(tmp, false);
                err = "Map failed";
                return false;
            }

            out.resize(size_t(width) * size_t(height));
            const uint8_t* src = static_cast<const uint8_t*>(mapped.pData);
            for (int y = 0; y < height; ++y)
                std::memcpy(&out[size_t(y) * size_t(width)], src + size_t(y) * mapped.RowPitch,
                            size_t(width) * 4);

            g_pContext->Unmap(staging, 0);
            staging->Release();
            releaseOverrideObjects(tmp, false);
            return true;
        }

        bool grabPixels(void* dxTexture, std::vector<uint32_t>& out, int& tw, int& th,
                        std::string& err)
        {
            if (!dxTexture)
            {
                err = "bad DxTexture pointer";
                return false;
            }

            fb::DxTexture* tex = asTex(dxTexture);

            void* srv = nullptr;
            {
                std::lock_guard<std::mutex> lock(g_overrideMutex);
                auto it = g_overrides.find(dxTexture);
                srv = (it != g_overrides.end() && it->second.srvLinear)
                    ? static_cast<void*>(it->second.srvLinear)
                    : static_cast<void*>(tex->m_shaderViews[0]);
            }

            if (!srv)
            {
                err = "texture has no shader view to read";
                return false;
            }

            tintSize(tex, tw, th);

            D3D11_SHADER_RESOURCE_VIEW_DESC vd{};
            const bool haveDesc = srvDesc(srv, vd);
            const bool srgb = haveDesc && isSrgbFormat(vd.Format);
            logger::debug("[texgen] readback {}x{} through a {} view (DXGI {})",
                tw, th, srgb ? "sRGB" : "linear", haveDesc ? uint32_t(vd.Format) : 0u);

            if (!readbackTexture(srv, tw, th, srgb, out, err))
                return false;

            if (pngOpaqueAlpha)
                for (uint32_t& px : out)
                    px |= 0xFF000000u;

            return true;
        }

        bool readIdentity(void* dxTexture, uint32_t& w, uint32_t& h, uint32_t& mips,
                          uint32_t& fmt)
        {
            const fb::DxTexture* tex = asTex(dxTexture);
            if (tex->m_vtable != fb::DxTexture::VTable())
                return false;

            w = tex->m_width;
            h = tex->m_height;
            mips = tex->m_mipmapCount;
            fmt = tex->m_shaderFormat;
            return true;
        }

        void* textureForHandle(uint16_t handle)
        {
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr)
                return nullptr;

            fb::TextureStreamingEntry* e = mgr->entry(handle);
            return e ? e->m_texture : nullptr;
        }

        void captureIdentity(void* dxTexture, Override& o)
        {
            o.identified = readIdentity(dxTexture, o.width, o.height, o.mips, o.shaderFormat);
            if (g_pathOf && o.assetPath.empty())
                o.assetPath = g_pathOf(dxTexture);
            if (o.assetPath.empty())
                logger::warning("[texgen] edit on {} has no asset path - it cannot follow the "
                                "texture if the engine re-creates it (scan textures first)", dxTexture);
            else
                logger::info("[texgen] edit on {} = {}", dxTexture, o.assetPath);

            const uint16_t raw = asTex(dxTexture)->m_handle;
            o.hasHandle = (raw & 0x7FFF) == raw;
            o.handle = raw;
        }

        enum class Ident { Same, Different, Unknown };

        Ident identityOf(void* dxTexture, const Override& o, const char** field = nullptr)
        {
            if (isClonedLocked(dxTexture))
                return Ident::Same;

            if (o.hasHandle)
            {
                void* const owner = textureForHandle(o.handle);
                if (!owner)
                    return Ident::Unknown;

                if (owner == dxTexture)
                    return Ident::Same;

                if (field)
                    *field = "streaming handle now belongs to another texture";
                return Ident::Different;
            }

            if (!o.identified)
                return Ident::Unknown;

            uint32_t w = 0, h = 0, mips = 0, fmt = 0;
            if (!readIdentity(dxTexture, w, h, mips, fmt))
                return Ident::Unknown;

            const char* which = nullptr;
            if (w != o.width) which = "width changed";
            else if (h != o.height) which = "height changed";
            else if (mips != o.mips) which = "mips changed";
            else if (fmt != o.shaderFormat) which = "shaderFormat changed";

            if (!which)
                return Ident::Same;

            if (field)
                *field = which;
            return Ident::Different;
        }

        bool dropIfStaleLocked(void* dxTexture)
        {
            auto it = g_overrides.find(dxTexture);
            if (it == g_overrides.end())
                return false;

            const char* field = nullptr;
            if (identityOf(dxTexture, it->second, &field) != Ident::Different)
                return false;

            const std::string path = it->second.assetPath;
            releaseBase(it->second);
            releaseOverrideObjects(it->second, it->second.installed);
            g_overrides.erase(it);

            logger::info("[texgen] dropped an edit for {} ({}): a different texture is at that "
                         "address now ({}){}", dxTexture, path.empty() ? "unnamed" : path.c_str(),
                         field ? field : "?",
                         path.empty() ? " - no path, cannot be replayed" : " - replaying on the asset's texture");
            return true;
        }

        void writeOurViews(fb::DxTexture* tex, Override& o)
        {
            o.srvLinear->AddRef();
            o.srvSrgb->AddRef();
            tex->m_shaderViews[0] = o.srvLinear;
            tex->m_shaderViews[1] = o.srvSrgb;
            o.installed = true;
        }

        void restoreEngineViews(fb::DxTexture* tex, Override& o)
        {
            tex->m_shaderViews[0] = static_cast<ID3D11ShaderResourceView*>(o.originalSrv0);
            tex->m_shaderViews[1] = static_cast<ID3D11ShaderResourceView*>(o.originalSrv1);
            o.srvLinear->Release();
            o.srvSrgb->Release();
            o.installed = false;
        }

        bool tryInstallLocked(fb::DxTexture* tex, Override& o)
        {
            if (o.installed)
                return true;
            if (!tex->m_shaderViews[0] || !tex->m_shaderViews[1])
                return false;

            o.originalSrv0 = tex->m_shaderViews[0];
            o.originalSrv1 = tex->m_shaderViews[1];
            writeOurViews(tex, o);
            return true;
        }

        Override& installOverrideLocked(void* dxTexture, Override& fresh)
        {
            fb::DxTexture* tex = asTex(dxTexture);
            auto it = g_overrides.find(dxTexture);

            if (it == g_overrides.end())
            {
                captureIdentity(dxTexture, fresh);
                fresh.installed = false;
                it = g_overrides.emplace(dxTexture, fresh).first;
                tryInstallLocked(tex, it->second);
                return it->second;
            }

            Override& o = it->second;
            const bool wasInstalled = o.installed;
            ID3D11Texture2D* oldTex = o.tex;
            ID3D11ShaderResourceView* old0 = o.srvLinear;
            ID3D11ShaderResourceView* old1 = o.srvSrgb;

            o.tex = fresh.tex;
            o.srvLinear = fresh.srvLinear;
            o.srvSrgb = fresh.srvSrgb;

            if (wasInstalled)
            {
                o.installed = false;
                writeOurViews(tex, o);
            }
            else
            {
                tryInstallLocked(tex, o);
            }

            releaseObjects(oldTex, old0, old1, wasInstalled ? 2 : 1);
            return o;
        }

        bool installOverride(void* dxTexture, Override& fresh, std::string& err)
        {
            if (!dxTexture)
            {
                releaseOverrideObjects(fresh, false);
                err = "bad DxTexture pointer";
                return false;
            }

            std::lock_guard<std::mutex> lock(g_overrideMutex);
            dropIfStaleLocked(dxTexture);

            if (!g_overrides.count(dxTexture) && !asTex(dxTexture)->m_shaderViews[0])
            {
                releaseOverrideObjects(fresh, false);
                err = "texture is not loaded";
                return false;
            }

            installOverrideLocked(dxTexture, fresh);
            return true;
        }

        bool applyPixels(void* dxTexture, const std::vector<uint32_t>& pixels,
                         int width, int height, const Params& p, std::string& err)
        {
            if (!dxTexture) { err = "bad DxTexture pointer"; return false; }

            std::lock_guard<std::mutex> lock(g_overrideMutex);
            dropIfStaleLocked(dxTexture);

            fb::DxTexture* tex = asTex(dxTexture);
            auto it = g_overrides.find(dxTexture);
            const Override* existing = it == g_overrides.end() ? nullptr : &it->second;

            if (!existing && !tex->m_shaderViews[0])
            {
                err = "texture is not loaded";
                return false;
            }

            bool srgb0 = false, srgb1 = false;
            slotEncodings(tex, existing, srgb0, srgb1);

            Override fresh;
            if (!build(pixels, width, height, srgb0, srgb1, fresh, err))
                return false;

            Override& o = installOverrideLocked(dxTexture, fresh);
            o.params = p;
            return true;
        }
#pragma pack(push, 1)
        struct DdsPixelFormat
        {
            uint32_t size, flags, fourCC, rgbBitCount, rBitMask, gBitMask, bBitMask, aBitMask;
        };
        struct DdsHeader
        {
            uint32_t size, flags, height, width, pitchOrLinearSize, depth, mipMapCount;
            uint32_t reserved1[11];
            DdsPixelFormat ddspf;
            uint32_t caps, caps2, caps3, caps4, reserved2;
        };
        struct DdsHeaderDxt10
        {
            uint32_t dxgiFormat, resourceDimension, miscFlag, arraySize, miscFlags2;
        };
#pragma pack(pop)

        constexpr uint32_t DDS_MAGIC = 0x20534444; // "DDS "
        constexpr uint32_t FOURCC_DX10 = 0x30315844; // "DX10"
        constexpr uint32_t DDSD_CAPS = 0x1;
        constexpr uint32_t DDSD_HEIGHT = 0x2;
        constexpr uint32_t DDSD_WIDTH = 0x4;
        constexpr uint32_t DDSD_PIXEL_FMT = 0x1000;
        constexpr uint32_t DDSD_MIP_COUNT = 0x20000;
        constexpr uint32_t DDSD_LINEAR = 0x80000;
        constexpr uint32_t DDPF_FOURCC = 0x4;
        constexpr uint32_t DDSCAPS_TEX = 0x1000;
        constexpr uint32_t DDSCAPS_MIP = 0x400000;
        constexpr uint32_t DDSCAPS_CMPLX = 0x8;

        bool isBlockCompressed(DXGI_FORMAT f)
        {
            switch (f)
            {
            case DXGI_FORMAT_BC1_TYPELESS: case DXGI_FORMAT_BC1_UNORM: case DXGI_FORMAT_BC1_UNORM_SRGB:
            case DXGI_FORMAT_BC2_TYPELESS: case DXGI_FORMAT_BC2_UNORM: case DXGI_FORMAT_BC2_UNORM_SRGB:
            case DXGI_FORMAT_BC3_TYPELESS: case DXGI_FORMAT_BC3_UNORM: case DXGI_FORMAT_BC3_UNORM_SRGB:
            case DXGI_FORMAT_BC4_TYPELESS: case DXGI_FORMAT_BC4_UNORM: case DXGI_FORMAT_BC4_SNORM:
            case DXGI_FORMAT_BC5_TYPELESS: case DXGI_FORMAT_BC5_UNORM: case DXGI_FORMAT_BC5_SNORM:
            case DXGI_FORMAT_BC6H_TYPELESS: case DXGI_FORMAT_BC6H_UF16: case DXGI_FORMAT_BC6H_SF16:
            case DXGI_FORMAT_BC7_TYPELESS: case DXGI_FORMAT_BC7_UNORM: case DXGI_FORMAT_BC7_UNORM_SRGB:
                return true;
            default:
                return false;
            }
        }

        uint32_t blockBytes(DXGI_FORMAT f)
        {
            switch (f)
            {
            case DXGI_FORMAT_BC1_TYPELESS: case DXGI_FORMAT_BC1_UNORM: case DXGI_FORMAT_BC1_UNORM_SRGB:
            case DXGI_FORMAT_BC4_TYPELESS: case DXGI_FORMAT_BC4_UNORM: case DXGI_FORMAT_BC4_SNORM:
                return 8;
            default:
                return 16;
            }
        }

        uint32_t bitsPerPixel(DXGI_FORMAT f)
        {
            switch (f)
            {
            case DXGI_FORMAT_R32G32B32A32_FLOAT: case DXGI_FORMAT_R32G32B32A32_UINT: return 128;
            case DXGI_FORMAT_R16G16B16A16_FLOAT: case DXGI_FORMAT_R16G16B16A16_UNORM: return 64;
            case DXGI_FORMAT_R8G8B8A8_UNORM: case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            case DXGI_FORMAT_R8G8B8A8_TYPELESS: case DXGI_FORMAT_B8G8R8A8_UNORM:
            case DXGI_FORMAT_R10G10B10A2_UNORM: case DXGI_FORMAT_R11G11B10_FLOAT: return 32;
            case DXGI_FORMAT_R8G8_UNORM: case DXGI_FORMAT_R16_FLOAT: return 16;
            case DXGI_FORMAT_R8_UNORM: case DXGI_FORMAT_A8_UNORM: return 8;
            default: return 32;
            }
        }

        void surfaceInfo(DXGI_FORMAT f, uint32_t w, uint32_t h,
                         uint32_t& rowBytes, uint32_t& rows)
        {
            if (isBlockCompressed(f))
            {
                const uint32_t bw = (std::max)(1u, (w + 3) / 4);
                const uint32_t bh = (std::max)(1u, (h + 3) / 4);
                rowBytes = bw * blockBytes(f);
                rows = bh;
            }
            else
            {
                rowBytes = (w * bitsPerPixel(f) + 7) / 8;
                rows = h;
            }
        }

        struct DdsImage
        {
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t mips = 0;
            DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
            std::vector<uint8_t> blob;
        };

        ID3D11Texture2D* asTexture2D(void* res)
        {
            ID3D11Texture2D* t = nullptr;
            const HRESULT hr = static_cast<IUnknown*>(res)->QueryInterface(
                __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&t));
            return FAILED(hr) ? nullptr : t;
        }

        DXGI_FORMAT typedFormat(DXGI_FORMAT f)
        {
            switch (f)
            {
            case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
            case DXGI_FORMAT_B8G8R8X8_TYPELESS: return DXGI_FORMAT_B8G8R8X8_UNORM;
            case DXGI_FORMAT_BC1_TYPELESS: return DXGI_FORMAT_BC1_UNORM;
            case DXGI_FORMAT_BC2_TYPELESS: return DXGI_FORMAT_BC2_UNORM;
            case DXGI_FORMAT_BC3_TYPELESS: return DXGI_FORMAT_BC3_UNORM;
            case DXGI_FORMAT_BC4_TYPELESS: return DXGI_FORMAT_BC4_UNORM;
            case DXGI_FORMAT_BC5_TYPELESS: return DXGI_FORMAT_BC5_UNORM;
            case DXGI_FORMAT_BC6H_TYPELESS: return DXGI_FORMAT_BC6H_UF16;
            case DXGI_FORMAT_BC7_TYPELESS: return DXGI_FORMAT_BC7_UNORM;
            case DXGI_FORMAT_R16G16B16A16_TYPELESS: return DXGI_FORMAT_R16G16B16A16_FLOAT;
            case DXGI_FORMAT_R32G32B32A32_TYPELESS: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case DXGI_FORMAT_R8_TYPELESS: return DXGI_FORMAT_R8_UNORM;
            case DXGI_FORMAT_R8G8_TYPELESS: return DXGI_FORMAT_R8G8_UNORM;
            case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_UNORM;
            case DXGI_FORMAT_R16G16_TYPELESS: return DXGI_FORMAT_R16G16_UNORM;
            case DXGI_FORMAT_R32_TYPELESS: return DXGI_FORMAT_R32_FLOAT;
            case DXGI_FORMAT_R10G10B10A2_TYPELESS: return DXGI_FORMAT_R10G10B10A2_UNORM;
            default: return f;
            }
        }

        bool readDdsImage(void* dxTexture, DdsImage& img, std::string& err)
        {
            if (!g_pDevice || !g_pContext)
            {
                err = "no device";
                return false;
            }

            void* res = asTex(dxTexture)->m_resource;
            if (!res)
            {
                err = "texture has no D3D resource";
                return false;
            }

            ID3D11Texture2D* src = asTexture2D(res);
            if (!src)
            {
                err = "resource is not a Texture2D";
                return false;
            }

            D3D11_TEXTURE2D_DESC td{};
            src->GetDesc(&td);

            D3D11_TEXTURE2D_DESC sd = td;
            sd.Usage = D3D11_USAGE_STAGING;
            sd.BindFlags = 0;
            sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            sd.MiscFlags = 0;

            ID3D11Texture2D* staging = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&sd, nullptr, &staging)) || !staging)
            {
                src->Release();
                err = "staging CreateTexture2D failed";
                return false;
            }

            g_pContext->CopyResource(staging, src);
            src->Release();

            std::vector<uint8_t>& blob = img.blob;
            const uint32_t mips = (std::max)(td.MipLevels, 1u);
            bool ok = true;

            for (uint32_t m = 0; m < mips && ok; ++m)
            {
                D3D11_MAPPED_SUBRESOURCE mapped{};
                if (FAILED(g_pContext->Map(staging, m, D3D11_MAP_READ, 0, &mapped)))
                {
                    err = "Map failed";
                    ok = false;
                    break;
                }

                const uint32_t mw = (std::max)(td.Width >> m, 1u);
                const uint32_t mh = (std::max)(td.Height >> m, 1u);
                uint32_t rowBytes = 0, rows = 0;
                surfaceInfo(td.Format, mw, mh, rowBytes, rows);

                const uint8_t* p = static_cast<const uint8_t*>(mapped.pData);
                for (uint32_t r = 0; r < rows; ++r)
                    blob.insert(blob.end(), p + size_t(r) * mapped.RowPitch,
                                p + size_t(r) * mapped.RowPitch + rowBytes);

                g_pContext->Unmap(staging, m);
            }
            staging->Release();
            if (!ok)
                return false;

            img.width = td.Width;
            img.height = td.Height;
            img.mips = mips;
            img.format = td.Format;
            if (typedFormat(img.format) != img.format)
            {
                const uint32_t viewFmt = asTex(dxTexture)->m_shaderFormat;
                img.format = (viewFmt && typedFormat(DXGI_FORMAT(viewFmt)) == DXGI_FORMAT(viewFmt))
                    ? DXGI_FORMAT(viewFmt) : typedFormat(img.format);
            }
            return true;
        }

        // any thread
        bool writeDdsImage(const DdsImage& img, const std::string& path, std::string& err)
        {
            uint32_t topRow = 0, topRows = 0;
            surfaceInfo(img.format, img.width, img.height, topRow, topRows);

            const uint32_t mips = (std::max)(img.mips, 1u);

            DdsHeader h{};
            h.size = sizeof(DdsHeader);
            h.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXEL_FMT | DDSD_LINEAR
                    | (mips > 1 ? DDSD_MIP_COUNT : 0u);
            h.height = img.height;
            h.width = img.width;
            h.pitchOrLinearSize = topRow * topRows;
            h.mipMapCount = mips;
            h.ddspf.size = sizeof(DdsPixelFormat);
            h.ddspf.flags = DDPF_FOURCC;
            h.ddspf.fourCC = FOURCC_DX10;
            h.caps = DDSCAPS_TEX | (mips > 1 ? (DDSCAPS_MIP | DDSCAPS_CMPLX) : 0u);

            DdsHeaderDxt10 h10{};
            h10.dxgiFormat = uint32_t(img.format);
            h10.resourceDimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
            h10.arraySize = 1;

            FILE* f = nullptr;
            if (fopen_s(&f, path.c_str(), "wb") != 0 || !f)
            {
                err = "cannot open file for writing";
                return false;
            }
            const uint32_t magic = DDS_MAGIC;
            fwrite(&magic, 4, 1, f);
            fwrite(&h, sizeof(h), 1, f);
            fwrite(&h10, sizeof(h10), 1, f);
            if (!img.blob.empty())
                fwrite(img.blob.data(), 1, img.blob.size(), f);
            fclose(f);
            return true;
        }

        bool exportDdsFile(void* dxTexture, const std::string& path, std::string& err)
        {
            DdsImage img;
            return readDdsImage(dxTexture, img, err) && writeDdsImage(img, path, err);
        }

        bool parseDdsFile(const std::string& path, std::vector<uint8_t>& data,
                          DXGI_FORMAT& fmt, uint32_t& width, uint32_t& height, uint32_t& mips,
                          std::vector<D3D11_SUBRESOURCE_DATA>& init, std::string& err)
        {
            FILE* f = nullptr;
            if (fopen_s(&f, path.c_str(), "rb") != 0 || !f)
            {
                err = "cannot open file";
                return false;
            }
            fseek(f, 0, SEEK_END);
            const long len = ftell(f);
            fseek(f, 0, SEEK_SET);
            data.assign(size_t((std::max)(len, 0L)), 0);
            if (!data.empty())
                fread(data.data(), 1, data.size(), f);
            fclose(f);

            if (data.size() < 4 + sizeof(DdsHeader))
            {
                err = "not a DDS file";
                return false;
            }
            if (*reinterpret_cast<const uint32_t*>(data.data()) != DDS_MAGIC)
            {
                err = "bad DDS magic";
                return false;
            }

            const DdsHeader* h = reinterpret_cast<const DdsHeader*>(data.data() + 4);
            size_t offset = 4 + sizeof(DdsHeader);
            fmt = DXGI_FORMAT_UNKNOWN;

            if ((h->ddspf.flags & DDPF_FOURCC) && h->ddspf.fourCC == FOURCC_DX10)
            {
                if (data.size() < offset + sizeof(DdsHeaderDxt10))
                {
                    err = "truncated DX10 header";
                    return false;
                }
                fmt = DXGI_FORMAT(reinterpret_cast<const DdsHeaderDxt10*>(data.data() + offset)->dxgiFormat);
                offset += sizeof(DdsHeaderDxt10);
            }
            else if (h->ddspf.flags & DDPF_FOURCC)
            {
                switch (h->ddspf.fourCC)
                {
                case 0x31545844: fmt = DXGI_FORMAT_BC1_UNORM; break; // DXT1
                case 0x33545844: fmt = DXGI_FORMAT_BC2_UNORM; break; // DXT3
                case 0x35545844: fmt = DXGI_FORMAT_BC3_UNORM; break; // DXT5
                default:
                    err = "unsupported legacy FourCC - re-save as DX10/BC7";
                    return false;
                }
            }
            else
            {
                fmt = DXGI_FORMAT_B8G8R8A8_UNORM; // assume BGRA8
            }

            width = h->width;
            height = h->height;
            mips = (std::max)(h->mipMapCount, 1u);
            init.assign(mips, D3D11_SUBRESOURCE_DATA{});
            size_t cursor = offset;

            for (uint32_t m = 0; m < mips; ++m)
            {
                const uint32_t mw = (std::max)(h->width >> m, 1u);
                const uint32_t mh = (std::max)(h->height >> m, 1u);
                uint32_t rowBytes = 0, rows = 0;
                surfaceInfo(fmt, mw, mh, rowBytes, rows);

                if (cursor + size_t(rowBytes) * rows > data.size())
                {
                    err = "DDS data shorter than its header claims";
                    return false;
                }
                init[m].pSysMem = data.data() + cursor;
                init[m].SysMemPitch = rowBytes;
                init[m].SysMemSlicePitch = 0;
                cursor += size_t(rowBytes) * rows;
            }

            return true;
        }

        bool importDdsFile(void* dxTexture, const std::string& path, std::string& err)
        {
            std::vector<uint8_t> data;
            DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN;
            uint32_t width = 0, height = 0, mips = 0;
            std::vector<D3D11_SUBRESOURCE_DATA> init;

            if (!parseDdsFile(path, data, fmt, width, height, mips, init, err))
                return false;
            fmt = typedFormat(fmt); // old exports are TYPELESS

            D3D11_TEXTURE2D_DESC td{};
            td.Width = width;
            td.Height = height;
            td.MipLevels = mips;
            td.ArraySize = 1;
            td.Format = fmt;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_IMMUTABLE;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            Override fresh;
            ID3D11Texture2D* tex = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&td, init.data(), &tex)) || !tex)
            {
                err = "CreateTexture2D failed (format may be unsupported)";
                return false;
            }

            D3D11_SHADER_RESOURCE_VIEW_DESC svd{};
            svd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            svd.Texture2D.MipLevels = mips;
            svd.Format = fmt;

            ID3D11ShaderResourceView* srv = nullptr;
            if (FAILED(g_pDevice->CreateShaderResourceView(tex, &svd, &srv)) || !srv)
            {
                tex->Release();
                err = "CreateShaderResourceView failed";
                return false;
            }

            fresh.tex = tex;
            fresh.srvLinear = srv;
            fresh.srvSrgb = srv;
            srv->AddRef();
            return installOverride(dxTexture, fresh, err);
        }

    }

    static std::string resolveExportPath(const std::string& path, std::string& err);
    static std::string resolveImportPath(const std::string& path);

    bool loadFileImage(const std::string& path, int maxSide, FileImage& out, std::string& err)
    {
        out = FileImage{};

        std::string ext;
        if (const size_t dot = path.find_last_of('.'); dot != std::string::npos)
            for (size_t i = dot + 1; i < path.size(); ++i)
                ext += char(std::tolower(static_cast<unsigned char>(path[i])));

        if (ext == "dds")
        {
            std::vector<uint8_t> data;
            DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN;
            uint32_t w = 0, h = 0, mips = 1;
            std::vector<D3D11_SUBRESOURCE_DATA> init;
            if (!parseDdsFile(path, data, fmt, w, h, mips, init, err))
                return false;
            fmt = typedFormat(fmt);

            uint32_t first = 0;
            if (maxSide > 0)
                while (first + 1 < mips && ((std::max)(w, h) >> (first + 1)) >= uint32_t(maxSide))
                    ++first;

            out.format = uint32_t(fmt);
            out.fullW = int(w);
            out.fullH = int(h);
            out.width = (std::max)(w >> first, 1u);
            out.height = (std::max)(h >> first, 1u);
            out.mips = mips - first;
            for (uint32_t m = first; m < mips; ++m)
            {
                uint32_t rowBytes = 0, rows = 0;
                surfaceInfo(fmt, (std::max)(w >> m, 1u), (std::max)(h >> m, 1u), rowBytes, rows);
                const uint8_t* src = static_cast<const uint8_t*>(init[m].pSysMem);
                out.offsets.push_back(uint32_t(out.data.size()));
                out.pitches.push_back(rowBytes);
                out.data.insert(out.data.end(), src, src + size_t(rowBytes) * rows);
            }
            return true;
        }

        int w = 0, h = 0;
        std::vector<uint32_t> rgba;
        if (!loadImageFile(path, w, h, rgba, err, maxSide))
            return false;
        out.format = uint32_t(DXGI_FORMAT_R8G8B8A8_UNORM);
        out.width = uint32_t(w);
        out.height = uint32_t(h);
        out.mips = 1;
        out.fullW = w;
        out.fullH = h;
        out.data.assign(reinterpret_cast<const uint8_t*>(rgba.data()),
                        reinterpret_cast<const uint8_t*>(rgba.data() + rgba.size()));
        out.pitches.push_back(uint32_t(w) * 4);
        out.offsets.push_back(0);
        return true;
    }

    void* createFileView(const FileImage& img, std::string& err)
    {
        if (!g_pDevice)
        {
            err = "no device";
            return nullptr;
        }
        if (!img.mips || img.data.empty() || img.offsets.size() != img.mips)
        {
            err = "empty image";
            return nullptr;
        }

        std::vector<D3D11_SUBRESOURCE_DATA> init(img.mips);
        for (uint32_t m = 0; m < img.mips; ++m)
        {
            init[m].pSysMem = img.data.data() + img.offsets[m];
            init[m].SysMemPitch = img.pitches[m];
        }

        D3D11_TEXTURE2D_DESC td{};
        td.Width = img.width;
        td.Height = img.height;
        td.MipLevels = img.mips;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT(img.format);
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_IMMUTABLE;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        ID3D11Texture2D* tex = nullptr;
        if (FAILED(g_pDevice->CreateTexture2D(&td, init.data(), &tex)) || !tex)
        {
            err = "CreateTexture2D failed (format may be unsupported)";
            return nullptr;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC svd{};
        svd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        svd.Texture2D.MipLevels = img.mips;
        svd.Format = td.Format;

        ID3D11ShaderResourceView* srv = nullptr;
        const HRESULT hr = g_pDevice->CreateShaderResourceView(tex, &svd, &srv);
        tex->Release();
        if (FAILED(hr) || !srv)
        {
            err = "CreateShaderResourceView failed";
            return nullptr;
        }
        return srv;
    }

    void releaseFileView(void* view)
    {
        if (view)
            static_cast<ID3D11ShaderResourceView*>(view)->Release();
    }

    bool exportPng(void* dxTexture, const std::string& path, std::string& err)
    {
        std::vector<uint32_t> pixels;
        int tw = 0, th = 0;
        if (!grabPixels(dxTexture, pixels, tw, th, err))
            return false;

        const std::string full = resolveExportPath(path, err);
        if (!savePng(full, pixels, tw, th, err))
        {
            if (err == "cannot write file")
                err += " (" + full + ")";
            return false;
        }
        logger::info("[texgen] exported {}x{} -> {}", tw, th, full);
        return true;
    }

    std::string defaultExportDir()
    {
        return getDumpsDir();
    }

    static std::string resolveExportPath(const std::string& path, std::string& err)
    {
        namespace fs = std::filesystem;
        try
        {
            fs::path p(path);
            if (!p.has_parent_path())
                p = fs::path(defaultExportDir()) / p;

            std::error_code ec;
            fs::create_directories(p.parent_path(), ec);
            return p.string();
        }
        catch (const std::exception& e)
        {
            err = e.what();
            return path;
        }
    }

    static std::string resolveImportPath(const std::string& path)
    {
        namespace fs = std::filesystem;
        try
        {
            fs::path p(path);
            if (!p.has_parent_path())
            {
                std::error_code ec;
                for (const std::string& dir : { getTexturesDir(), defaultExportDir() })
                {
                    const fs::path candidate = fs::path(dir) / p;
                    if (fs::exists(candidate, ec))
                        return candidate.string();
                }
            }
        }
        catch (const std::exception&)
        {
        }
        return path;
    }

    bool exportDds(void* dxTexture, const std::string& path, std::string& err)
    {
        const std::string full = resolveExportPath(path, err);
        if (!exportDdsFile(dxTexture, full, err))
        {
            if (err == "cannot open file for writing")
                err += " (" + full + ")";
            return false;
        }
        logger::info("[texgen] exported -> {}", full);
        return true;
    }

    static void noteSource(void* dxTexture, const std::string& path)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        if (auto it = g_overrides.find(dxTexture); it != g_overrides.end())
        {
            it->second.sourceFile = path;
            it->second.tinted = false;
        }
    }

    static bool sameBytes(const std::filesystem::path& a, const std::filesystem::path& b)
    {
        std::error_code ec;
        if (std::filesystem::file_size(a, ec) != std::filesystem::file_size(b, ec))
            return false;
        std::ifstream fa(a, std::ios::binary), fb(b, std::ios::binary);
        char ba[65536], bb[65536];
        while (fa && fb)
        {
            fa.read(ba, sizeof(ba));
            fb.read(bb, sizeof(bb));
            if (fa.gcount() != fb.gcount() || std::memcmp(ba, bb, size_t(fa.gcount())) != 0)
                return false;
        }
        return true;
    }

    static std::string keepInLibrary(const std::string& path)
    {
        namespace fs = std::filesystem;
        try
        {
            std::error_code ec;
            const fs::path src = fs::absolute(resolveImportPath(path), ec);
            const fs::path lib = fs::absolute(getTexturesDir(), ec);
            fs::create_directories(lib, ec);
            if (fs::equivalent(src.parent_path(), lib, ec))
                return src.filename().string();

            fs::path dst = lib / src.filename();
            for (int n = 2; fs::exists(dst, ec) && !sameBytes(src, dst); ++n)
                dst = lib / (src.stem().string() + "_" + std::to_string(n) + src.extension().string());
            if (!fs::exists(dst, ec))
            {
                fs::copy_file(src, dst, ec);
                if (ec)
                {
                    logger::warning("[texgen] could not copy {} into {}: {}", src.string(), lib.string(), ec.message());
                    return path;
                }
                logger::info("[texgen] kept {} as {}", src.filename().string(), dst.string());
            }
            return dst.filename().string();
        }
        catch (const std::exception&)
        {
            return path;
        }
    }

    bool importDds(void* dxTexture, const std::string& path, std::string& err)
    {
        if (!importDdsFile(dxTexture, resolveImportPath(path), err))
            return false;

        noteSource(dxTexture, keepInLibrary(path));
        return true;
    }

    bool importImage(void* dxTexture, const std::string& path, std::string& err)
    {
        std::vector<uint32_t> pixels;
        int iw = 0, ih = 0;
        if (!loadImageFile(resolveImportPath(path), iw, ih, pixels, err))
            return false;

        const Params p;
        if (!applyPixels(dxTexture, pixels, iw, ih, p, err))
            return false;

        noteSource(dxTexture, keepInLibrary(path));
        return true;
    }

    const char* formatName(void* dxTexture)
    {
        if (!dxTexture)
            return "?";
        const uint32_t f = asTex(dxTexture)->m_format;
#if defined(BFVE_GAME_BF4)
        const auto name = magic_enum::enum_name(static_cast<TextureFormat>(f));
#else
        const auto name = magic_enum::enum_name(static_cast<fb::TextureFormat>(f));
#endif
        static char buf[64];
        if (name.empty())
        {
            std::snprintf(buf, sizeof(buf), "format %u", f);
            return buf;
        }
        const size_t us = name.find('_');
        const std::string_view leaf = us == std::string_view::npos ? name : name.substr(us + 1);
        std::snprintf(buf, sizeof(buf), "%.*s", int(leaf.size()), leaf.data());
        return buf;
    }

    bool tintExisting(void* dxTexture, const Params& p, std::string& err)
    {
        if (!dxTexture)
        {
            err = "bad DxTexture pointer";
            return false;
        }

        std::lock_guard<std::mutex> lock(g_overrideMutex);
        dropIfStaleLocked(dxTexture);

        fb::DxTexture* tex = asTex(dxTexture);
        auto it = g_overrides.find(dxTexture);
        Override* existing = it == g_overrides.end() ? nullptr : &it->second;

        void* srcSrv = nullptr;
        if (existing && existing->baseSrv)
        {
            srcSrv = existing->baseSrv;
        }
        else if (existing)
        {
            srcSrv = existing->srvLinear;
            if (srcSrv)
            {
                existing->baseSrv = static_cast<ID3D11ShaderResourceView*>(srcSrv);
                existing->baseSrv->AddRef();
            }
        }
        else
        {
            srcSrv = tex->m_shaderViews[0];
        }

        if (!srcSrv)
        {
            err = existing ? "texture has no shader view to read" : "texture is not loaded";
            return false;
        }

        int tw = 0, th = 0;
        tintSize(tex, tw, th);

        bool srgb0 = false, srgb1 = false;
        slotEncodings(tex, existing, srgb0, srgb1);

        int scale = p.upscale >= 4 ? 4 : p.upscale >= 2 ? 2 : 1;
        while (scale > 1 && (std::max)(tw, th) * scale > 4096)
            scale /= 2;
        tw *= scale;
        th *= scale;
        Override fresh;
        if (!renderTintPass(srcSrv, tw, th, p, srgb0, srgb1, fresh, err, scale, true))
            return false;

        const bool isNew = existing == nullptr;
        Override& o = installOverrideLocked(dxTexture, fresh);
        o.params = p;
        o.tinted = true;

        if (isNew && !o.baseSrv)
        {
            auto* orig = static_cast<ID3D11ShaderResourceView*>(srcSrv);
            orig->AddRef();
            o.baseSrv = orig;
        }
        return true;
    }

    bool isClonedTexture(void* p)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        return isClonedLocked(p);
    }

    void* cloneSource(void* clone)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        for (const auto& [copy, src] : g_clones)
            if (copy == clone)
                return src;
        return nullptr;
    }

    bool installCubeOverride(void* dxTexture, ID3D11Texture2D* tex, ID3D11ShaderResourceView* srv, std::string& err)
    {
        if (!tex || !srv)
        {
            err = "no cube objects";
            return false;
        }
        Override fresh;
        fresh.tex = tex;
        fresh.srvLinear = srv;
        fresh.srvSrgb = srv;
        srv->AddRef();
        return installOverride(dxTexture, fresh, err);
    }

    void* cloneTexture(void* dxTexture)
    {
        if (!dxTexture)
            return nullptr;

        fb::DxTexture* src = asTex(dxTexture);
        if (src->m_vtable != fb::DxTexture::VTable())
            return nullptr;
        if (!src->m_shaderViews[0])
            return nullptr;

        auto* copy = static_cast<fb::DxTexture*>(::operator new(sizeof(fb::DxTexture), std::nothrow));
        if (!copy)
            return nullptr;

        std::memcpy(copy, src, sizeof(fb::DxTexture));
        copy->m_refCount = 0x40000000u;
        copy->m_handle = 0xFFFF;

        ID3D11ShaderResourceView* v0 = copy->m_shaderViews[0];
        ID3D11ShaderResourceView* v1 = copy->m_shaderViews[1];
        v0->AddRef();
        if (v1 && v1 != v0)
            v1->AddRef();

        std::lock_guard<std::mutex> lock(g_overrideMutex);
        g_clones.push_back({ copy, dxTexture });
        logger::info("[texgen] cloned DxTexture {} -> {} ({} clone(s) live)",
            dxTexture, static_cast<void*>(copy), g_clones.size());

        return copy;
    }

    namespace
    {
        bool revertLocked(void* dxTexture)
        {
            if (dropIfStaleLocked(dxTexture))
                return true;

            auto it = g_overrides.find(dxTexture);
            if (it == g_overrides.end())
                return false;

            Override& o = it->second;
            if (o.installed)
                restoreEngineViews(asTex(dxTexture), o);

            releaseBase(o);
            releaseOverrideObjects(o, false);
            g_overrides.erase(it);
            return true;
        }

        void releaseClonesLocked()
        {
            if (g_clones.empty())
                return;

            for (const auto& [clone, source] : g_clones)
            {
                (void)source;
                revertLocked(clone);

                fb::DxTexture* c = asTex(clone);
                ID3D11ShaderResourceView* v0 = c->m_shaderViews[0];
                ID3D11ShaderResourceView* v1 = c->m_shaderViews[1];
                if (v0)
                    v0->Release();
                if (v1 && v1 != v0)
                    v1->Release();

                ::operator delete(clone);
            }

            logger::info("[texgen] released {} cloned texture(s)", g_clones.size());
            g_clones.clear();
        }
    }

    void releaseClones()
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        releaseClonesLocked();
    }

    void onEngineReleasingViews(void* dxTexture)
    {
        if (!dxTexture)
            return;

        std::lock_guard<std::mutex> lock(g_overrideMutex);
        if (isClonedLocked(dxTexture))
            return;

        auto it = g_overrides.find(dxTexture);
        if (it == g_overrides.end() || !it->second.installed)
            return;

        Override& o = it->second;
        restoreEngineViews(asTex(dxTexture), o);
        o.originalSrv0 = nullptr;
        o.originalSrv1 = nullptr;
        static uint32_t released = 0;
        if (++released <= 8 || (released % 64) == 0)
            logger::info("[texgen] engine released the views of an edited texture {} ({} total)", dxTexture, released);
    }

    void onEngineCreatedViews(void* dxTexture)
    {
        if (!dxTexture)
            return;
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        auto it = g_overrides.find(dxTexture);
        if (it == g_overrides.end() || !it->second.srvLinear)
            return;
        Override& o = it->second;
        if (o.installed)
        {
            if (asTex(dxTexture)->m_shaderViews[0] == o.srvLinear)
                return;
            o.installed = false;
        }
        static uint32_t missed = 0;
        if (identityOf(dxTexture, o) != Ident::Same)
        {
            if (++missed <= 8 || (missed % 64) == 0)
                logger::info("[texgen] engine rebuilt {} but it is another texture now - not re-installed ({} misses)", dxTexture, missed);
            return;
        }
        if (!tryInstallLocked(asTex(dxTexture), o))
        {
            if (++missed <= 8 || (missed % 64) == 0)
                logger::info("[texgen] engine rebuilt {} without views yet - not re-installed ({} misses)", dxTexture, missed);
            return;
        }
        static uint32_t count = 0;
        if (++count <= 8 || (count % 64) == 0)
            logger::info("[texgen] engine rebuilt {} - edit re-installed at once ({} total)", dxTexture, count);
    }

    void setAssetResolvers(PathOfTexture pathOf, TextureOfPath textureOf)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        g_pathOf = pathOf;
        g_textureOf = textureOf;
    }

    void* originalView(void* dxTexture)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        auto it = g_overrides.find(dxTexture);
        return it == g_overrides.end() ? nullptr : it->second.baseSrv;
    }

    void reassertOverrides()
    {
        std::vector<std::pair<std::string, EditInfo>> retarget;
        {
            std::lock_guard<std::mutex> lock(g_overrideMutex);
            if (g_overrides.empty())
                return;

            static uint32_t reinstalled = 0;
            std::vector<void*> stale;

            for (auto& [dxTexture, o] : g_overrides)
            {
                if (!o.srvLinear)
                    continue;

                const Ident id = identityOf(dxTexture, o);
                if (id == Ident::Different)
                {
                    stale.push_back(dxTexture);
                    if (!o.assetPath.empty() && (o.tinted || !o.sourceFile.empty()))
                    {
                        EditInfo e;
                        e.sourceFile = o.sourceFile;
                        e.tinted = o.tinted;
                        e.params = o.params;
                        retarget.emplace_back(o.assetPath, e);
                    }
                    continue;
                }
                if (id != Ident::Same)
                    continue;

                const bool wasInstalled = o.installed;
                void* const slot0 = asTex(dxTexture)->m_shaderViews[0];
                void* const slot1 = asTex(dxTexture)->m_shaderViews[1];
                if (o.installed)
                {
                    if (slot0 == o.srvLinear)
                        continue;
                    o.installed = false;
                }

                if (!tryInstallLocked(asTex(dxTexture), o))
                    continue;

                ++reinstalled;
                if (reinstalled <= 10)
                    logger::info("[texgen] re-applied edit to {}: slots held {} / {} (ours {}), was {}, resource {}",
                                 dxTexture, slot0, slot1, static_cast<void*>(o.srvLinear),
                                 wasInstalled ? "installed" : "waiting for residency",
                                 static_cast<void*>(asTex(dxTexture)->m_resource));
                static uint32_t lastReport = 0, atReport = 0;
                const uint32_t now = uint32_t(GetTickCount64() / 1000);
                if (now != lastReport)
                {
                    logger::info("[texgen] re-applied edits {} time(s) this second (engine rebuilt the views; {} total)",
                                 reinstalled - atReport, reinstalled);
                    lastReport = now;
                    atReport = reinstalled;
                }
            }

            for (void* dead : stale)
                dropIfStaleLocked(dead);
        }

        if (!g_textureOf)
            return;
        for (auto& [path, e] : retarget)
        {
            void* now = g_textureOf(path);
            if (!now)
            {
                std::lock_guard<std::mutex> lock(g_overrideMutex);
                g_pendingReplays.emplace_back(path, e);
                continue;
            }
            e.dxTexture = now;
            std::string err;
            if (replayEdit(e, err))
                logger::info("[texgen] moved the edit of {} to its re-created texture {}", path, now);
            else
                logger::warning("[texgen] could not move the edit of {}: {}", path, err);
        }
    }

    void replayPending()
    {
        std::vector<std::pair<std::string, EditInfo>> ready;
        {
            std::lock_guard<std::mutex> lock(g_overrideMutex);
            if (g_pendingReplays.empty() || !g_textureOf)
                return;
            for (size_t i = 0; i < g_pendingReplays.size();)
            {
                void* now = g_textureOf(g_pendingReplays[i].first);
                if (now && g_overrides.find(now) == g_overrides.end())
                {
                    g_pendingReplays[i].second.dxTexture = now;
                    ready.push_back(std::move(g_pendingReplays[i]));
                    g_pendingReplays.erase(g_pendingReplays.begin() + i);
                }
                else
                    ++i;
            }
        }
        for (auto& [path, e] : ready)
        {
            std::string err;
            if (replayEdit(e, err))
                logger::info("[texgen] replayed the edit of {} on its streamed-in texture", path);
            else
                logger::warning("[texgen] replay of {}: {}", path, err);
        }
    }

    bool revert(void* dxTexture)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        return revertLocked(dxTexture);
    }

    void revertAll()
    {
        {
            std::lock_guard<std::mutex> lock(g_overrideMutex);
            while (!g_overrides.empty())
                revertLocked(g_overrides.begin()->first);
            releaseClonesLocked();
        }
        g_ui.clear();
    }

    bool hasOverride(void* dxTexture)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        return g_overrides.find(dxTexture) != g_overrides.end();
    }

    void* installedView(void* dxTexture)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        auto it = g_overrides.find(dxTexture);
        return it != g_overrides.end() && it->second.installed ? static_cast<void*>(it->second.srvLinear) : nullptr;
    }

    std::vector<EditInfo> listEdits()
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        std::vector<EditInfo> out;
        out.reserve(g_overrides.size());

        for (const auto& [dxTexture, o] : g_overrides)
        {
            if (o.sourceFile.empty() && !o.tinted)
                continue;

            EditInfo e;
            e.dxTexture = dxTexture;
            e.sourceFile = o.sourceFile;
            e.tinted = o.tinted;
            e.params = o.params;
            out.push_back(std::move(e));
        }

        return out;
    }

    bool replayEdit(const EditInfo& edit, std::string& err)
    {
        if (!edit.dxTexture)
        {
            err = "bad DxTexture pointer";
            return false;
        }

        if (!edit.sourceFile.empty())
        {
            const bool dds = edit.sourceFile.size() >= 4 &&
                _stricmp(edit.sourceFile.c_str() + edit.sourceFile.size() - 4, ".dds") == 0;

            if (!(dds ? importDds(edit.dxTexture, edit.sourceFile, err)
                      : importImage(edit.dxTexture, edit.sourceFile, err)))
                return false;
        }

        if (edit.tinted && !tintExisting(edit.dxTexture, edit.params, err))
            return false;

        return true;
    }
    size_t overrideCount()
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        return g_overrides.size();
    }

    void* currentSrv(void* dxTexture)
    {
        std::lock_guard<std::mutex> lock(g_overrideMutex);
        auto it = g_overrides.find(dxTexture);
        return it == g_overrides.end() ? nullptr : it->second.srvLinear;
    }

    namespace
    {
        struct BatchState
        {
            std::vector<BatchItem> items;
            size_t next = 0;
            bool dds = true;
            bool running = false;
            std::string folder = "all";
        };

        BatchState g_batch;
        std::vector<std::future<void>> g_writers;
        std::atomic<uint32_t> g_batchDone{ 0 };
        std::atomic<uint32_t> g_batchFailed{ 0 };

        constexpr size_t MAX_WRITERS = 4;
        constexpr size_t READS_PER_FRAME = 2;

        void reapWriters()
        {
            for (size_t i = 0; i < g_writers.size(); )
            {
                const bool done = !g_writers[i].valid() ||
                    g_writers[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready;
                if (done)
                    g_writers.erase(g_writers.begin() + ptrdiff_t(i));
                else
                    ++i;
            }
        }

        std::string sanitizeRelative(const std::string& name)
        {
            std::string out;
            out.reserve(name.size());
            for (char c : name)
            {
                if (c == '\\')
                    c = '/';
                if (c == '/')
                {
                    if (!out.empty() && out.back() != '/')
                        out.push_back('/');
                    continue;
                }
                const bool bad = uint8_t(c) < 0x20 || std::strchr(":*?\"<>|", c) != nullptr;
                out.push_back(bad ? '_' : c);
            }
            while (!out.empty() && out.front() == '/')
                out.erase(out.begin());
            while (!out.empty() && out.back() == '/')
                out.pop_back();
            return out.empty() ? std::string("texture") : out;
        }
    }

    std::string batchFolder()
    {
        return defaultExportDir() + "/" + g_batch.folder;
    }

    void beginBatch(std::vector<BatchItem> items, bool asDds, const std::string& subDir)
    {
        cancelBatch();

        g_batch.items = std::move(items);
        g_batch.next = 0;
        g_batch.dds = asDds;
        g_batch.folder = subDir.empty() ? std::string("all") : subDir;
        g_batch.running = !g_batch.items.empty();
        g_batchDone = 0;
        g_batchFailed = 0;

        if (g_batch.running)
            logger::info("[texgen] exporting {} texture(s) as {} -> {}",
                g_batch.items.size(), asDds ? "dds" : "png", batchFolder());
    }

    void cancelBatch()
    {
        g_batch.running = false;
        g_batch.items.clear();
        g_batch.next = 0;
    }

    bool batchActive() { return g_batch.running; }
    size_t batchTotal() { return g_batch.items.size(); }
    size_t batchDone() { return g_batchDone.load(); }
    size_t batchFailed() { return g_batchFailed.load(); }

    void tickBatch()
    {
        reapWriters();
        if (!g_batch.running)
            return;

        for (size_t n = 0; n < READS_PER_FRAME && g_batch.next < g_batch.items.size(); ++n)
        {
            if (g_writers.size() >= MAX_WRITERS)
                return;

            const BatchItem item = g_batch.items[g_batch.next++];
            const std::string rel = sanitizeRelative(item.name);

            if (!item.dxTexture || asTex(item.dxTexture)->m_vtable != fb::DxTexture::VTable())
            {
                ++g_batchFailed;
                continue;
            }

            std::string err;
            const std::string path = resolveExportPath(
                batchFolder() + "/" + rel + (g_batch.dds ? ".dds" : ".png"), err);

            if (g_batch.dds)
            {
                DdsImage img;
                if (!readDdsImage(item.dxTexture, img, err))
                {
                    ++g_batchFailed;
                    logger::warning("[texgen] export {}: {}", rel, err);
                    continue;
                }

                g_writers.push_back(std::async(std::launch::async,
                    [img = std::move(img), path]
                    {
                        std::string e;
                        if (writeDdsImage(img, path, e))
                            ++g_batchDone;
                        else
                        {
                            ++g_batchFailed;
                            logger::warning("[texgen] write {}: {}", path, e);
                        }
                    }));
            }
            else
            {
                std::vector<uint32_t> pixels;
                int tw = 0, th = 0;
                if (!grabPixels(item.dxTexture, pixels, tw, th, err))
                {
                    ++g_batchFailed;
                    logger::warning("[texgen] export {}: {}", rel, err);
                    continue;
                }

                g_writers.push_back(std::async(std::launch::async,
                    [pixels = std::move(pixels), path, tw, th]
                    {
                        std::string e;
                        if (savePng(path, pixels, tw, th, e))
                            ++g_batchDone;
                        else
                        {
                            ++g_batchFailed;
                            logger::warning("[texgen] write {}: {}", path, e);
                        }
                    }));
            }
        }

        if (g_batch.next >= g_batch.items.size() && g_writers.empty())
        {
            g_batch.running = false;
            logger::info("[texgen] export finished: {} written, {} failed -> {}",
                g_batchDone.load(), g_batchFailed.load(), batchFolder());
        }
    }

    void shutdown()
    {
        cancelBatch();
        for (std::future<void>& f : g_writers)
            if (f.valid())
                f.wait();
        g_writers.clear();
    }

    bool renderUI(void* dxTexture, const char* suggestedName)
    {
        ImGui::PushID(dxTexture);
        struct IdScope { ~IdScope() { ImGui::PopID(); } } idScope;

        UiState& st = g_ui[dxTexture];

        Params& p = st.p;
        std::string& lastError = st.lastError;
        std::string& lastInfo = st.lastInfo;
        auto& imagePath = st.imagePath; // sizeof() must stay 512
        int& fileFormat = st.fileFormat;

        const std::string seed = suggestedName ? suggestedName : "";
        if (!st.seeded || seed != st.lastSeed)
        {
            st.seeded = true;
            st.lastSeed = seed;
            lastError.clear();
            lastInfo.clear();

            std::string safe = seed.empty() ? std::string("texture") : seed;
            const size_t slash = safe.find_last_of("/\\");
            if (slash != std::string::npos)
                safe = safe.substr(slash + 1);
            for (char& c : safe)
                if (std::strchr("/\\:*?\"<>|", c))
                    c = '_';

            std::snprintf(imagePath, sizeof(imagePath), "%s%s",
                safe.c_str(), fileFormat == 0 ? ".dds" : ".png");
        }

        if (hasOverride(dxTexture))
        {
            ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "edited");
            ImGui::SameLine();
            if (ImGui::SmallButton("restore original"))
            {
                revert(dxTexture);
                return true;
            }
        }

        bool regenerated = false;

        ImGui::SeparatorText("Tint");
        {
            fb::DxTexture* tex = asTex(dxTexture);
            ImGui::TextDisabled("%s   resource DXGI %u   view DXGI %u%s", formatName(dxTexture),
                tex->m_resFormat, tex->m_shaderFormat, tex->m_srgb ? "   sRGB" : "");
            static const char* scales[] = { "1x", "2x", "4x" };
            int sel = p.upscale >= 4 ? 2 : p.upscale >= 2 ? 1 : 0;
            ImGui::SetNextItemWidth(90.0f);
            if (ImGui::Combo("upscale", &sel, scales, 3))
                p.upscale = sel == 2 ? 4 : sel == 1 ? 2 : 1;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("%ux%u (source %ux%u, max 4096)",
                    tex->m_width * (p.upscale >= 4 ? 4 : p.upscale >= 2 ? 2 : 1), tex->m_height * (p.upscale >= 4 ? 4 : p.upscale >= 2 ? 2 : 1),
                    tex->m_width, tex->m_height);
            ImGui::PushItemWidth(180.0f);
            ImGui::SliderFloat("sharpen", &p.sharpness, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("edge-only unsharp mask, baked into every mip");
            ImGui::SliderFloat("detail", &p.detail, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("grain at the new pixel size, matched to the texture");
            ImGui::PopItemWidth();
        }
        ImGui::ColorEdit4("color", p.colorA, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
        ImGui::PushItemWidth(180.0f);
        ImGui::SliderFloat("brightness", &p.brightness, 0.0f, 4.0f);
        ImGui::PopItemWidth();

        if (ImGui::Button("Apply tint"))
        {
            lastError.clear();
            lastInfo.clear();
            if (tintExisting(dxTexture, p, lastError))
                regenerated = true;
            else
                logger::error("[texgen] {}", lastError);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            p = Params{};
        }
        ImGui::TextDisabled("always applied to the original, so repeat applies do not stack");

        ImGui::SeparatorText("Export / import");

        ImGui::RadioButton("DDS", &fileFormat, 0);
        ImGui::SameLine();
        ImGui::RadioButton("PNG", &fileFormat, 1);
        ImGui::SameLine();
        const bool wantDds = fileFormat == 0;
        ImGui::TextDisabled(wantDds ? "keeps the engine format and mips"
                                    : "decodes to RGBA8, opens anywhere");

        if (!wantDds)
        {
            ImGui::Checkbox("opaque alpha", &pngOpaqueAlpha);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("alpha often holds a gloss mask; keep it off to import the PNG back");
        }

        {
            std::string ip = imagePath;
            const size_t slash = ip.find_last_of("/\\");
            const size_t dot = ip.find_last_of('.');
            if (dot != std::string::npos && (slash == std::string::npos || dot > slash))
                ip.resize(dot);
            if (!ip.empty())
            {
                ip += wantDds ? ".dds" : ".png";
                if (ip != imagePath)
                    std::snprintf(imagePath, sizeof(imagePath), "%s", ip.c_str());
            }
        }

        ImGui::PushItemWidth(-180.0f);
        ImGui::InputTextWithHint("##imgpath", "filename, or a full path",
                                 imagePath, sizeof(imagePath));
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::SmallButton("Browse"))
        {
            ui::filedlg::open("Choose an image to import", ui::filedlg::Mode::Open,
                              defaultExportDir(), imagePath,
                              { "dds", "png", "jpg", "jpeg", "bmp", "tga" });
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(".dds imports as stored, everything else is decoded");

        ImGui::SameLine();
        if (ImGui::SmallButton("Save as"))
        {
            ui::filedlg::open("Export this texture to", ui::filedlg::Mode::Save,
                              defaultExportDir(), imagePath,
                              { wantDds ? "dds" : "png" });
        }

        if (std::string chosen; ui::filedlg::draw(chosen))
        {
            std::snprintf(imagePath, sizeof(imagePath), "%s", chosen.c_str());

            const std::string picked = chosen.size() >= 4
                ? chosen.substr(chosen.size() - 4) : std::string{};
            if (_stricmp(picked.c_str(), ".dds") == 0) fileFormat = 0;
            else if (_stricmp(picked.c_str(), ".png") == 0) fileFormat = 1;
        }

        if (ImGui::Button("Export"))
        {
            lastError.clear();
            lastInfo.clear();

            if (imagePath[0] == 0)
            {
                lastError = "enter a filename first";
            }
            else
            {
                const bool ok = wantDds ? exportDds(dxTexture, imagePath, lastError)
                                        : exportPng(dxTexture, imagePath, lastError);
                if (ok)
                    lastInfo = "exported (full path in the log)";
                else
                    logger::error("[texgen] export: {}", lastError);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Import"))
        {
            lastError.clear();
            lastInfo.clear();

            if (imagePath[0] == 0)
            {
                lastError = "enter a filename first";
            }
            else if (wantDds)
            {
                if (importDds(dxTexture, imagePath, lastError))
                {
                    lastInfo = "imported, original format kept";
                    regenerated = true;
                    logger::info("[texgen] imported dds {}", imagePath);
                }
            }
            else if (importImage(dxTexture, imagePath, lastError))
            {
                lastInfo = "imported";
                regenerated = true;
                logger::info("[texgen] imported {}", imagePath);
            }

            if (!lastError.empty())
                logger::error("[texgen] import: {}", lastError);
        }

        ImGui::TextDisabled("export, edit it anywhere, then import the result back");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("imports are copied to Documents/VisEnvEditor/Textures; the config names them");

        const std::string outDir = defaultExportDir();
        ImGui::TextDisabled("bare filenames go to:");
        ImGui::SameLine();
        ImGui::TextWrapped("%s", outDir.c_str());
        if (ImGui::SmallButton("copy folder path"))
            ImGui::SetClipboardText(outDir.c_str());

        if (!lastError.empty())
            ImGui::TextColored(ImVec4(0.9f, 0.4f, 0.4f, 1.0f), "%s", lastError.c_str());
        else if (!lastInfo.empty())
            ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%s", lastInfo.c_str());

        return regenerated;
    }
}
