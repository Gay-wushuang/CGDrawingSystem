#include "App.h"
#include "../core/Canvas.h"
#include "../core/Color.h"

void App::AddShape(std::shared_ptr<Shape> shape)
{
    if (shape)
    {
        m_shapes.push_back(shape);
        // 添加图形后重绘窗口
        InvalidateRect(m_hMainWnd, nullptr, FALSE);
    }
}

void App::DrawAllShapes(Canvas& canvas) const
{
    for (const auto& shape : m_shapes)
    {
        if (shape)
        {
            shape->Draw(canvas);
        }
    }
}

void App::ClearScene()
{
    m_shapes.clear();
    m_selectedShape.reset();
    InvalidateRect(m_hMainWnd, nullptr, TRUE);
}

void App::ClearAllShapes()
{
    m_shapes.clear();
    m_selectedShape.reset();
    InvalidateRect(m_hMainWnd, nullptr, TRUE);
}

void App::SelectShape(int x, int y)
{
    // 从后往前查找（后画的图形在上层）
    m_selectedShape.reset();
    for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it)
    {
        if (*it && (*it)->HitTest(x, y))
        {
            m_selectedShape = *it;
            InvalidateRect(m_hMainWnd, nullptr, FALSE);
            break;
        }
    }
    if (!m_selectedShape)
    {
        InvalidateRect(m_hMainWnd, nullptr, FALSE);
    }
}

void App::SetFillParams(int seedX, int seedY, Color fillColor, Color boundaryColor) 
{
    m_fillSeedX = seedX;
    m_fillSeedY = seedY;
    m_fillColor = fillColor;
    m_boundaryColor = boundaryColor;
    // m_fillType 不再根据颜色自动修改，只由菜单命令决定
}
