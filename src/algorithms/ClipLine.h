#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"

namespace Algo
{
    // Cohen-Sutherland 编码
    enum class OutCode
    {
        INSIDE = 0,
        LEFT = 1,
        RIGHT = 2,
        BOTTOM = 4,
        TOP = 8
    };

    // 重载 |= 运算符
    inline OutCode& operator|=(OutCode& lhs, OutCode rhs)
    {
        lhs = static_cast<OutCode>(static_cast<int>(lhs) | static_cast<int>(rhs));
        return lhs;
    }

    // 重载 | 运算符
    inline OutCode operator|(OutCode lhs, OutCode rhs)
    {
        return static_cast<OutCode>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    // 重载 & 运算符
    inline OutCode operator&(OutCode lhs, OutCode rhs)
    {
        return static_cast<OutCode>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

    // 裁剪矩形结构
    struct ClipRect
    {
        int xmin, ymin, xmax, ymax;
        
        ClipRect(int x1, int y1, int x2, int y2)
        {
            xmin = (x1 < x2) ? x1 : x2;
            xmax = (x1 > x2) ? x1 : x2;
            ymin = (y1 < y2) ? y1 : y2;
            ymax = (y1 > y2) ? y1 : y2;
        }
    };

    // Cohen-Sutherland 直线裁剪算法
    // 返回 true 表示线段在裁剪窗口内或部分在窗口内，false 表示完全在窗口外
    bool ClipLineCohenSutherland(int& x0, int& y0, int& x1, int& y1, const ClipRect& rect);

    // 裁剪并绘制直线
    void DrawClippedLine(Canvas& canvas, int x0, int y0, int x1, int y1, const ClipRect& rect, Color color);
}
