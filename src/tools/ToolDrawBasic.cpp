#include "ToolDrawBasic.h"
#include "../app/App.h"
#include "../model/ShapeLine.h"
#include "../model/ShapeCircle.h"
#include "../model/ShapeEllipse.h"
#include "../model/ShapePolygon.h"
#include "../core/Color.h"
#include "../core/Canvas.h"
#include "../algorithms/LineBresenham.h"
#include "../algorithms/CircleBresenham.h"
#include "../algorithms/EllipseBresenham.h"
#include <memory>
#include <algorithm>
#include <cmath>

void ToolDrawBasic::OnLButtonDown(POINT pt)
{
    if (m_basicTool == BasicDrawTool::None)
        return;

    if (m_basicTool == BasicDrawTool::Polygon)
    {
        // 多边形：左键添加顶点
        if (m_state == State::Idle)
        {
            m_state = State::PolygonDrawing;
            m_polygonPoints.clear();
        }
        m_polygonPoints.push_back(pt);
        App::Get().GetMainWindow();  // 触发重绘
        InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
    }
    else
    {
        // 其他图形：左键点起点
        if (m_state == State::Idle)
        {
            m_startPoint = pt;
            m_currentPoint = pt;
            m_state = State::Drawing;
        }
    }
}

void ToolDrawBasic::OnLButtonUp(POINT pt)
{
    if (m_state != State::Drawing)
        return;

    if (m_basicTool == BasicDrawTool::Polygon)
        return;  // 多边形用右键结束

    // 左键释放：完成图形
    m_currentPoint = pt;
    FinishShape();
}

void ToolDrawBasic::OnMouseMove(POINT pt)
{
    if (m_state == State::Drawing)
    {
        m_currentPoint = pt;
        // 实时预览：触发重绘（在 WndProc 中处理）
        InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
    }
}

void ToolDrawBasic::OnRButtonDown(POINT pt)
{
    if (m_basicTool == BasicDrawTool::Polygon)
    {
        // 多边形：右键结束并闭合
        if (m_state == State::PolygonDrawing && m_polygonPoints.size() >= 2)
        {
            FinishShape();
        }
    }
    else
    {
        // 其他图形：右键取消
        CancelShape();
    }
}

void ToolDrawBasic::OnEscape()
{
    CancelShape();
}

void ToolDrawBasic::FinishShape()
{
    App& app = App::Get();
    std::shared_ptr<Shape> shape;

    switch (m_basicTool)
    {
    case BasicDrawTool::DDA_Line:
        shape = std::make_shared<ShapeLine>(
            m_startPoint.x, m_startPoint.y,
            m_currentPoint.x, m_currentPoint.y,
            ShapeLine::Algorithm::DDA,
            Color::Black());
        break;

    case BasicDrawTool::Bresenham_Line:
        shape = std::make_shared<ShapeLine>(
            m_startPoint.x, m_startPoint.y,
            m_currentPoint.x, m_currentPoint.y,
            ShapeLine::Algorithm::Bresenham,
            Color::Black());
        break;

    case BasicDrawTool::System_Line:
        shape = std::make_shared<ShapeLine>(
            m_startPoint.x, m_startPoint.y,
            m_currentPoint.x, m_currentPoint.y,
            ShapeLine::Algorithm::System,
            Color::Red());  // 系统线用红色区分
        break;

    case BasicDrawTool::Circle:
    {
        int dx = m_currentPoint.x - m_startPoint.x;
        int dy = m_currentPoint.y - m_startPoint.y;
        int radius = (int)sqrt(dx * dx + dy * dy);
        shape = std::make_shared<ShapeCircle>(
            m_startPoint.x, m_startPoint.y, radius, Color::Black());
        break;
    }

    case BasicDrawTool::Ellipse:
    {
        int dx = abs(m_currentPoint.x - m_startPoint.x);
        int dy = abs(m_currentPoint.y - m_startPoint.y);
        shape = std::make_shared<ShapeEllipse>(
            m_startPoint.x, m_startPoint.y, dx, dy, Color::Black());
        break;
    }

    case BasicDrawTool::Rect:
    {
        // 矩形：用多边形实现（4个顶点）
        std::vector<int> xPoints = {
            m_startPoint.x, m_currentPoint.x, m_currentPoint.x, m_startPoint.x
        };
        std::vector<int> yPoints = {
            m_startPoint.y, m_startPoint.y, m_currentPoint.y, m_currentPoint.y
        };
        shape = std::make_shared<ShapePolygon>(xPoints, yPoints, Color::Black());
        break;
    }

    case BasicDrawTool::Polygon:
    {
        if (m_polygonPoints.size() >= 2)
        {
            std::vector<int> xPoints, yPoints;
            for (const auto& pt : m_polygonPoints)
            {
                xPoints.push_back(pt.x);
                yPoints.push_back(pt.y);
            }
            shape = std::make_shared<ShapePolygon>(xPoints, yPoints, Color::Black());
        }
        break;
    }

    default:
        break;
    }

    if (shape)
    {
        app.AddShape(shape);
    }

    // 重置状态
    m_state = State::Idle;
    m_polygonPoints.clear();
}

void ToolDrawBasic::CancelShape()
{
    m_state = State::Idle;
    m_polygonPoints.clear();
    InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
}

void ToolDrawBasic::DrawPreview(Canvas& canvas) const
{
    if (m_state == State::Idle)
        return;

    Color previewColor = Color(128, 128, 128);  // 灰色预览

    if (m_basicTool == BasicDrawTool::Polygon && m_state == State::PolygonDrawing)
    {
        // 多边形预览：绘制已有点之间的连线
        if (m_polygonPoints.size() >= 2)
        {
            for (size_t i = 0; i < m_polygonPoints.size() - 1; ++i)
            {
                Algo::DrawLineBresenham(canvas,
                    m_polygonPoints[i].x, m_polygonPoints[i].y,
                    m_polygonPoints[i + 1].x, m_polygonPoints[i + 1].y,
                    previewColor);
            }
        }
    }
    else if (m_state == State::Drawing)
    {
        // 其他图形的预览
        switch (m_basicTool)
        {
        case BasicDrawTool::DDA_Line:
        case BasicDrawTool::Bresenham_Line:
        case BasicDrawTool::System_Line:
            Algo::DrawLineBresenham(canvas,
                m_startPoint.x, m_startPoint.y,
                m_currentPoint.x, m_currentPoint.y,
                previewColor);
            break;

        case BasicDrawTool::Circle:
        {
            int dx = m_currentPoint.x - m_startPoint.x;
            int dy = m_currentPoint.y - m_startPoint.y;
            int radius = (int)sqrt(dx * dx + dy * dy);
            Algo::DrawCircleBresenham(canvas, m_startPoint.x, m_startPoint.y, radius, previewColor);
            break;
        }

        case BasicDrawTool::Ellipse:
        {
            int dx = abs(m_currentPoint.x - m_startPoint.x);
            int dy = abs(m_currentPoint.y - m_startPoint.y);
            Algo::DrawEllipseBresenham(canvas, m_startPoint.x, m_startPoint.y, dx, dy, previewColor);
            break;
        }

        case BasicDrawTool::Rect:
        {
            // 矩形预览：用四条边
            Algo::DrawLineBresenham(canvas, m_startPoint.x, m_startPoint.y, m_currentPoint.x, m_startPoint.y, previewColor);
            Algo::DrawLineBresenham(canvas, m_currentPoint.x, m_startPoint.y, m_currentPoint.x, m_currentPoint.y, previewColor);
            Algo::DrawLineBresenham(canvas, m_currentPoint.x, m_currentPoint.y, m_startPoint.x, m_currentPoint.y, previewColor);
            Algo::DrawLineBresenham(canvas, m_startPoint.x, m_currentPoint.y, m_startPoint.x, m_startPoint.y, previewColor);
            break;
        }

        default:
            break;
        }
    }
}
