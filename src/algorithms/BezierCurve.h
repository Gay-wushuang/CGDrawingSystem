#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include <vector>

namespace Algo
{
    // Bezier 曲线算法（支持一次、二次、三次）
    // 控制点数量决定阶数：2个点=一次，3个点=二次，4个点=三次
    void DrawBezierCurve(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color);
}


