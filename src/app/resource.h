#pragma once

// 主菜单资源 ID
#define IDR_MAINMENU                    101

// 通用资源 ID
#define IDC_STATIC                      -1

// 顶层菜单中每个子命令（这里只是占位，用于切换 currentTool）

// 基本图形绘制
#define ID_BASIC_DDA_LINE               2001
#define ID_BASIC_BRESENHAM_LINE         2002
#define ID_BASIC_SYS_LINE               2003
#define ID_BASIC_CIRCLE                 2004
#define ID_BASIC_ELLIPSE                2005
#define ID_BASIC_RECT                   2006
#define ID_BASIC_POLYGON                2007

// 图形变换
#define ID_TRANS_TRANSLATE              2101
#define ID_TRANS_SCALE                  2102
#define ID_TRANS_SHEAR                  2103
#define ID_TRANS_REFLECT                2104
#define ID_TRANS_ROTATE                 2105
#define ID_TRANS_SCALE_COMPOSED         2106
#define ID_TRANS_ROTATE_COMPOSED        2107
#define ID_TRANS_ADD_STAR               2108

// 自由曲线
#define ID_CURVE_BEZIER                 2201
#define ID_CURVE_BSPLINE                2202

// 图形裁剪
#define ID_CLIP_LINE                    2301
#define ID_CLIP_POLYGON                 2302

// 图形填充
#define ID_FILL_SEED                    2401
#define ID_FILL_SCANLINE                2402
#define ID_FILL_PLAY                    2403
#define ID_FILL_PAUSE                   2404
#define ID_FILL_RESET                   2405

// 变换对话框资源ID
#define IDD_TRANSFORM_TRANSLATE         2000
#define IDD_TRANSFORM_SCALE             2001
#define IDD_TRANSFORM_ROTATE            2002
#define IDD_TRANSFORM_SHEAR             2003

#define IDC_EDIT_DX                     1001
#define IDC_EDIT_DY                     1002
#define IDC_EDIT_SX                     1003
#define IDC_EDIT_SY                     1004
#define IDC_EDIT_ANGLE                  1005
#define IDC_EDIT_SHX                    1006
#define IDC_EDIT_SHY                    1007

// 中心点对话框资源ID
#define IDD_CENTER_POINT                2004

#define IDC_EDIT_CENTER_X               1008
#define IDC_EDIT_CENTER_Y               1009

// 对称变换对话框资源ID
#define IDD_TRANSFORM_REFLECT           2006

#define IDC_RADIO_REFLECT_X             1013
#define IDC_RADIO_REFLECT_Y             1014
#define IDC_RADIO_REFLECT_ORIGIN        1015

// 填充参数对话框资源ID
#define IDD_FILL_PARAMS                 2005

#define IDC_EDIT_FILL_X                 1010
#define IDC_EDIT_FILL_Y                 1011
#define IDC_COMBO_SCAN_DIR              1012

// 清屏命令
#define ID_FILE_CLEAR                   1008



