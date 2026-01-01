#pragma once
#include "Shape.h"

// 直线图形类
class ShapeLine : public Shape
{
public:
    enum class Algorithm
    {
        DDA,
        Bresenham,
        System  // 系统函数（GDI LineTo）
    };

    ShapeLine(int x0, int y0, int x1, int y1, Algorithm algo, Color color = Color::Black())
        : m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1), m_algorithm(algo)
    {
        m_color = color;
    }

    void Draw(Canvas& canvas) const override;
    void Transform(const Matrix3& mat) override;
    bool HitTest(int x, int y, int threshold = 5) const override;
    ShapeType GetType() const override { return ShapeType::Line; }

    // 获取起点和终点（用于裁剪）
    void GetPoints(int& x0, int& y0, int& x1, int& y1) const
    {
        x0 = m_x0; y0 = m_y0;
        x1 = m_x1; y1 = m_y1;
    }

private:
    int m_x0, m_y0, m_x1, m_y1;
    Algorithm m_algorithm;

    // 点到线段距离的平方
    double PointToLineDistanceSq(int px, int py, int x0, int y0, int x1, int y1) const;
};




