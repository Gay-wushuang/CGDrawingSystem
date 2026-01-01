#pragma once
#include "Shape.h"
#include <vector>

// Bezier / B 样条曲线
enum class CurveType
{
    Bezier,
    BSpline
};

class ShapeCurve : public Shape
{
public:
    ShapeCurve(CurveType type, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color = Color::Black())
        : m_type(type), m_xPoints(xPoints), m_yPoints(yPoints)
    {
        m_color = color;
    }

    void Draw(Canvas& canvas) const override;
    void Transform(const Matrix3& mat) override;
    bool HitTest(int x, int y, int threshold = 5) const override;
    ShapeType GetType() const override { return ShapeType::Curve; }

private:
    CurveType m_type;
    std::vector<int> m_xPoints;
    std::vector<int> m_yPoints;
};




