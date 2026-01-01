#pragma once
#include "Shape.h"
#include <vector>

// 多边形图形类
class ShapePolygon : public Shape
{
public:
    ShapePolygon(const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color = Color::Black())
        : m_xPoints(xPoints), m_yPoints(yPoints)
    {
        m_color = color;
    }

    void Draw(Canvas& canvas) const override;
    void Transform(const Matrix3& mat) override;
    bool HitTest(int x, int y, int threshold = 5) const override;
    ShapeType GetType() const override { return ShapeType::Polygon; }

    // 获取顶点（用于裁剪）
    void GetPoints(std::vector<int>& xPoints, std::vector<int>& yPoints) const
    {
        xPoints = m_xPoints;
        yPoints = m_yPoints;
    }

protected:
    std::vector<int> m_xPoints;
    std::vector<int> m_yPoints;
};




