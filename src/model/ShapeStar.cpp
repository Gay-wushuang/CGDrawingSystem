#include "ShapeStar.h"
#include <cmath>

ShapeStar::ShapeStar(int cx, int cy, int outerRadius, int innerRadius, Color color)
    : ShapePolygon({}, {}, color)  // 先初始化基类，稍后填充顶点
{
    
    const int numPoints = 10;  // 五角星有10个顶点（5个外顶点 + 5个内顶点）
    const double angleStep = 2.0 * 3.14159265358979323846 / numPoints;
    const double startAngle = -3.14159265358979323846 / 2.0;  // 从顶部开始
    
    m_xPoints.reserve(numPoints);
    m_yPoints.reserve(numPoints);
    
    for (int i = 0; i < numPoints; ++i)
    {
        double angle = startAngle + i * angleStep;
        int radius = (i % 2 == 0) ? outerRadius : innerRadius;
        
        int x = cx + (int)round(radius * cos(angle));
        int y = cy + (int)round(radius * sin(angle));
        
        m_xPoints.push_back(x);
        m_yPoints.push_back(y);
    }
}


