#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"

namespace Algo
{
    // Bresenham 椭圆算法
    void DrawEllipseBresenham(Canvas& canvas, int cx, int cy, int a, int b, Color color);
}
