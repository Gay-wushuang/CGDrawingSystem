#include "TransformDialog.h"
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"

// 静态成员初始化
double TransformDialog::s_dx = 0.0, TransformDialog::s_dy = 0.0;
double TransformDialog::s_sx = 1.0, TransformDialog::s_sy = 1.0;
double TransformDialog::s_angle = 0.0;
double TransformDialog::s_shx = 0.0, TransformDialog::s_shy = 0.0;

// 平移对话框处理函数
INT_PTR CALLBACK TransformDialog::TranslateProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR buffer[32];
            _stprintf_s(buffer, TEXT("%.2f"), s_dx);
            SetDlgItemText(hwndDlg, IDC_EDIT_DX, buffer);
            _stprintf_s(buffer, TEXT("%.2f"), s_dy);
            SetDlgItemText(hwndDlg, IDC_EDIT_DY, buffer);
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR buffer[32];
            GetDlgItemText(hwndDlg, IDC_EDIT_DX, buffer, 32);
            s_dx = _tstof(buffer);
            GetDlgItemText(hwndDlg, IDC_EDIT_DY, buffer, 32);
            s_dy = _tstof(buffer);
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

// 缩放对话框处理函数
INT_PTR CALLBACK TransformDialog::ScaleProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR buffer[32];
            _stprintf_s(buffer, TEXT("%.2f"), s_sx);
            SetDlgItemText(hwndDlg, IDC_EDIT_SX, buffer);
            _stprintf_s(buffer, TEXT("%.2f"), s_sy);
            SetDlgItemText(hwndDlg, IDC_EDIT_SY, buffer);
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR buffer[32];
            GetDlgItemText(hwndDlg, IDC_EDIT_SX, buffer, 32);
            s_sx = _tstof(buffer);
            GetDlgItemText(hwndDlg, IDC_EDIT_SY, buffer, 32);
            s_sy = _tstof(buffer);
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

// 旋转对话框处理函数
INT_PTR CALLBACK TransformDialog::RotateProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR buffer[32];
            _stprintf_s(buffer, TEXT("%.2f"), s_angle);
            SetDlgItemText(hwndDlg, IDC_EDIT_ANGLE, buffer);
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR buffer[32];
            GetDlgItemText(hwndDlg, IDC_EDIT_ANGLE, buffer, 32);
            s_angle = _tstof(buffer);
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

// 错切对话框处理函数
INT_PTR CALLBACK TransformDialog::ShearProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR buffer[32];
            _stprintf_s(buffer, TEXT("%.2f"), s_shx);
            SetDlgItemText(hwndDlg, IDC_EDIT_SHX, buffer);
            _stprintf_s(buffer, TEXT("%.2f"), s_shy);
            SetDlgItemText(hwndDlg, IDC_EDIT_SHY, buffer);
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR buffer[32];
            GetDlgItemText(hwndDlg, IDC_EDIT_SHX, buffer, 32);
            s_shx = _tstof(buffer);
            GetDlgItemText(hwndDlg, IDC_EDIT_SHY, buffer, 32);
            s_shy = _tstof(buffer);
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

bool TransformDialog::ShowTranslateDialog(double& dx, double& dy)
{
    s_dx = dx;  // 设置默认值
    s_dy = dy;
    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_TRANSFORM_TRANSLATE), 
        GetActiveWindow(), 
        TranslateProc);
    if (result == IDOK)
    {
        dx = s_dx;
        dy = s_dy;
        return true;
    }
    return false;
}

bool TransformDialog::ShowScaleDialog(double& sx, double& sy)
{
    s_sx = sx;
    s_sy = sy;
    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_TRANSFORM_SCALE), 
        GetActiveWindow(), 
        ScaleProc);
    if (result == IDOK)
    {
        sx = s_sx;
        sy = s_sy;
        return true;
    }
    return false;
}

bool TransformDialog::ShowRotateDialog(double& angle)
{
    s_angle = angle;
    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_TRANSFORM_ROTATE), 
        GetActiveWindow(), 
        RotateProc);
    if (result == IDOK)
    {
        angle = s_angle;
        return true;
    }
    return false;
}

int TransformDialog::s_reflectType = 0;  // 默认X轴对称

bool TransformDialog::ShowShearDialog(double& shx, double& shy)
{
    s_shx = shx;
    s_shy = shy;
    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_TRANSFORM_SHEAR), 
        GetActiveWindow(), 
        ShearProc);
    if (result == IDOK)
    {
        shx = s_shx;
        shy = s_shy;
        return true;
    }
    return false;
}

// 对称对话框处理函数
INT_PTR CALLBACK TransformDialog::ReflectProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // 设置默认选中的单选按钮
            switch (s_reflectType)
            {
            case 0:  // X轴对称
                CheckRadioButton(hwndDlg, IDC_RADIO_REFLECT_X, IDC_RADIO_REFLECT_ORIGIN, IDC_RADIO_REFLECT_X);
                break;
            case 1:  // Y轴对称
                CheckRadioButton(hwndDlg, IDC_RADIO_REFLECT_X, IDC_RADIO_REFLECT_ORIGIN, IDC_RADIO_REFLECT_Y);
                break;
            case 2:  // 原点对称
                CheckRadioButton(hwndDlg, IDC_RADIO_REFLECT_X, IDC_RADIO_REFLECT_ORIGIN, IDC_RADIO_REFLECT_ORIGIN);
                break;
            default:
                CheckRadioButton(hwndDlg, IDC_RADIO_REFLECT_X, IDC_RADIO_REFLECT_ORIGIN, IDC_RADIO_REFLECT_X);
                break;
            }
            return TRUE;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // 确定按钮：检查哪个单选按钮被选中
            if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_REFLECT_X) == BST_CHECKED)
                s_reflectType = 0;  // X轴对称
            else if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_REFLECT_Y) == BST_CHECKED)
                s_reflectType = 1;  // Y轴对称
            else if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_REFLECT_ORIGIN) == BST_CHECKED)
                s_reflectType = 2;  // 原点对称
            else
                s_reflectType = 0;  // 默认X轴对称
            
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

bool TransformDialog::ShowReflectDialog(int& reflectType)
{
    s_reflectType = reflectType;
    INT_PTR result = DialogBox(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_TRANSFORM_REFLECT), 
        GetActiveWindow(), 
        ReflectProc);
    if (result == IDOK)
    {
        reflectType = s_reflectType;
        return true;
    }
    return false;
}