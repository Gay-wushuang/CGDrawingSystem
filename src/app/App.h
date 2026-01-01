#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include "../core/Color.h"
#include "../model/Shape.h"
#include "../core/Math2D.h"

// 一级工具类别（对应五个顶层菜单）
enum class ToolCategory
{
    None = 0,
    BasicDraw,
    Transform,
    Curve,
    Clip,
    Fill
};

// 基本图形绘制子工具类型
enum class BasicDrawTool
{
    None,
    DDA_Line,
    Bresenham_Line,
    System_Line,
    Circle,
    Ellipse,
    Rect,
    Polygon
};

class App
{
public:
    static App& Get()
    {
        static App instance;
        return instance;
    }

    void Initialize(HINSTANCE hInst, HWND hMainWnd)
    {
        m_hInstance = hInst;
        m_hMainWnd  = hMainWnd;
    }

    HINSTANCE GetInstance() const { return m_hInstance; }
    HWND      GetMainWindow() const { return m_hMainWnd; }

    void SetToolCategory(ToolCategory cat) { m_currentCategory = cat; }
    ToolCategory GetToolCategory() const { return m_currentCategory; }

    void SetBasicDrawTool(BasicDrawTool tool) { m_basicDrawTool = tool; }
    BasicDrawTool GetBasicDrawTool() const { return m_basicDrawTool; }

    // 场景管理：添加图形到场景
    void AddShape(std::shared_ptr<Shape> shape);
    
    // 绘制所有场景中的图形
    void DrawAllShapes(class Canvas& canvas) const;

    // 清空场景
    void ClearScene();
    
    // 清空所有图形
    void ClearAllShapes();

    // 选中图形管理
    void SelectShape(int x, int y);  // 通过点击位置选中图形
    std::shared_ptr<Shape> GetSelectedShape() const { return m_selectedShape; }
    void ClearSelection() { m_selectedShape.reset(); }

    // 填充参数设置
    void SetFillParams(int seedX, int seedY, Color fillColor, Color boundaryColor);

    // 获取填充参数
    int GetFillSeedX() const { return m_fillSeedX; }
    int GetFillSeedY() const { return m_fillSeedY; }
    int GetFillType() const { return m_fillType; }
    void SetFillType(int type) { m_fillType = type; }
    Color GetFillColor() const { return m_fillColor; }
    Color GetBoundaryColor() const { return m_boundaryColor; }

    // 坐标转换器相关方法
    void SetScreenSize(int width, int height) { m_converter.SetScreenSize(width, height); }
    CoordinateConverter& GetCoordinateConverter() { return m_converter; }
    const CoordinateConverter& GetCoordinateConverter() const { return m_converter; }

private:
    App() = default;
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    HINSTANCE    m_hInstance = nullptr;
    HWND         m_hMainWnd  = nullptr;
    ToolCategory m_currentCategory = ToolCategory::None;
    BasicDrawTool m_basicDrawTool = BasicDrawTool::None;

    // 场景图形列表
    std::vector<std::shared_ptr<Shape>> m_shapes;

    // 当前选中的图形
    std::shared_ptr<Shape> m_selectedShape;

    // 填充参数
    int m_fillSeedX = 0;
    int m_fillSeedY = 0;
    int m_fillType = 0; // 0: 种子填充, 1: 扫描线填充
    Color m_fillColor = {255, 0, 0}; // 红色
    Color m_boundaryColor = {0, 0, 0}; // 黑色

    // 坐标转换器
    CoordinateConverter m_converter;
};




