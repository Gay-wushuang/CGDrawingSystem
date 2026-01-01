#pragma once
#include "ToolBase.h"
#include "../app/App.h"
#include <vector>

// 基本图形绘制交互工具
class ToolDrawBasic : public ToolBase
{
public:
    ToolDrawBasic() : m_state(State::Idle), m_basicTool(BasicDrawTool::None) {}

    void SetTool(BasicDrawTool tool) { m_basicTool = tool; }

    void OnLButtonDown(POINT pt) override;
    void OnLButtonUp(POINT pt) override;
    void OnMouseMove(POINT pt) override;
    void OnRButtonDown(POINT pt) override;
    void OnEscape();

    // 绘制当前预览（在 WM_PAINT 中调用）
    void DrawPreview(class Canvas& canvas) const;

    // 获取当前状态（用于判断是否需要绘制预览）
    bool IsDrawing() const { return m_state == State::Drawing || m_state == State::PolygonDrawing; }

private:
    enum class State
    {
        Idle,           // 空闲状态
        Drawing,        // 正在绘制（已点起点）
        PolygonDrawing  // 多边形绘制中（已点多个顶点）
    };

    State m_state;
    BasicDrawTool m_basicTool;

    // 当前绘制数据
    POINT m_startPoint = {};
    POINT m_currentPoint = {};
    std::vector<POINT> m_polygonPoints;  // 多边形顶点列表

    // 完成当前图形绘制
    void FinishShape();
    void CancelShape();
};
