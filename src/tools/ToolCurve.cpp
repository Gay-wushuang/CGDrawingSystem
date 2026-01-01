#include "ToolCurve.h"
#include "../app/App.h"
#include "../model/ShapeCurve.h"
#include "../core/Color.h"
#include "../core/Canvas.h"
#include "../algorithms/LineBresenham.h"
#include <memory>

void ToolCurve::OnLButtonDown(POINT pt)
{
    if (m_state == State::Idle)
    {
        m_state = State::Drawing;
        m_controlPoints.clear();
    }
    
    // 添加控制点
    m_controlPoints.push_back(pt);
    InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
}

void ToolCurve::OnRButtonDown(POINT pt)
{
    if (m_state == State::Drawing && m_controlPoints.size() >= 2)
    {
        // 完成曲线绘制
        std::vector<int> xPoints, yPoints;
        for (const auto& pt : m_controlPoints)
        {
            xPoints.push_back(pt.x);
            yPoints.push_back(pt.y);
        }

        CurveType curveType = (m_curveType == CurveToolType::Bezier) ? CurveType::Bezier : CurveType::BSpline;
        auto shape = std::make_shared<ShapeCurve>(curveType, xPoints, yPoints, Color::Black());
        App::Get().AddShape(shape);

        // 重置状态
        m_state = State::Idle;
        m_controlPoints.clear();
    }
}

void ToolCurve::OnMouseMove(POINT pt)
{
    // 可以添加实时预览
    UNREFERENCED_PARAMETER(pt);
}

void ToolCurve::OnEscape()
{
    m_state = State::Idle;
    m_controlPoints.clear();
    InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
}

void ToolCurve::DrawPreview(Canvas& canvas) const
{
    if (m_state != State::Drawing || m_controlPoints.size() < 2)
        return;

    Color previewColor = Color(128, 128, 128);

    // 绘制控制点连线
    for (size_t i = 0; i < m_controlPoints.size() - 1; ++i)
    {
        Algo::DrawLineBresenham(canvas,
            m_controlPoints[i].x, m_controlPoints[i].y,
            m_controlPoints[i + 1].x, m_controlPoints[i + 1].y,
            previewColor);
    }

    // 绘制控制点
    for (const auto& pt : m_controlPoints)
    {
        // 画一个小十字标记
        for (int i = -2; i <= 2; ++i)
        {
            canvas.PutPixel(pt.x + i, pt.y, previewColor);
            canvas.PutPixel(pt.x, pt.y + i, previewColor);
        }
    }
}
