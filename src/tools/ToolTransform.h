#pragma once
#include "ToolBase.h"
#include "../app/App.h"
#include "../core/Math2D.h"

// 图形变换工具
enum class TransformType
{
    None,
    Translate,
    Scale,
    Shear,
    Reflect,
    Rotate,
    ScaleComposed,
    RotateComposed
};

class ToolTransform : public ToolBase
{
public:
    ToolTransform() : m_transformType(TransformType::None) {}

    void SetTransformType(TransformType type) { m_transformType = type; }

    void OnLButtonDown(POINT pt) override;

    // 执行变换（通过对话框输入参数）
    void ExecuteTransform();

    // 显示中心点输入对话框
    static bool ShowCenterPointDialog(Vec2& center, const wchar_t* title, const wchar_t* prompt);

private:
    TransformType m_transformType;
    
    // 静态成员变量
    static double s_centerX;
    static double s_centerY;
    static const wchar_t* s_dialogTitle;
    static const wchar_t* s_dialogPrompt;
    
    // 对话框处理函数
    static INT_PTR CALLBACK CenterPointProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
