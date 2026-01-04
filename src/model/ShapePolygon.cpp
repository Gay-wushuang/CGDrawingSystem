#include "ShapePolygon.h"
#include "../algorithms/PolygonDraw.h"
#include "../core/Math2D.h"
#include "../algorithms/LineBresenham.h"
#include "../app/App.h"
#include <cmath>
#include <algorithm>

void ShapePolygon::Draw(Canvas& canvas) const
{
    Algo::DrawPolygon(canvas, m_xPoints, m_yPoints, m_color);
}

void ShapePolygon::Transform(const Matrix3& mat)
{
    App& app = App::Get();
    CoordinateConverter& converter = app.GetCoordinateConverter();
    
    for (size_t i = 0; i < m_xPoints.size(); ++i)
    {
        // 将屏幕坐标转换为世界坐标
        Vec2 worldPt;
        converter.ScreenToWorld(m_xPoints[i], m_yPoints[i], worldPt);
        
        // 应用变换矩阵
        worldPt = mat.Transform(worldPt);
        
        // 将世界坐标转换回屏幕坐标
        int screenX, screenY;
        converter.WorldToScreen(worldPt, screenX, screenY);
        m_xPoints[i] = screenX;
        m_yPoints[i] = screenY;
    }
}

void ShapePolygon::GetCenter(int& cx, int& cy) const
{
    if (m_xPoints.empty() || m_yPoints.empty())
    {
        cx = 0;
        cy = 0;
        return;
    }
    
    int xmin = *std::min_element(m_xPoints.begin(), m_xPoints.end());
    int xmax = *std::max_element(m_xPoints.begin(), m_xPoints.end());
    int ymin = *std::min_element(m_yPoints.begin(), m_yPoints.end());
    int ymax = *std::max_element(m_yPoints.begin(), m_yPoints.end());
    
    cx = (xmin + xmax) / 2;
    cy = (ymin + ymax) / 2;
}

bool ShapePolygon::HitTest(int x, int y, int threshold) const
{
    if (m_xPoints.size() < 2)
        return false;
    
    // 检查点到各边的距离
    for (size_t i = 0; i < m_xPoints.size(); ++i)
    {
        size_t next = (i + 1) % m_xPoints.size();
        int x0 = m_xPoints[i];
        int y0 = m_yPoints[i];
        int x1 = m_xPoints[next];
        int y1 = m_yPoints[next];
        
        Vec2 lineVec(x1 - x0, y1 - y0);
        Vec2 pointVec(x - x0, y - y0);
        
        double lineLenSq = lineVec.LengthSq();
        if (lineLenSq < 1e-6)
            continue;
        
        double t = (std::max)(0.0, (std::min)(1.0, (pointVec.x * lineVec.x + pointVec.y * lineVec.y) / lineLenSq));
        Vec2 closest(x0 + t * lineVec.x, y0 + t * lineVec.y);
        Vec2 diff(x - closest.x, y - closest.y);
        
        if (diff.LengthSq() <= threshold * threshold)
            return true;
    }
    
    return false;
}
