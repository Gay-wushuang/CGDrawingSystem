#pragma once
#include <windows.h>

// 变换参数输入对话框
class TransformDialog
{
public:
    static bool ShowTranslateDialog(double& dx, double& dy);
    static bool ShowScaleDialog(double& sx, double& sy);
    static bool ShowRotateDialog(double& angle);
    static bool ShowShearDialog(double& shx, double& shy);
    static bool ShowReflectDialog(int& reflectType);  // 0=X轴, 1=Y轴, 2=原点
    
private:
    static double s_dx, s_dy, s_sx, s_sy, s_angle, s_shx, s_shy;
    static int s_reflectType;  // 0=X轴, 1=Y轴, 2=原点
    static INT_PTR CALLBACK TranslateProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK ScaleProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK RotateProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK ShearProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK ReflectProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
};