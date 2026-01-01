#include "ToolTransform.h"
#include "../app/App.h"
#include "../model/Shape.h"
#include "../app/TransformDialog.h"
#include "../core/Math2D.h"
#include "../app/resource.h"
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cmath>

// 静态变量定义
double ToolTransform::s_centerX = 0.0;
double ToolTransform::s_centerY = 0.0;
const wchar_t* ToolTransform::s_dialogTitle = nullptr;
const wchar_t* ToolTransform::s_dialogPrompt = nullptr;

// 中心点对话框处理函数
INT_PTR CALLBACK ToolTransform::CenterPointProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // 设置提示文本（如果需要的话，可以添加一个静态文本控件）
            TCHAR buffer[32];
            _stprintf_s(buffer, TEXT("%.2f"), s_centerX);
            SetDlgItemText(hwndDlg, IDC_EDIT_CENTER_X, buffer);
            _stprintf_s(buffer, TEXT("%.2f"), s_centerY);
            SetDlgItemText(hwndDlg, IDC_EDIT_CENTER_Y, buffer);
            
            // 设置窗口标题
            SetWindowTextW(hwndDlg, s_dialogTitle);
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR buffer[32];
            GetDlgItemText(hwndDlg, IDC_EDIT_CENTER_X, buffer, 32);
            s_centerX = _tstof(buffer);
            GetDlgItemText(hwndDlg, IDC_EDIT_CENTER_Y, buffer, 32);
            s_centerY = _tstof(buffer);
            EndDialog(hwndDlg, IDOK);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ToolTransform::OnLButtonDown(POINT pt)
{
    App& app = App::Get();
    
    // 点击选中图形
    app.SelectShape(pt.x, pt.y);
    
    if (app.GetSelectedShape())
    {
        // 有选中图形，执行变换
        ExecuteTransform();
    }
}

bool ToolTransform::ShowCenterPointDialog(Vec2& center, const wchar_t* title, const wchar_t* prompt)
{
    // 保存参数
    s_centerX = center.x;
    s_centerY = center.y;
    s_dialogTitle = title;
    s_dialogPrompt = prompt;

    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_CENTER_POINT), 
        GetActiveWindow(), 
        CenterPointProc);

    if (result == IDOK)
    {
        center.x = static_cast<float>(s_centerX);
        center.y = static_cast<float>(s_centerY);
        return true;
    }
    return false;
}

void ToolTransform::ExecuteTransform()
{
    App& app = App::Get();
    auto shape = app.GetSelectedShape();
    if (!shape)
        return;

    Matrix3 mat;
    
    double value1 = 0.0, value2 = 0.0;
    
    switch (m_transformType)
    {
    case TransformType::Translate:
        if (TransformDialog::ShowTranslateDialog(value1, value2))
        {
            mat = Matrix3::Translate(value1, value2);
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;

    case TransformType::Scale:
        if (TransformDialog::ShowScaleDialog(value1, value2))
        {
            // 以世界坐标原点为中心进行缩放
            // 由于形状坐标存储在屏幕坐标系中，需要先转换到世界坐标系
            // 应用变换，然后再转换回屏幕坐标系
            mat = Matrix3::Scale(value1, value2, 0.0, 0.0);
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;

    case TransformType::Rotate:
        if (TransformDialog::ShowRotateDialog(value1))
        {
            double angleRad = value1 * 3.14159265358979323846 / 180.0;
            // 以世界坐标原点为中心进行旋转
            mat = Matrix3::Rotate(angleRad, 0.0, 0.0);
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;

    case TransformType::Shear:
        if (TransformDialog::ShowShearDialog(value1, value2))
        {
            // 以世界坐标原点为中心进行错切
            mat = Matrix3::Shear(value1, value2, 0.0, 0.0);
            
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;

    case TransformType::Reflect:
    {
        int reflectType = 0; // 0=X轴, 1=Y轴, 2=原点
        if (TransformDialog::ShowReflectDialog(reflectType))
        {
            switch (reflectType)
            {
            case 0:  // X轴对称
                mat = Matrix3::ReflectX();
                break;
            case 1:  // Y轴对称
                mat = Matrix3::ReflectY();
                break;
            case 2:  // 原点对称
                mat = Matrix3::ReflectOrigin();
                break;
            default:
                mat = Matrix3::ReflectX();  // 默认X轴对称
                break;
            }
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;
    }

    case TransformType::ScaleComposed:
    {
        // 弹出对话框让用户输入变换中心点
        Vec2 center = {0.0f, 0.0f}; // 默认使用坐标原点
        if (ShowCenterPointDialog(center, L"复合缩放", L"请输入缩放中心点"))
        {
            // 复合比例：先缩放(2,2)，再缩放(0.5,1.5)
            Matrix3 scale1 = Matrix3::Scale(2.0, 2.0, center.x, center.y);
            Matrix3 scale2 = Matrix3::Scale(0.5, 1.5, center.x, center.y);
            
            mat = scale2 * scale1;
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;
    }

    case TransformType::RotateComposed:
    {
        // 弹出对话框让用户输入变换中心点
        Vec2 center = {0.0f, 0.0f}; // 默认使用坐标原点
        if (ShowCenterPointDialog(center, L"复合旋转", L"请输入旋转中心点"))
        {
            // 复合旋转：先旋转30度，再旋转45度
            Matrix3 rotate1 = Matrix3::Rotate(30.0 * 3.14159265358979323846 / 180.0, center.x, center.y);
            Matrix3 rotate2 = Matrix3::Rotate(45.0 * 3.14159265358979323846 / 180.0, center.x, center.y);
            
            mat = rotate2 * rotate1;
            shape->Transform(mat);
            InvalidateRect(app.GetMainWindow(), nullptr, TRUE);
        }
        break;
    }

    default:
        break;
    }
}
