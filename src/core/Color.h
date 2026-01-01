#pragma once
#include <windows.h>
#include <cstdint>

// 简单颜色结构，方便后续 Canvas 使用
struct Color
{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    constexpr Color(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 255)
        : a(_a), r(_r), g(_g), b(_b) {}

    COLORREF ToCOLORREF() const
    {
        return RGB(r, g, b);
    }

    static constexpr Color Black()  { return Color(0, 0, 0); }
    static constexpr Color White()  { return Color(255, 255, 255); }
    static constexpr Color Red()    { return Color(255, 0, 0); }
    static constexpr Color Green()  { return Color(0, 255, 0); }
    static constexpr Color Blue()   { return Color(0, 0, 255); }
    static constexpr Color Gray()   { return Color(128, 128, 128); }
    
    // 比较操作符
    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    
    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};





