#include "enlighten_internal.h"

#include <cstdio>

namespace editor::enlighten::detail
{
    namespace
    {
        constexpr uint32_t DDS_MAGIC = 0x20534444; // "DDS "
        constexpr uint32_t FOURCC_DX10 = 0x30315844; // "DX10"
        constexpr uint32_t DDPF_FOURCC = 0x4;

#pragma pack(push, 1)
        struct DdsPixelFormat
        {
            uint32_t size, flags, fourCC, rgbBitCount, rMask, gMask, bMask, aMask;
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
    }

    bool writeAtlasDds(const std::string& path, uint32_t w, uint32_t h, uint32_t dxgi, uint32_t bpp,
                       const std::vector<uint8_t>& pixels, std::string& err)
    {
        DdsHeader hd{};
        hd.size = sizeof(DdsHeader);
        hd.flags = 0x1 | 0x2 | 0x4 | 0x1000 | 0x8; // caps, height, width, pixelformat, pitch
        hd.height = h;
        hd.width = w;
        hd.pitchOrLinearSize = w * bpp;
        hd.mipMapCount = 1;
        hd.ddspf.size = sizeof(DdsPixelFormat);
        hd.ddspf.flags = DDPF_FOURCC;
        hd.ddspf.fourCC = FOURCC_DX10;
        hd.caps = 0x1000;
        DdsHeaderDxt10 dx{};
        dx.dxgiFormat = dxgi;
        dx.resourceDimension = 3; // texture2d
        dx.arraySize = 1;

        FILE* f = nullptr;
        if (fopen_s(&f, path.c_str(), "wb") != 0 || !f)
        {
            err = "cannot write " + path;
            return false;
        }
        fwrite(&DDS_MAGIC, 4, 1, f);
        fwrite(&hd, sizeof(hd), 1, f);
        fwrite(&dx, sizeof(dx), 1, f);
        fwrite(pixels.data(), 1, pixels.size(), f);
        fclose(f);
        return true;
    }
}
