#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include "../core/Math2D.h"
#include <vector>

namespace Algo
{
    // 绘制倾斜椭圆（通过参数方程生成点）
    void DrawSkewedEllipse(Canvas& canvas, int cx, int cy, int a, int b, double rotation, Color color);
    
    // 通过变换矩阵绘制倾斜椭圆
    void DrawTransformedEllipse(Canvas& canvas, int cx, int cy, int a, int b, const Matrix3& transform, Color color);
}