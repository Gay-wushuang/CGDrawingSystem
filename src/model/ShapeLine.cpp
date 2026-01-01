#include "ShapeLine.h"
#include "../algorithms/LineDDA.h"
#include "../algorithms/LineBresenham.h"
#include "../core/Math2D.h"
#include "../app/App.h"
#define NOMINMAX  // 防止 Windows.h 定义 min/max 宏
#include <windows.h>
#include <algorithm>
#include <cmath>

void ShapeLine::Draw(Canvas& canvas) const
{
    switch (m_algorithm)
    {
    case Algorithm::DDA:
        Algo::DrawLineDDA(canvas, m_x0, m_y0, m_x1, m_y1, m_color);
        break;

    case Algorithm::Bresenham:
        Algo::DrawLineBresenham(canvas, m_x0, m_y0, m_x1, m_y1, m_color);
        break;

    case Algorithm::System:
    {
        // 系统函数画线：需要获取 Canvas 的内部 HDC
        // 注意：这里为了演示系统函数，我们仍然使用算法绘制，但可以设置线宽/线型/颜色
        // 实际项目中，如果需要真正的 GDI LineTo，需要 Canvas 提供 GetHDC() 接口
        // 这里先用 Bresenham 算法绘制，颜色使用 m_color
        Algo::DrawLineBresenham(canvas, m_x0, m_y0, m_x1, m_y1, m_color);
        break;
    }
    }
}

void ShapeLine::Transform(const Matrix3& mat)
{
    App& app = App::Get();
    CoordinateConverter& converter = app.GetCoordinateConverter();
    
    // 将屏幕坐标转换为世界坐标
    Vec2 worldP0, worldP1;
    converter.ScreenToWorld(m_x0, m_y0, worldP0);
    converter.ScreenToWorld(m_x1, m_y1, worldP1);
    
    // 应用变换矩阵
    worldP0 = mat.Transform(worldP0);
    worldP1 = mat.Transform(worldP1);
    
    // 将世界坐标转换回屏幕坐标
    int screenX0, screenY0, screenX1, screenY1;
    converter.WorldToScreen(worldP0, screenX0, screenY0);
    converter.WorldToScreen(worldP1, screenX1, screenY1);
    
    m_x0 = screenX0;
    m_y0 = screenY0;
    m_x1 = screenX1;
    m_y1 = screenY1;
}

bool ShapeLine::HitTest(int x, int y, int threshold) const
{
    double distSq = PointToLineDistanceSq(x, y, m_x0, m_y0, m_x1, m_y1);
    return distSq <= threshold * threshold;
}

double ShapeLine::PointToLineDistanceSq(int px, int py, int x0, int y0, int x1, int y1) const
{
    Vec2 lineVec(x1 - x0, y1 - y0);
    Vec2 pointVec(px - x0, py - y0);
    
    double lineLenSq = lineVec.LengthSq();
    if (lineLenSq < 1e-6)
    {
        // 线段退化为点
        return pointVec.LengthSq();
    }
    
    double t = (std::max)(0.0, (std::min)(1.0, (pointVec.x * lineVec.x + pointVec.y * lineVec.y) / lineLenSq));
    Vec2 closest(x0 + t * lineVec.x, y0 + t * lineVec.y);
    Vec2 diff(px - closest.x, py - closest.y);
    
    return diff.LengthSq();
}
