#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"

namespace Algo
{
    // Bresenham 圆算法
    // 圆心可以是任意位置（包括第二象限）
    void DrawCircleBresenham(Canvas& canvas, int cx, int cy, int radius, Color color);
}
