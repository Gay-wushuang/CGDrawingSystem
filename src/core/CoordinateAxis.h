#pragma once
#include "Canvas.h"

// 坐标系绘制：在画布中心画 x/y 轴，并标注 O、x、y
// 使用 Canvas::PutPixel 绘制轴线（保证是"自己画"的）
class CoordinateAxis
{
public:
    void Draw(Canvas& canvas) const;
};


