#include "ShapeCircle.h"
#include "../algorithms/CircleBresenham.h"
#include "../algorithms/EllipseBresenham.h"
#include "../algorithms/SkewedEllipse.h"
#include "../core/Math2D.h"
#include "../app/App.h"
#include <cmath>

void ShapeCircle::Draw(Canvas& canvas) const
{
    if (m_isEllipse) {
        // 如果已变换为椭圆，使用变换矩阵绘制倾斜椭圆
        Algo::DrawTransformedEllipse(canvas, m_cx, m_cy, m_a, m_b, m_transform, m_color);
    } else {
        // 否则绘制圆形
        Algo::DrawCircleBresenham(canvas, m_cx, m_cy, m_radius, m_color);
    }
}

void ShapeCircle::Transform(const Matrix3& mat)
{
    App& app = App::Get();
    CoordinateConverter& converter = app.GetCoordinateConverter();
    
    // 将屏幕坐标转换为世界坐标
    Vec2 worldCenter;
    converter.ScreenToWorld(m_cx, m_cy, worldCenter);
    
    // 应用变换矩阵 - 这会改变圆心位置
    worldCenter = mat.Transform(worldCenter);
    
    // 将世界坐标转换回屏幕坐标
    int screenCX, screenCY;
    converter.WorldToScreen(worldCenter, screenCX, screenCY);
    m_cx = screenCX;  // 确保圆心位置更新
    m_cy = screenCY;  // 确保圆心位置更新
    
    // 累积变换矩阵
    m_transform = mat * m_transform;
    
    // 检查变换矩阵是否包含剪切操作
    bool hasShear = (abs(mat.m[0][1]) > 1e-6 || abs(mat.m[1][0]) > 1e-6);
    
    if (hasShear) {
        // 如果是剪切变换，圆会变成椭圆，需要计算椭圆参数
        // 变换圆上的关键点来确定椭圆形状
        Vec2 rightPoint = mat.Transform(Vec2(worldCenter.x + m_radius, worldCenter.y));
        Vec2 downPoint = mat.Transform(Vec2(worldCenter.x, worldCenter.y + m_radius));
        Vec2 leftPoint = mat.Transform(Vec2(worldCenter.x - m_radius, worldCenter.y));
        Vec2 upPoint = mat.Transform(Vec2(worldCenter.x, worldCenter.y - m_radius));
        
        // 计算变换后椭圆的半轴长度
        double new_a = sqrt(pow(rightPoint.x - worldCenter.x, 2) + pow(rightPoint.y - worldCenter.y, 2));
        double new_b = sqrt(pow(downPoint.x - worldCenter.x, 2) + pow(downPoint.y - worldCenter.y, 2));
        
        // 将圆标记为椭圆并保存椭圆参数
        m_isEllipse = true;
        m_a = (int)round(new_a);
        m_b = (int)round(new_b);
    } else {
        // 非剪切变换（平移、旋转、缩放）- 保持为圆形
        Vec2 rightPoint = mat.Transform(Vec2(worldCenter.x + m_radius, worldCenter.y));
        Vec2 centerRight = mat.Transform(Vec2(worldCenter.x, worldCenter.y));
        
        // 计算变换后的半径
        double newRadius = sqrt(pow(rightPoint.x - centerRight.x, 2) + pow(rightPoint.y - centerRight.y, 2));
        m_radius = (int)round(newRadius);
        m_isEllipse = false;  // 保持圆形
    }
}

bool ShapeCircle::HitTest(int x, int y, int threshold) const
{
    int dx = x - m_cx;
    int dy = y - m_cy;
    double dist = sqrt(dx * dx + dy * dy);
    return abs(dist - m_radius) <= threshold;
}
