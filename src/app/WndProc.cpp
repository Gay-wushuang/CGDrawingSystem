#include "WndProc.h"
#include "App.h"
#include "resource.h"
#include "../core/Canvas.h"
#include "../core/CoordinateAxis.h"
#include "../core/Color.h"
#include "../core/Types.h"
#include "../tools/ToolDrawBasic.h"
#include "../tools/ToolTransform.h"
#include "../tools/ToolCurve.h"
#include "../tools/ToolClip.h"
#include "../tools/ToolFill.h"
#include "../model/ShapeStar.h"
#include "../model/ShapePolygon.h"
#include <memory>
#include <algorithm>

// 简单全局/静态对象：实际项目中可以放到 App 里
static Canvas         g_Canvas;
static CoordinateAxis g_Axis;
static ToolDrawBasic  g_DrawTool;
static ToolTransform  g_TransformTool;
static ToolCurve      g_CurveTool;
static ToolClip       g_ClipTool;
static ToolFill       g_FillTool;

// 填充演示定时器 ID
#define TIMER_FILL_DEMO  1001

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RECT rc{};
        GetClientRect(hWnd, &rc);
        g_Canvas.Resize(rc.right - rc.left, rc.bottom - rc.top);
        App::Get().SetScreenSize(rc.right - rc.left, rc.bottom - rc.top);  // 初始化坐标转换器的屏幕尺寸
        return 0;
    }
    case WM_SIZE:
    {
        int width  = LOWORD(lParam);
        int height = HIWORD(lParam);
        if (width > 0 && height > 0)
        {
            g_Canvas.Resize(width, height);
            App::Get().SetScreenSize(width, height);  // 更新坐标转换器的屏幕尺寸
            // 窗口大小改变后需要重绘
            InvalidateRect(hWnd, nullptr, FALSE);
        }
        return 0;
    }
    case WM_COMMAND:
    {
        const int wmId = LOWORD(wParam);
        App& app = App::Get();

        switch (wmId)
        {
        // 基本图形绘制相关命令
        case ID_BASIC_DDA_LINE:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::DDA_Line);
            g_DrawTool.SetTool(BasicDrawTool::DDA_Line);
            break;
        case ID_BASIC_BRESENHAM_LINE:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::Bresenham_Line);
            g_DrawTool.SetTool(BasicDrawTool::Bresenham_Line);
            break;
        case ID_BASIC_SYS_LINE:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::System_Line);
            g_DrawTool.SetTool(BasicDrawTool::System_Line);
            break;
        case ID_BASIC_CIRCLE:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::Circle);
            g_DrawTool.SetTool(BasicDrawTool::Circle);
            break;
        case ID_BASIC_ELLIPSE:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::Ellipse);
            g_DrawTool.SetTool(BasicDrawTool::Ellipse);
            break;
        case ID_BASIC_RECT:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::Rect);
            g_DrawTool.SetTool(BasicDrawTool::Rect);
            break;
        case ID_BASIC_POLYGON:
            app.SetToolCategory(ToolCategory::BasicDraw);
            app.SetBasicDrawTool(BasicDrawTool::Polygon);
            g_DrawTool.SetTool(BasicDrawTool::Polygon);
            break;

        // 图形变换
        case ID_TRANS_TRANSLATE:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::Translate);
            break;
        case ID_TRANS_SCALE:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::Scale);
            break;
        case ID_TRANS_SHEAR:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::Shear);
            break;
        case ID_TRANS_REFLECT:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::Reflect);
            break;
        case ID_TRANS_ROTATE:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::Rotate);
            break;
        case ID_TRANS_SCALE_COMPOSED:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::ScaleComposed);
            break;
        case ID_TRANS_ROTATE_COMPOSED:
            app.SetToolCategory(ToolCategory::Transform);
            g_TransformTool.SetTransformType(TransformType::RotateComposed);
            break;
        case ID_TRANS_ADD_STAR:
        {
            // 添加预置五角星到场景中心
            RECT rc{};
            GetClientRect(hWnd, &rc);
            int cx = (rc.left + rc.right) / 2;
            int cy = (rc.top + rc.bottom) / 2;
            auto star = std::make_shared<ShapeStar>(cx, cy, 80, 40, Color::Blue());
            app.AddShape(star);
            break;
        }

        // 自由曲线
        case ID_CURVE_BEZIER:
            app.SetToolCategory(ToolCategory::Curve);
            g_CurveTool.SetCurveType(CurveToolType::Bezier);
            break;
        case ID_CURVE_BSPLINE:
            app.SetToolCategory(ToolCategory::Curve);
            g_CurveTool.SetCurveType(CurveToolType::BSpline);
            break;

        // 裁剪
        case ID_CLIP_LINE:
            app.SetToolCategory(ToolCategory::Clip);
            g_ClipTool.SetClipType(ClipToolType::Line);
            break;
        case ID_CLIP_POLYGON:
            app.SetToolCategory(ToolCategory::Clip);
            g_ClipTool.SetClipType(ClipToolType::Polygon);
            break;

        // 填充
        case ID_FILL_SEED:
            app.SetToolCategory(ToolCategory::Fill);
            g_FillTool.Reset();
            app.SetFillType(0); // 0表示种子填充
            break;
        case ID_FILL_SCANLINE:
            app.SetToolCategory(ToolCategory::Fill);
            g_FillTool.Reset();
            // 设置填充类型为扫描线填充
            app.SetFillType(1); // 1表示扫描线填充
            app.SetFillParams(app.GetFillSeedX(), app.GetFillSeedY(), app.GetFillColor(), app.GetBoundaryColor());
            break;
        case ID_FILL_PLAY:
            g_FillTool.Play();
            SetTimer(hWnd, TIMER_FILL_DEMO, 30, nullptr);  // 30ms 更新一次
            break;
        case ID_FILL_PAUSE:
            g_FillTool.Pause();
            KillTimer(hWnd, TIMER_FILL_DEMO);
            break;
        case ID_FILL_RESET:
            g_FillTool.Reset();
            KillTimer(hWnd, TIMER_FILL_DEMO);
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

        // 文件菜单
        case ID_FILE_CLEAR:
            App::Get().ClearAllShapes();  // 清除所有图形
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

        default:
            break;
        }

        // 工具切换后重绘
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ToolCategory cat = App::Get().GetToolCategory();
        if (cat == ToolCategory::BasicDraw)
        {
            g_DrawTool.OnLButtonDown(pt);
        }
        else if (cat == ToolCategory::Transform)
        {
            g_TransformTool.OnLButtonDown(pt);
        }
        else if (cat == ToolCategory::Curve)
        {
            g_CurveTool.OnLButtonDown(pt);
        }
        else if (cat == ToolCategory::Clip)
        {
            g_ClipTool.OnLButtonDown(pt);
        }
        else if (cat == ToolCategory::Fill)
        {
            App& app = App::Get();
            if (app.GetFillType() == 1) // 扫描线填充
            {
                // 在点击位置重新选择图形
                app.SelectShape(pt.x, pt.y);
                auto selectedShape = app.GetSelectedShape();
                
                if (selectedShape) 
                {
                    if (selectedShape->GetType() == ShapeType::Polygon)
                    {
                        // 弹出颜色选择对话框
                        CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
                        COLORREF color = app.GetFillColor().ToCOLORREF();
                        COLORREF acrCustClr[16] = { 0 }; // 静态数组存储自定义颜色

                        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                        cc.hwndOwner = hWnd;
                        cc.rgbResult = color;
                        cc.lpCustColors = (LPDWORD)acrCustClr;

                        if (ChooseColor(&cc)) // 用户点击了确定
                        {
                            // 更新填充颜色
                            Color fillColor = Color(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
                            app.SetFillParams(pt.x, pt.y, fillColor, Color::Black()); // 边界颜色默认为黑色
                            
                            // 将多边形的顶点转换为Point类型
                            std::vector<Point> polygonPoints;
                            std::vector<int> xPoints, yPoints;
                            
                            // 动态转换为ShapePolygon以获取顶点
                            auto polygonShape = std::dynamic_pointer_cast<ShapePolygon>(selectedShape);
                            if (polygonShape)
                            {
                                polygonShape->GetPoints(xPoints, yPoints);
                                
                                // 组合x和y坐标为Point
                                for (size_t i = 0; i < xPoints.size() && i < yPoints.size(); i++)
                                {
                                    polygonPoints.push_back({xPoints[i], yPoints[i]});
                                }
                                
                                // 检查是否有足够的顶点形成多边形
                                if (polygonPoints.size() >= 3)
                                {
                                    g_FillTool.StartScanlineFillDemo(g_Canvas, polygonPoints, fillColor);
                                    // 不立即启动定时器，等待用户点击"播放"按钮
                                    MessageBoxW(hWnd, L"已设置填充参数，点击菜单中的'播放'按钮开始填充演示！", L"提示", MB_OK | MB_ICONINFORMATION);
                                }
                                else
                                {
                                    MessageBoxW(hWnd, L"多边形顶点数量不足，无法进行扫描线填充！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                            else
                            {
                                MessageBoxW(hWnd, L"无法获取多边形顶点信息！", L"错误", MB_OK | MB_ICONERROR);
                            }
                        }
                    }
                    else
                    {
                        // 选中了非多边形图形
                        MessageBoxW(hWnd, L"选中的是非多边形图形，无法进行扫描线填充！请选中一个多边形。", L"提示", MB_OK | MB_ICONWARNING);
                    }
                }
                else
                {
                    // 没有选中任何图形
                    MessageBoxW(hWnd, L"未选中任何图形，请点击一个多边形以进行填充！", L"提示", MB_OK | MB_ICONWARNING);
                }
            }
            else // 种子填充
            {
                // 弹出颜色选择对话框
                CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
                COLORREF color = app.GetFillColor().ToCOLORREF();
                COLORREF acrCustClr[16] = { 0 }; // 静态数组存储自定义颜色

                cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                cc.hwndOwner = hWnd;
                cc.rgbResult = color;
                cc.lpCustColors = (LPDWORD)acrCustClr;

                if (ChooseColor(&cc)) // 用户点击了确定
                {
                    // 更新填充颜色
                    Color fillColor = Color(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
                    app.SetFillParams(pt.x, pt.y, fillColor, Color::Black()); // 边界颜色默认为黑色
                    
                    // 开始种子填充演示（但不立即启动定时器）
                    g_FillTool.StartFillDemo(g_Canvas, pt.x, pt.y, fillColor, Color::Black());
                    // 不立即启动定时器，等待用户点击"播放"按钮
                    MessageBoxW(hWnd, L"已设置填充参数，点击菜单中的'播放'按钮开始填充演示！", L"提示", MB_OK | MB_ICONINFORMATION);
                }
            }
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ToolCategory cat = App::Get().GetToolCategory();
        if (cat == ToolCategory::BasicDraw)
        {
            g_DrawTool.OnLButtonUp(pt);
        }
        else if (cat == ToolCategory::Clip)
        {
            g_ClipTool.OnLButtonUp(pt);
        }
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ToolCategory cat = App::Get().GetToolCategory();
        if (cat == ToolCategory::BasicDraw)
        {
            g_DrawTool.OnMouseMove(pt);
        }
        else if (cat == ToolCategory::Clip)
        {
            g_ClipTool.OnMouseMove(pt);
        }
        else if (cat == ToolCategory::Curve)
        {
            g_CurveTool.OnMouseMove(pt);
        }
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ToolCategory cat = App::Get().GetToolCategory();
        if (cat == ToolCategory::BasicDraw)
        {
            g_DrawTool.OnRButtonDown(pt);
        }
        else if (cat == ToolCategory::Curve)
        {
            g_CurveTool.OnRButtonDown(pt);
        }
        else if (cat == ToolCategory::Clip)
        {
            g_ClipTool.OnRButtonDown(pt);
        }
        return 0;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            ToolCategory cat = App::Get().GetToolCategory();
            if (cat == ToolCategory::BasicDraw)
            {
                g_DrawTool.OnEscape();
            }
            else if (cat == ToolCategory::Curve)
            {
                g_CurveTool.OnEscape();
            }
            else if (cat == ToolCategory::Clip)
            {
                g_ClipTool.OnRButtonDown({0, 0});  // 重置
            }
        }
        return 0;
    }
    case WM_TIMER:
    {
        if (wParam == TIMER_FILL_DEMO)
        {
            App& app = App::Get();
            if (app.GetFillType() == 1) // 扫描线填充
            {
                g_FillTool.UpdateScanlineFillDemo(g_Canvas, 3);  // 每帧填充3行扫描线，较慢但更清晰的演示
                if (!g_FillTool.IsPlaying()) // 检查扫描线填充是否完成
                {
                    KillTimer(hWnd, TIMER_FILL_DEMO);
                }
            }
            else // 种子填充
            {
                if (g_FillTool.IsPlaying() && g_FillTool.HasFillTask())
                {
                    g_FillTool.UpdateFillDemo(g_Canvas, 15);  // 每帧填充15个像素，较慢但更清晰的演示
                }
                else
                {
                    KillTimer(hWnd, TIMER_FILL_DEMO);
                }
            }
            
            // 强制重绘窗口以显示填充过程
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 清空画布为白色
        g_Canvas.Clear(Color::White());

        // 绘制坐标系（使用 Canvas::PutPixel）
        g_Axis.Draw(g_Canvas);

        // 绘制场景中所有已完成的图形
        App::Get().DrawAllShapes(g_Canvas);

        // 绘制当前正在绘制的预览（如果有）
        ToolCategory cat = App::Get().GetToolCategory();
        if (cat == ToolCategory::BasicDraw && g_DrawTool.IsDrawing())
        {
            g_DrawTool.DrawPreview(g_Canvas);
        }
        else if (cat == ToolCategory::Curve && g_CurveTool.IsDrawing())
        {
            g_CurveTool.DrawPreview(g_Canvas);
        }
        else if (cat == ToolCategory::Clip && g_ClipTool.IsDrawing())
        {
            g_ClipTool.DrawPreview(g_Canvas);
        }

        // 将离屏缓冲呈现到窗口
        g_Canvas.Present(hdc);

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        // 清理定时器
        KillTimer(hWnd, TIMER_FILL_DEMO);
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}


