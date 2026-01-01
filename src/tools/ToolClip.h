#pragma once
#include "ToolBase.h"
#include "../app/App.h"
#include "../algorithms/ClipLine.h"
#include <vector>

// 裁剪工具
enum class ClipToolType
{
    Line,
    Polygon
};

class ToolClip : public ToolBase
{
public:
    ToolClip() : m_clipType(ClipToolType::Line), m_state(State::Idle), m_clipRect(0, 0, 0, 0) {}

    void SetClipType(ClipToolType type) { m_clipType = type; }

    void OnLButtonDown(POINT pt) override;
    void OnLButtonUp(POINT pt) override;
    void OnMouseMove(POINT pt) override;
    void OnRButtonDown(POINT pt) override;

    // 绘制预览
    void DrawPreview(class Canvas& canvas) const;
    bool IsDrawing() const { return m_state != State::Idle; }

private:
    enum class State
    {
        Idle,
        SelectingClipRect,  // 正在选择裁剪矩形
        SelectingShape      // 正在选择要裁剪的图形
    };

    State m_state;
    ClipToolType m_clipType;
    POINT m_rectStart, m_rectEnd;  // 裁剪矩形
    Algo::ClipRect m_clipRect;
    bool m_hasClipRect = false;
};
