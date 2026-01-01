#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"

namespace Algo
{
    // DDA 直线算法（Digital Differential Analyzer）
    // 支持四象限直线绘制
    // 使用 Canvas::PutPixel 绘制像素点
    void DrawLineDDA(Canvas& canvas, int x0, int y0, int x1, int y1, Color color);
}
