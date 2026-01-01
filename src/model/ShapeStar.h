#pragma once
#include "ShapePolygon.h"

// 预置五角星图形（用于演示变换）
class ShapeStar : public ShapePolygon
{
public:
    ShapeStar(int cx, int cy, int outerRadius, int innerRadius, Color color = Color::Black());
};


