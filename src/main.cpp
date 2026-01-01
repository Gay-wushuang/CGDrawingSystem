#include <windows.h>
#include "app/WndProc.h"
#include "app/App.h"
#include "app/resource.h"

// 窗口标题必须固定为：学号_姓名_计算机图形学期末大作业
static const wchar_t* const kWindowTitle = L"学号_姓名_计算机图形学期末大作业";
static const wchar_t* const kWindowClass = L"CGFinalProjectMainWindowClass";

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEXW wcex = {};                                  // 窗口类对象
    // 初始化窗口类对象
    wcex.cbSize        = sizeof(WNDCLASSEXW);               // 窗口类对象大小
    wcex.style         = CS_HREDRAW | CS_VREDRAW;           // 窗口类样式：水平重绘 | 垂直重绘
    wcex.lpfnWndProc   = WndProc;                            // 窗口过程函数指针
    wcex.cbClsExtra    = 0;                                     // 窗口类额外内存大小
    wcex.cbWndExtra    = 0;                                      // 窗口额外内存大小
    wcex.hInstance     = hInstance;                                // 实例句柄
    wcex.hIcon         = LoadIconW(nullptr, IDI_APPLICATION);  // 窗口图标句柄
    wcex.hCursor       = LoadCursorW(nullptr, IDC_ARROW);      // 窗口光标句柄
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);           // 窗口背景画刷句柄
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDR_MAINMENU);      // 菜单资源句柄
    wcex.lpszClassName = kWindowClass;                        // 窗口类名
    wcex.hIconSm       = LoadIconW(nullptr, IDI_APPLICATION); // 窗口小图标句柄

    if (!RegisterClassExW(&wcex))
    {
        MessageBoxW(nullptr, L"RegisterClassEx 失败", kWindowTitle, MB_ICONERROR);
        return 0;
    }

    HWND hWnd = CreateWindowW(
        kWindowClass,
        kWindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        960, 720,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if (!hWnd)
    {
        MessageBoxW(nullptr, L"CreateWindow 失败", kWindowTitle, MB_ICONERROR);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 初始化全局 App 状态
    App::Get().Initialize(hInstance, hWnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}





