#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include <vector>

namespace Algo
{
    // 多边形边界绘制（使用 Bresenham 算法连接顶点）
    void DrawPolygon(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color);
}
