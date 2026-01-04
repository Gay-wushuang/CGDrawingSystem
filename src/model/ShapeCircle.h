#pragma once
#include "Shape.h"
#include "../core/Math2D.h"  // 为了使用Matrix3

// 圆形/椭圆图形类
class ShapeCircle : public Shape
{
public:
    ShapeCircle(int cx, int cy, int radius, Color color = Color::Black())
        : m_cx(cx), m_cy(cy), m_radius(radius), m_isEllipse(false), m_transform(Matrix3::Identity())
    {
        m_color = color;
    }
    
    void Draw(Canvas& canvas) const override;
    void Transform(const Matrix3& mat) override;
    bool HitTest(int x, int y, int threshold = 5) const override;
    ShapeType GetType() const override { return ShapeType::Circle; }
    void GetCenter(int& cx, int& cy) const override { cx = m_cx; cy = m_cy; }

private:
    int m_cx, m_cy;  // 圆心坐标
    int m_radius;     // 半径
    bool m_isEllipse; // 是否已变换为椭圆 (新增)
    int m_a, m_b;    // 椭圆半轴（当m_isEllipse为true时使用）(新增)
    Matrix3 m_transform;  // 累积变换矩阵，用于处理倾斜椭圆
};




