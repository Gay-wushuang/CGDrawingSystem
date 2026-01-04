#include "ToolFill.h"
#include "../app/App.h"
#include "../core/Types.h"  // 包含统一的类型定义
#include <algorithm>
#include <cmath>
#include <windowsx.h>  // 用于GET_X_LPARAM, GET_Y_LPARAM

ToolFill::ToolFill()
{
}

ToolFill::~ToolFill()
{
}

// 辅助函数：精确检测边界
bool ToolFill::IsBoundaryPixel(Color pixelColor, Color boundaryColor, Color fillColor) const
{
    // 精确比较：使用RGB分量比较
    return (pixelColor.r == boundaryColor.r && pixelColor.g == boundaryColor.g && pixelColor.b == boundaryColor.b) ||
           (pixelColor.r == fillColor.r && pixelColor.g == fillColor.g && pixelColor.b == fillColor.b);
}

void ToolFill::OnLButtonDown(POINT pt)
{
    // 对于填充工具，点击事件由WndProc直接处理
}

void ToolFill::SetFillParams(int seedX, int seedY, Color fillColor, Color boundaryColor)
{
    // 设置填充参数
}

void ToolFill::StartFillDemo(Canvas& canvas, int seedX, int seedY, Color fillColor, Color boundaryColor)
{
    m_currentFillColor = fillColor;
    m_currentBoundaryColor = boundaryColor;
    m_canvasWidth = canvas.Width();
    m_canvasHeight = canvas.Height();
    m_isPlaying = true;
    
    // 清空之前的队列
    while (!m_pixelQueue.empty()) m_pixelQueue.pop();
    m_visited.clear();
    m_filledPixels.clear();
    
    if (seedX < 0 || seedX >= m_canvasWidth || seedY < 0 || seedY >= m_canvasHeight)
    {
        m_isPlaying = false;
        return;
    }
    m_pixelQueue.push({ seedX, seedY });
    m_visited.insert({ seedX, seedY }); // 立即标记"已入队"，避免重复入队卡顿
}

void ToolFill::UpdateFillDemo(Canvas& canvas, int pixelsPerFrame)
{
    if (!m_isPlaying) return;

    auto TryPush = [&](int x, int y)
    {
        if (x < 0 || x >= m_canvasWidth || y < 0 || y >= m_canvasHeight) return;
        auto key = std::make_pair(x, y);
        if (m_visited.insert(key).second)  // first time seen
        {
            m_pixelQueue.push(key);
        }
    };
    
    int processedPixels = 0;
    while (!m_pixelQueue.empty() && processedPixels < pixelsPerFrame)
    {
        auto [x, y] = m_pixelQueue.front();
        m_pixelQueue.pop();
        
        // 检查是否为边界颜色或已填充颜色
        Color currentColor = canvas.GetPixel(x, y);
        
        // 使用精确的边界检测函数
        if (IsBoundaryPixel(currentColor, m_currentBoundaryColor, m_currentFillColor)) {
            continue;
        }
        
        // 填充当前像素
        canvas.PutPixel(x, y, m_currentFillColor);
        m_filledPixels.push_back({ x, y, m_currentFillColor });
        
        // 用 4 邻域更稳：避免斜边/尖角处从对角缝"钻出去"
        TryPush(x + 1, y);
        TryPush(x - 1, y);
        TryPush(x, y + 1);
        TryPush(x, y - 1);
        
        processedPixels++;
    }
    
    if (m_pixelQueue.empty())
    {
        m_isPlaying = false;
    }
}

void ToolFill::StartScanlineFillDemo(Canvas& canvas, const std::vector<Point>& points, Color fillColor)
{
    if (points.size() < 3) return;
    
    m_polygonPoints = points;
    m_currentFillColor = fillColor;
    m_canvasWidth = canvas.Width();
    m_canvasHeight = canvas.Height();
    m_isPlaying = true;
    
    // 计算Y轴范围
    m_yMin = m_polygonPoints[0].y;
    m_yMax = m_polygonPoints[0].y;
    for (const auto& p : m_polygonPoints) {
        if (p.y < m_yMin) m_yMin = p.y;
        if (p.y > m_yMax) m_yMax = p.y;
    }
    
    // 从点击的y坐标开始扫描（夹在范围内）
    int startY = App::Get().GetFillSeedY();
    if (startY < m_yMin) startY = m_yMin;
    if (startY > m_yMax) startY = m_yMax;
    m_scanlineIndex = startY;
    
    // 获取边界颜色 - 从多边形的边线上取一个点的颜色
    if (!points.empty()) {
        // 从第一个顶点的邻近点获取边界颜色
        Point start = points[0];
        Point end = points[1 % points.size()];
        // 取边界线上一个点的颜色作为边界色
        int midX = (start.x + end.x) / 2;
        int midY = (start.y + end.y) / 2;
        m_currentBoundaryColor = canvas.GetPixel(midX, midY);
    } else {
        m_currentBoundaryColor = Color::Black(); // 默认边界色
    }
}

void ToolFill::UpdateScanlineFillDemo(Canvas& canvas, int linesPerFrame)
{
    if (!m_isPlaying || m_polygonPoints.size() < 3)
    {
        m_isPlaying = false;
        return;
    }

    int filledLines = 0;
    while (m_scanlineIndex <= m_yMax && filledLines < linesPerFrame)
    {
        int y = m_scanlineIndex;
        const double ySample = y + 0.5;            // 像素中心采样，斜边更稳定
        std::vector<double> nodeX;                 // 交点用 double，别 round/unique

        // 遍历每条边 (p[i] -> p[next])
        for (size_t i = 0; i < m_polygonPoints.size(); i++) {
            Point p1 = m_polygonPoints[i];
            Point p2 = m_polygonPoints[(i + 1) % m_polygonPoints.size()];

            // 忽略水平边
            if (p1.y == p2.y) continue;

            // 确保 p1 是 y 较小的点，方便计算
            if (p1.y > p2.y) std::swap(p1, p2);

            // 如果当前行 y 在这条边的 Y 范围内
            // 采用 top-left 规则：包含下端点但不包含上端点
            if (ySample >= p1.y && ySample < p2.y) {
                // 计算交点 x = x1 + (ySample - y1) * (1/k)
                // 1/k = (x2 - x1) / (y2 - y1)
                double currentX = p1.x + (ySample - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                nodeX.push_back(currentX);
            }
        }

        // 对交点 X 进行排序
        std::sort(nodeX.begin(), nodeX.end());

        // 两两配对填充，确保有偶数个交点
        for (size_t i = 0; i + 1 < nodeX.size(); i += 2) {
            if (i + 1 >= nodeX.size()) break;
            double xL = nodeX[i];
            double xR = nodeX[i + 1];
            if (xL > xR) std::swap(xL, xR);

            // 右边界不包含（类似 top-left 规则），减少斜边"吃到外面一列像素"
            int xStart = (int)std::ceil(xL);
            int xEnd   = (int)std::floor(xR) - 1;

            // 确保区间有效
            if (xStart <= xEnd) {
                for (int x = xStart; x <= xEnd; x++) {
                    if (x >= 0 && x < m_canvasWidth && y >= 0 && y < m_canvasHeight) {
                        canvas.PutPixel(x, y, m_currentFillColor);
                        m_filledPixels.push_back({ x, y, m_currentFillColor });
                    }
                }
            }
        }

        m_scanlineIndex++;
        filledLines++;
    }

    if (m_scanlineIndex > m_yMax)
    {
        m_isPlaying = false;
    }
}

void ToolFill::Play()
{
    m_isPlaying = true;
}

void ToolFill::Pause()
{
    m_isPlaying = false;
}

void ToolFill::Reset()
{
    m_isPlaying = false;
    m_scanlineIndex = 0;
    m_yMin = 0;
    m_yMax = 0;
    m_polygonPoints.clear();
    while (!m_pixelQueue.empty()) m_pixelQueue.pop();
    m_visited.clear();
    m_filledPixels.clear();
}

// 显示填充参数对话框 - 简化版本，使用App类管理参数
bool ToolFill::ShowFillParamsDialog(int& seedX, int& seedY, int& scanDir, const wchar_t* title)
{
    // 这个函数可以用来显示一个简单的对话框让用户输入填充参数
    // 但为了简化，我们直接使用App类中的参数
    App& app = App::Get();
    seedX = app.GetFillSeedX();
    seedY = app.GetFillSeedY();
    scanDir = app.GetFillType();  // 0: 种子填充, 1: 扫描线填充
    return true;  // 假设用户确认了参数
}

void ToolFill::DrawFillOverlay(Canvas& canvas) const
{
    for (const auto& px : m_filledPixels)
    {
        canvas.PutPixel(px.x, px.y, px.color);
    }
}