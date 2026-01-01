#include "ToolClip.h"
#include "../app/App.h"
#include "../core/Canvas.h"
#include "../core/Color.h"
#include "../algorithms/ClipLine.h"
#include "../algorithms/ClipPolygon.h"
#include "../algorithms/LineBresenham.h"
#include "../model/Shape.h"
#include "../model/ShapeLine.h"
#include "../model/ShapePolygon.h"
#include <memory>

void ToolClip::OnLButtonDown(POINT pt)
{
    if (m_state == State::Idle)
    {
        // 开始选择裁剪矩形
        m_state = State::SelectingClipRect;
        m_rectStart = pt;
        m_rectEnd = pt;
        m_hasClipRect = false;
    }
    else if (m_state == State::SelectingClipRect)
    {
        // 完成裁剪矩形选择
        m_rectEnd = pt;
        m_clipRect = Algo::ClipRect(m_rectStart.x, m_rectStart.y, m_rectEnd.x, m_rectEnd.y);
        m_hasClipRect = true;
        m_state = State::SelectingShape;
        InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
    }
    else if (m_state == State::SelectingShape && m_hasClipRect)
    {
        // 选择要裁剪的图形并执行裁剪
        App& app = App::Get();
        app.SelectShape(pt.x, pt.y);
        
        auto shape = app.GetSelectedShape();
        if (shape && m_hasClipRect)
        {
            // 执行裁剪：创建裁剪后的新图形
            std::shared_ptr<Shape> clippedShape;
            
            // 根据设置的裁剪类型来执行裁剪
            if (m_clipType == ClipToolType::Line)
            {
                // 只处理直线裁剪
                if (auto line = std::dynamic_pointer_cast<ShapeLine>(shape))
                {
                    // 直线裁剪
                    int x0, y0, x1, y1;
                    line->GetPoints(x0, y0, x1, y1);
                    
                    int cx0 = x0, cy0 = y0, cx1 = x1, cy1 = y1;
                    if (Algo::ClipLineCohenSutherland(cx0, cy0, cx1, cy1, m_clipRect))
                    {
                        // 创建裁剪后的新直线
                        clippedShape = std::make_shared<ShapeLine>(cx0, cy0, cx1, cy1, 
                            ShapeLine::Algorithm::Bresenham, Color::Green());
                        app.AddShape(clippedShape);
                    }
                }
                else
                {
                    // 如果当前图形不是直线但选择了直线裁剪，可以提示用户
                    MessageBoxW(app.GetMainWindow(), 
                        L"当前选择的图形不是直线，无法执行直线裁剪", 
                        L"裁剪类型不匹配", MB_OK | MB_ICONWARNING);
                    return; // 不重置状态，允许用户重新选择图形
                }
            }
            else if (m_clipType == ClipToolType::Polygon)
            {
                // 只处理多边形裁剪
                if (auto poly = std::dynamic_pointer_cast<ShapePolygon>(shape))
                {
                    // 多边形裁剪
                    std::vector<int> xPoints, yPoints;
                    poly->GetPoints(xPoints, yPoints);
                    
                    auto clipped = Algo::ClipPolygonSutherlandHodgman(xPoints, yPoints, m_clipRect);
                    if (clipped.size() >= 3)
                    {
                        std::vector<int> cx, cy;
                        for (const auto& p : clipped)
                        {
                            cx.push_back(p.first);
                            cy.push_back(p.second);
                        }
                        clippedShape = std::make_shared<ShapePolygon>(cx, cy, Color::Green());
                        app.AddShape(clippedShape);
                    }
                }
                else
                {
                    // 如果当前图形不是多边形但选择了多边形裁剪，可以提示用户
                    MessageBoxW(app.GetMainWindow(), 
                        L"当前选择的图形不是多边形，无法执行多边形裁剪", 
                        L"裁剪类型不匹配", MB_OK | MB_ICONWARNING);
                    return; // 不重置状态，允许用户重新选择图形
                }
            }
            
            // 重置状态
            m_state = State::Idle;
            m_hasClipRect = false;
            InvalidateRect(app.GetMainWindow(), nullptr, FALSE);
        }
    }
}

void ToolClip::OnLButtonUp(POINT pt)
{
    if (m_state == State::SelectingClipRect)
    {
        m_rectEnd = pt;
        m_clipRect = Algo::ClipRect(m_rectStart.x, m_rectStart.y, m_rectEnd.x, m_rectEnd.y);
        m_hasClipRect = true;
        m_state = State::SelectingShape;
        InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
    }
}

void ToolClip::OnMouseMove(POINT pt)
{
    if (m_state == State::SelectingClipRect)
    {
        m_rectEnd = pt;
        InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
    }
}

void ToolClip::OnRButtonDown(POINT pt)
{
    // 右键重置
    m_state = State::Idle;
    m_hasClipRect = false;
    InvalidateRect(App::Get().GetMainWindow(), nullptr, FALSE);
}

void ToolClip::DrawPreview(Canvas& canvas) const
{
    if (m_state == State::SelectingClipRect)
    {
        // 绘制裁剪矩形预览
        Color previewColor = Color(128, 128, 255);
        Algo::DrawLineBresenham(canvas, m_rectStart.x, m_rectStart.y, m_rectEnd.x, m_rectStart.y, previewColor);
        Algo::DrawLineBresenham(canvas, m_rectEnd.x, m_rectStart.y, m_rectEnd.x, m_rectEnd.y, previewColor);
        Algo::DrawLineBresenham(canvas, m_rectEnd.x, m_rectEnd.y, m_rectStart.x, m_rectEnd.y, previewColor);
        Algo::DrawLineBresenham(canvas, m_rectStart.x, m_rectEnd.y, m_rectStart.x, m_rectStart.y, previewColor);
    }
    else if (m_state == State::SelectingShape && m_hasClipRect)
    {
        // 绘制裁剪矩形
        Color rectColor = Color(0, 0, 255);
        Algo::DrawLineBresenham(canvas, m_clipRect.xmin, m_clipRect.ymin, m_clipRect.xmax, m_clipRect.ymin, rectColor);
        Algo::DrawLineBresenham(canvas, m_clipRect.xmax, m_clipRect.ymin, m_clipRect.xmax, m_clipRect.ymax, rectColor);
        Algo::DrawLineBresenham(canvas, m_clipRect.xmax, m_clipRect.ymax, m_clipRect.xmin, m_clipRect.ymax, rectColor);
        Algo::DrawLineBresenham(canvas, m_clipRect.xmin, m_clipRect.ymax, m_clipRect.xmin, m_clipRect.ymin, rectColor);
    }
}
