#pragma once
#include "Shape.h"
#include "../core/Math2D.h"  // 为了使用Matrix3

// 椭圆图形类
class ShapeEllipse : public Shape
{
public:
    ShapeEllipse(int cx, int cy, int a, int b, Color color = Color::Black())
        : m_cx(cx), m_cy(cy), m_a(a), m_b(b), m_transform(Matrix3::Identity())
    {
        m_color = color;
    }

    void Draw(Canvas& canvas) const override;
    void Transform(const Matrix3& mat) override;
    bool HitTest(int x, int y, int threshold = 5) const override;
    ShapeType GetType() const override { return ShapeType::Ellipse; }

private:
    int m_cx, m_cy;  // 椭圆中心坐标
    int m_a, m_b;    // 椭圆半轴长度
    Matrix3 m_transform;  // 累积变换矩阵，用于处理倾斜椭圆
};




