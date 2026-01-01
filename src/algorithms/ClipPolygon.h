#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include "../algorithms/ClipLine.h"
#include <vector>

namespace Algo
{
    // Sutherland-Hodgman 多边形裁剪算法
    // 返回裁剪后的多边形顶点
    std::vector<std::pair<int, int>> ClipPolygonSutherlandHodgman(
        const std::vector<int>& xPoints,
        const std::vector<int>& yPoints,
        const ClipRect& rect);

    // 裁剪并绘制多边形
    void DrawClippedPolygon(Canvas& canvas, 
        const std::vector<int>& xPoints, 
        const std::vector<int>& yPoints,
        const ClipRect& rect, 
        Color color);
}
