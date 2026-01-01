#pragma once
#include "ToolBase.h"
#include "../app/App.h"
#include <vector>

// 自由曲线绘制工具
enum class CurveToolType
{
    Bezier,
    BSpline
};

class ToolCurve : public ToolBase
{
public:
    ToolCurve() : m_curveType(CurveToolType::Bezier), m_state(State::Idle) {}

    void SetCurveType(CurveToolType type) { m_curveType = type; }

    void OnLButtonDown(POINT pt) override;
    void OnRButtonDown(POINT pt) override;
    void OnMouseMove(POINT pt) override;
    void OnEscape();

    // 绘制预览
    void DrawPreview(class Canvas& canvas) const;
    bool IsDrawing() const { return m_state == State::Drawing; }

private:
    enum class State
    {
        Idle,
        Drawing  // 正在添加控制点
    };

    State m_state;
    CurveToolType m_curveType;
    std::vector<POINT> m_controlPoints;
};
