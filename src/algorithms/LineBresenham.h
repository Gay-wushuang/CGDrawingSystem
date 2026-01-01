#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"

namespace Algo
{
    // Bresenham 直线算法（任意斜率）
    // 使用整数运算，效率高，支持所有象限
    void DrawLineBresenham(Canvas& canvas, int x0, int y0, int x1, int y1, Color color);
}
