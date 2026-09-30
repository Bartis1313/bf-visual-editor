#pragma once

#include <cmath>
#include <numbers>

namespace fb
{
    inline Vec2 vec2(float x, float y) { Vec2 v{}; v.m_x = x; v.m_y = y; return v; }
    inline Vec2 operator+(const Vec2& a, const Vec2& b) { return vec2(a.m_x + b.m_x, a.m_y + b.m_y); }
    inline Vec2 operator-(const Vec2& a, const Vec2& b) { return vec2(a.m_x - b.m_x, a.m_y - b.m_y); }
    inline Vec2 operator*(const Vec2& a, float s) { return vec2(a.m_x * s, a.m_y * s); }
    inline Vec2 operator/(const Vec2& a, float s) { const float i = 1.0f / s; return a * i; }

    inline Vec3 vec3(float x, float y, float z) { Vec3 v{}; v.m_x = x; v.m_y = y; v.m_z = z; return v; }
    inline Vec3 vec3(const float* p) { return vec3(p[0], p[1], p[2]); }

    inline Vec3 operator+(const Vec3& a, const Vec3& b) { return vec3(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z); }
    inline Vec3 operator-(const Vec3& a, const Vec3& b) { return vec3(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z); }
    inline Vec3 operator*(const Vec3& a, float s) { return vec3(a.m_x * s, a.m_y * s, a.m_z * s); }
    inline Vec3 operator*(float s, const Vec3& a) { return a * s; }
    inline Vec3 operator*(const Vec3& a, const Vec3& b) { return vec3(a.m_x * b.m_x, a.m_y * b.m_y, a.m_z * b.m_z); }
    inline Vec3 operator/(const Vec3& a, float s) { const float i = 1.0f / s; return a * i; }
    inline Vec3 operator-(const Vec3& a) { return vec3(-a.m_x, -a.m_y, -a.m_z); }
    inline Vec3& operator+=(Vec3& a, const Vec3& b) { a.m_x += b.m_x; a.m_y += b.m_y; a.m_z += b.m_z; return a; }
    inline Vec3& operator-=(Vec3& a, const Vec3& b) { a.m_x -= b.m_x; a.m_y -= b.m_y; a.m_z -= b.m_z; return a; }
    inline Vec3& operator*=(Vec3& a, float s) { a.m_x *= s; a.m_y *= s; a.m_z *= s; return a; }
    inline Vec3& operator*=(Vec3& a, const Vec3& b) { a.m_x *= b.m_x; a.m_y *= b.m_y; a.m_z *= b.m_z; return a; }

    inline float dot(const Vec3& a, const Vec3& b) { return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z; }
    inline Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return vec3(a.m_y * b.m_z - a.m_z * b.m_y, a.m_z * b.m_x - a.m_x * b.m_z, a.m_x * b.m_y - a.m_y * b.m_x);
    }
    inline float lengthSq(const Vec3& a) { return dot(a, a); }
    inline float length(const Vec3& a) { return std::sqrt(lengthSq(a)); }
    inline Vec3 normalized(const Vec3& a) { const float l = length(a); return l > 1e-12f ? a * (1.0f / l) : Vec3{}; }
    inline float distance(const Vec3& a, const Vec3& b) { return length(a - b); }
    inline float distanceSq(const Vec3& a, const Vec3& b) { return lengthSq(a - b); }
    inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) { return a + (b - a) * t; }
    inline Vec3 vmin(const Vec3& a, const Vec3& b) { return vec3(a.m_x < b.m_x ? a.m_x : b.m_x, a.m_y < b.m_y ? a.m_y : b.m_y, a.m_z < b.m_z ? a.m_z : b.m_z); }
    inline Vec3 vmax(const Vec3& a, const Vec3& b) { return vec3(a.m_x > b.m_x ? a.m_x : b.m_x, a.m_y > b.m_y ? a.m_y : b.m_y, a.m_z > b.m_z ? a.m_z : b.m_z); }
    inline float sum(const Vec3& a) { return a.m_x + a.m_y + a.m_z; }
    inline float maxComponent(const Vec3& a) { return a.m_x > a.m_y ? (a.m_x > a.m_z ? a.m_x : a.m_z) : (a.m_y > a.m_z ? a.m_y : a.m_z); }
    inline float& at(Vec3& a, int i) { return (&a.m_x)[i]; }
    inline float at(const Vec3& a, int i) { return (&a.m_x)[i]; }

    // world = right * x + up * y + forward * z + trans
    inline Vec3 transformPoint(const LinearTransform& t, const Vec3& p)
    {
        return t.m_trans + t.m_right * p.m_x + t.m_up * p.m_y + t.m_forward * p.m_z;
    }
    inline Vec3 transformVector(const LinearTransform& t, const Vec3& v)
    {
        return t.m_right * v.m_x + t.m_up * v.m_y + t.m_forward * v.m_z;
    }
    inline float determinant(const LinearTransform& t) { return dot(t.m_right, cross(t.m_up, t.m_forward)); }
    inline LinearTransform identityTransform()
    {
        LinearTransform t{};
        t.m_right = vec3(1, 0, 0); t.m_up = vec3(0, 1, 0); t.m_forward = vec3(0, 0, 1); t.m_trans = Vec3{};
        return t;
    }
    inline LinearTransform lookTransform(const Vec3& forward, const Vec3& pos)
    {
        LinearTransform t = identityTransform();
        Vec3 f = normalized(forward);
        if (lengthSq(f) < 1e-12f) f = vec3(0, 0, 1);
        const Vec3 ref = std::fabs(f.m_y) > 0.9f ? vec3(1, 0, 0) : vec3(0, 1, 0);
        const Vec3 r = normalized(cross(ref, f));
        t.m_right = r;
        t.m_up = cross(f, r);
        t.m_forward = f;
        t.m_trans = pos;
        return t;
    }
    inline void boxCorners(const LinearTransform& t, const Vec3& mn, const Vec3& mx, Vec3 (&out)[8])
    {
        for (int i = 0; i < 8; ++i)
            out[i] = transformPoint(t, vec3((i & 1) ? mx.m_x : mn.m_x, (i & 2) ? mx.m_y : mn.m_y, (i & 4) ? mx.m_z : mn.m_z));
    }
    inline void boxCorners(const Vec3& center, const Vec3& extents, Vec3 (&out)[8])
    {
        for (int i = 0; i < 8; ++i)
            out[i] = center + vec3((i & 1) ? extents.m_x : -extents.m_x, (i & 2) ? extents.m_y : -extents.m_y, (i & 4) ? extents.m_z : -extents.m_z);
    }
    constexpr int BOX_EDGES[12][2] = { {0,1},{1,3},{3,2},{2,0},{4,5},{5,7},{7,6},{6,4},{0,4},{1,5},{2,6},{3,7} };
}

namespace math
{
    constexpr float PI = std::numbers::pi_v<float>;
    constexpr double PI_D = std::numbers::pi;
    constexpr float PI_HALF = PI * 0.5f;
    constexpr float PI_2 = PI * 2.0f;
    constexpr float DEG = PI / 180.0f;
    [[nodiscard]] constexpr float DEG2RAD(float degrees) { return degrees * (PI / 180.0f); }
    [[nodiscard]] constexpr float RAD2DEG(float radians) { return radians * (180.0f / PI); }
    [[nodiscard]] inline float clamp01(float v) { return v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v; }
    // reinhard + gamma
    [[nodiscard]] inline float display(float linear, float exposure = 1.0f)
    {
        float v = linear * exposure;
        v = v / (1.0f + v);
        return std::pow(v < 0.0f ? 0.0f : v, 1.0f / 2.2f);
    }
    [[nodiscard]] inline fb::Vec3 cosineDir(const fb::Vec3& n, float u1, float u2)
    {
        const float r = std::sqrt(u1), phi = PI_2 * u2;
        const float x = r * std::cos(phi), y = r * std::sin(phi), z = std::sqrt(1.0f - u1 > 0.0f ? 1.0f - u1 : 0.0f);
        const fb::Vec3 a = std::fabs(n.m_x) > 0.9f ? fb::vec3(0, 1, 0) : fb::vec3(1, 0, 0);
        const fb::Vec3 t = fb::normalized(fb::cross(a, n));
        const fb::Vec3 b = fb::cross(n, t);
        return t * x + b * y + n * z;
    }
    // VE outdoor light rotations
    [[nodiscard]] inline fb::Vec3 sunDirection(float rotXDeg, float rotYDeg)
    {
        const float th = DEG2RAD(rotXDeg), ph = DEG2RAD(rotYDeg);
        return fb::vec3(std::sin(th) * std::cos(ph), std::sin(ph), std::cos(th) * std::cos(ph));
    }
}
