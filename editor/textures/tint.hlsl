// compiled offline with fxc:
//   fxc /T vs_4_0 /E VSMain /Fh tint_vs.h /Vn g_tintVS tint.hlsl
//   fxc /T ps_4_0 /E PSMain /Fh tint_ps.h /Vn g_tintPS tint.hlsl
Texture2D src : register(t0);
SamplerState smp : register(s0);

cbuffer TintCB : register(b0)
{
    float4 gTint; // rgb multiply, a scales alpha
    float4 gParams; // x = saturation, y = brightness, z = sharpen 0..1, w = upscale factor
    float4 gTexel; // xy = 1 / source size, zw = source size
    float4 gEnhance; // x = detail 0..1, y = mode: 0 plain, 1 resample+enhance, 2 mip level
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(uint id : SV_VertexID)
{
    VSOut o;
    float2 uv = float2((id << 1) & 2, id & 2);
    o.uv = uv;
    o.pos = float4(uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    return o;
}

float4 texel(int2 p)
{
    p = clamp(p, int2(0, 0), int2(gTexel.zw) - 1);
    return src.SampleLevel(smp, (float2(p) + 0.5f) * gTexel.xy, 0);
}

float lanczos3(float x)
{
    x = abs(x);
    if (x < 1e-4f) return 1.0f;
    if (x >= 3.0f) return 0.0f;
    float px = 3.14159265f * x;
    return 3.0f * sin(px) * sin(px / 3.0f) / (px * px);
}

// Lanczos-3 6x6, clamped to the inner 4x4 range
float4 resample(float2 uv, out float4 mn, out float4 mx)
{
    float2 pos = uv * gTexel.zw - 0.5f;
    int2 base = int2(floor(pos));
    float2 f = pos - float2(base);

    float wx[6], wy[6];
    [unroll] for (int i = 0; i < 6; ++i)
    {
        wx[i] = lanczos3(f.x - float(i - 2));
        wy[i] = lanczos3(f.y - float(i - 2));
    }
    float4 sum = 0;
    float wsum = 0;
    mn = 1e9f;
    mx = -1e9f;
    [unroll] for (int y = 0; y < 6; ++y)
    {
        [unroll] for (int x = 0; x < 6; ++x)
        {
            float w = wx[x] * wy[y];
            float4 c = texel(base + int2(x - 2, y - 2));
            sum += c * w;
            wsum += w;
            if (x >= 1 && x <= 4 && y >= 1 && y <= 4)
            {
                mn = min(mn, c);
                mx = max(mx, c);
            }
        }
    }
    return clamp(sum / wsum, mn, mx);
}

void neighbourhood(float2 uv, out float3 mean, out float3 mn, out float3 mx)
{
    int2 c = int2(floor(uv * gTexel.zw));
    mean = 0;
    mn = 1e9f;
    mx = -1e9f;
    [unroll] for (int y = -1; y <= 1; ++y)
        [unroll] for (int x = -1; x <= 1; ++x)
        {
            float3 v = texel(c + int2(x, y)).rgb;
            mean += v;
            mn = min(mn, v);
            mx = max(mx, v);
        }
    mean /= 9.0f;
}

float hash(float2 p)
{
    return frac(sin(dot(p, float2(12.9898f, 78.233f))) * 43758.5453f);
}

// mode 1: lanczos, bounded unsharp, residual detail, grain
float4 enhanced(float2 uv)
{
    float4 mn4, mx4;
    float4 base = resample(uv, mn4, mx4);
    float3 c = base.rgb;

    float3 mean, mn, mx;
    neighbourhood(uv, mean, mn, mx);
    float contrast = max(max(mx.r - mn.r, mx.g - mn.g), mx.b - mn.b);

    if (gParams.z > 0.0f)
    {
        float edge = smoothstep(0.02f, 0.18f, contrast);
        c += 2.0f * gParams.z * edge * (base.rgb - mean);
        c = clamp(c, mn, mx);
    }

    if (gEnhance.x > 0.0f && gParams.w > 1.5f)
    {
        float3 residual = texel(int2(floor(uv * gTexel.zw))).rgb - mean;
        float amp = min(length(residual), 0.25f);
        float flat = 1.0f - smoothstep(0.05f, 0.3f, contrast);
        float g = hash(floor(uv * gTexel.zw * gParams.w)) * 2.0f - 1.0f;
        c += gEnhance.x * flat * (amp * 0.8f + 0.015f) * g;
    }
    return float4(saturate(c), base.a);
}

// mode 2: one mip from the previous, bilinear = 2x2 box
float4 mipLevel(float2 uv)
{
    float4 box = src.SampleLevel(smp, uv, 0);
    if (gParams.z <= 0.0f)
        return box;
    float2 d = gTexel.xy * 2.0f;
    float3 n = 0;
    float3 mn = box.rgb, mx = box.rgb;
    float3 s;
    s = src.SampleLevel(smp, uv + float2(-d.x, 0), 0).rgb; n += s; mn = min(mn, s); mx = max(mx, s);
    s = src.SampleLevel(smp, uv + float2(d.x, 0), 0).rgb; n += s; mn = min(mn, s); mx = max(mx, s);
    s = src.SampleLevel(smp, uv + float2(0, -d.y), 0).rgb; n += s; mn = min(mn, s); mx = max(mx, s);
    s = src.SampleLevel(smp, uv + float2(0, d.y), 0).rgb; n += s; mn = min(mn, s); mx = max(mx, s);
    n *= 0.25f;
    float contrast = max(max(mx.r - mn.r, mx.g - mn.g), mx.b - mn.b);
    float edge = smoothstep(0.02f, 0.18f, contrast);
    float3 c = box.rgb + gParams.z * edge * (box.rgb - n);
    return float4(clamp(c, mn, mx), box.a);
}

float4 PSMain(VSOut i) : SV_Target
{
    float4 c;
    if (gEnhance.y > 1.5f)
        c = mipLevel(i.uv);
    else if (gEnhance.y > 0.5f)
        c = enhanced(i.uv);
    else
        c = src.Sample(smp, i.uv);

    float grey = dot(c.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    c.rgb = lerp(grey.xxx, c.rgb, gParams.x);
    c.rgb *= gParams.y;
    c *= gTint;
    return c;
}
