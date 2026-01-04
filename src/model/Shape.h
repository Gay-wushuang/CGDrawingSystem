#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include "../core/Math2D.h"

// 图形类型枚举
enum class ShapeType
{
    Line,
    Circle,
    Ellipse,
    Rect,
    Polygon,
    Curve
};

// 所有图形的抽象基类，后续用于变换、裁剪、填充等
class Shape
{
public:
    virtual ~Shape() = default;

    // 使用 Canvas 绘制图形（所有算法都通过 PutPixel 实现）
    virtual void Draw(Canvas& canvas) const = 0;

    // 图形变换：应用齐次坐标变换矩阵
    virtual void Transform(const Matrix3& mat) = 0;

    // 碰撞检测：判断点是否在图形附近（用于选中图形）
    virtual bool HitTest(int x, int y, int threshold = 5) const = 0;

    // 获取图形类型
    virtual ShapeType GetType() const = 0;

    // 返回屏幕坐标下的"图形中心"（用于默认缩放/旋转中心）
    virtual void GetCenter(int& cx, int& cy) const = 0;

protected:
    Color m_color = Color::Black();  // 图形颜色
};



