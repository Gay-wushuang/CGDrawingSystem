#include "ShapeEllipse.h"
#include "../algorithms/EllipseBresenham.h"
#include "../algorithms/SkewedEllipse.h"
#include "../core/Math2D.h"
#include "../app/App.h"
#include <cmath>

void ShapeEllipse::Draw(Canvas& canvas) const
{
    // 使用变换矩阵绘制倾斜椭圆
    Algo::DrawTransformedEllipse(canvas, m_cx, m_cy, m_a, m_b, m_transform, m_color);
}

void ShapeEllipse::Transform(const Matrix3& mat)
{
    App& app = App::Get();
    CoordinateConverter& converter = app.GetCoordinateConverter();
    
    // 将屏幕坐标转换为世界坐标
    Vec2 worldCenter;
    converter.ScreenToWorld(m_cx, m_cy, worldCenter);
    
    // 应用变换矩阵 - 这会改变椭圆中心位置
    worldCenter = mat.Transform(worldCenter);
    
    // 将世界坐标转换回屏幕坐标
    int screenCX, screenCY;
    converter.WorldToScreen(worldCenter, screenCX, screenCY);
    m_cx = screenCX;  // 确保椭圆中心位置更新
    m_cy = screenCY;  // 确保椭圆中心位置更新
    
    // 累积变换矩阵
    m_transform = mat * m_transform;
    
    // 对于剪切变换，椭圆可能会变成更复杂的椭圆
    // 需要变换椭圆上的点来确定变换后的椭圆参数
    Vec2 rightPoint = mat.Transform(Vec2(worldCenter.x + m_a, worldCenter.y));
    Vec2 downPoint = mat.Transform(Vec2(worldCenter.x, worldCenter.y + m_b));
    Vec2 leftPoint = mat.Transform(Vec2(worldCenter.x - m_a, worldCenter.y));
    Vec2 upPoint = mat.Transform(Vec2(worldCenter.x, worldCenter.y - m_b));
    
    // 计算变换后椭圆的半轴长度
    double new_a = sqrt(pow(rightPoint.x - worldCenter.x, 2) + pow(rightPoint.y - worldCenter.y, 2));
    double new_b = sqrt(pow(downPoint.x - worldCenter.x, 2) + pow(downPoint.y - worldCenter.y, 2));
    
    m_a = (int)round(new_a);
    m_b = (int)round(new_b);
}

bool ShapeEllipse::HitTest(int x, int y, int threshold) const
{
    // 简化：检查点是否在椭圆附近
    int dx = x - m_cx;
    int dy = y - m_cy;
    double dist = sqrt(dx * dx + dy * dy);
    double avgRadius = (m_a + m_b) / 2.0;
    return abs(dist - avgRadius) <= threshold + avgRadius * 0.1;
}
