#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include <vector>

namespace Algo
{
    // 均匀周期 B 样条曲线
    void DrawBSplineCurve(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color);
}


