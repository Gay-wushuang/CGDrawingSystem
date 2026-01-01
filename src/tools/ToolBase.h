#pragma once
#include <windows.h>

// 所有交互工具的基类，后续可加入鼠标、键盘处理接口
class ToolBase
{
public:
    virtual ~ToolBase() = default;

    virtual void OnLButtonDown(POINT) {}
    virtual void OnLButtonUp(POINT)   {}
    virtual void OnMouseMove(POINT)   {}
    virtual void OnRButtonDown(POINT) {}
};




