#pragma once
#include "ToolBase.h"
#include "../core/Canvas.h"
#include "../core/Types.h"  // 包含统一的类型定义
#include <queue>
#include <set>
#include <vector>

// 填充工具（支持过程演示）
class ToolFill : public ToolBase
{
public:
    ToolFill();
    ~ToolFill();

    void OnLButtonDown(POINT pt) override;
    
    // 设置填充参数（由外部调用）
    void SetFillParams(int seedX, int seedY, Color fillColor, Color boundaryColor);

    // 开始填充演示
    void StartFillDemo(class Canvas& canvas, int seedX, int seedY, Color fillColor, Color boundaryColor);
    
    // 显示填充参数对话框
    static bool ShowFillParamsDialog(int& seedX, int& seedY, int& scanDir, const wchar_t* title);

    // 更新填充演示（每次调用填充一批像素）
    void UpdateFillDemo(class Canvas& canvas, int pixelsPerFrame = 10);
    
    // 扫描线填充相关函数
    void StartScanlineFillDemo(class Canvas& canvas, const std::vector<Point>& points, Color fillColor);
    void UpdateScanlineFillDemo(class Canvas& canvas, int linesPerFrame = 1);

    // 控制函数
    void Play();
    void Pause();
    void Reset();

    bool IsPlaying() const { return m_isPlaying; }
    bool HasFillTask() const { return !m_pixelQueue.empty(); }
    bool HasScanlineFillTask() const { return !m_polygonPoints.empty(); }
    int GetScanlineIndex() const { return m_scanlineIndex; }

    // 在 WM_PAINT 中调用：绘制已经填充的像素（用于"过程演示"且避免重绘被清空）
    void DrawFillOverlay(Canvas& canvas) const;

private:
    struct FillTask
    {
        int seedX, seedY;
        Color fillColor;
        Color boundaryColor;
    };

    struct FilledPixel
    {
        int x, y;
        Color color;
    };

    // 辅助函数：精确检测边界
    bool IsBoundaryPixel(Color pixelColor, Color boundaryColor, Color fillColor) const;

    bool m_isPlaying = false;
    std::queue<FillTask> m_fillQueue;
    std::queue<std::pair<int, int>> m_pixelQueue;  // 待填充的像素队列
    Color m_currentFillColor;
    Color m_currentBoundaryColor;
    std::set<std::pair<int, int>> m_visited;
    std::vector<FilledPixel> m_filledPixels;  // 已填充像素（用于重绘/演示）
    int m_canvasWidth = 0;
    int m_canvasHeight = 0;
    
    // 扫描线填充相关变量
    std::vector<Point> m_polygonPoints;  // 多边形顶点
    int m_scanlineIndex = 0;  // 当前扫描的行索引
    int m_yMin = 0;  // 多边形Y轴最小值
    int m_yMax = 0;  // 多边形Y轴最大值
};
