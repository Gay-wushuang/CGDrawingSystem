#include "ShapeCurve.h"
#include "../algorithms/BezierCurve.h"
#include "../algorithms/BSplineCurve.h"
#include "../core/Math2D.h"
#include "../app/App.h"
#include <cmath>
#include <algorithm>

void ShapeCurve::Draw(Canvas& canvas) const
{
    if (m_xPoints.size() < 2)
        return;

    switch (m_type)
    {
    case CurveType::Bezier:
        Algo::DrawBezierCurve(canvas, m_xPoints, m_yPoints, m_color);
        break;
    case CurveType::BSpline:
        Algo::DrawBSplineCurve(canvas, m_xPoints, m_yPoints, m_color);
        break;
    }
}

void ShapeCurve::Transform(const Matrix3& mat)
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

bool ShapeCurve::HitTest(int x, int y, int threshold) const
{
    // 简化：检查点到控制点的距离
    for (size_t i = 0; i < m_xPoints.size(); ++i)
    {
        int dx = x - m_xPoints[i];
        int dy = y - m_yPoints[i];
        if (dx * dx + dy * dy <= threshold * threshold)
            return true;
    }
    return false;
}
