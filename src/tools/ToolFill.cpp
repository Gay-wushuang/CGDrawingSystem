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
    
    // 将种子点加入队列
    m_pixelQueue.push({seedX, seedY});
}

void ToolFill::UpdateFillDemo(Canvas& canvas, int pixelsPerFrame)
{
    if (!m_isPlaying) return;
    
    int processedPixels = 0;
    while (!m_pixelQueue.empty() && processedPixels < pixelsPerFrame)
    {
        auto [x, y] = m_pixelQueue.front();
        m_pixelQueue.pop();
        
        // 检查边界和访问标记
        if (x < 0 || x >= m_canvasWidth || y < 0 || y >= m_canvasHeight) continue;
        if (m_visited.count({x, y})) continue;
        
        // 检查是否为边界颜色或已填充颜色
        Color currentColor = canvas.GetPixel(x, y);
        if (currentColor == m_currentBoundaryColor || currentColor == m_currentFillColor) {
            continue;
        }
        
        // 填充当前像素
        canvas.PutPixel(x, y, m_currentFillColor);
        m_visited.insert({x, y});
        
        // 将相邻像素加入队列
        m_pixelQueue.push({x + 1, y});
        m_pixelQueue.push({x - 1, y});
        m_pixelQueue.push({x, y + 1});
        m_pixelQueue.push({x, y - 1});
        
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
    
    m_scanlineIndex = m_yMin;
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
        std::vector<int> nodeX; // 存储当前行与边的交点 x 坐标

        // 遍历每条边 (p[i] -> p[next])
        for (size_t i = 0; i < m_polygonPoints.size(); i++) {
            Point p1 = m_polygonPoints[i];
            Point p2 = m_polygonPoints[(i + 1) % m_polygonPoints.size()];

            // 忽略水平边
            if (p1.y == p2.y) continue;

            // 确保 p1 是 y 较小的点，方便计算
            if (p1.y > p2.y) std::swap(p1, p2);

            // 如果当前行 y 在这条边的 Y 范围内
            // 注意：对于顶点情况的处理，这里采用包含下端点但不包含上端点的策略
            // 但为了处理共享顶点的情况，我们对所有边都进行计算
            if (y >= p1.y && y < p2.y) {
                // 计算交点 x = x1 + (y - y1) * (1/k)
                // 1/k = (x2 - x1) / (y2 - y1)
                double currentX = p1.x + (double)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                nodeX.push_back((int)round(currentX));
            }
        }

        // 对交点 X 进行排序
        std::sort(nodeX.begin(), nodeX.end());

        // 去除重复的交点，避免填充错误
        nodeX.erase(std::unique(nodeX.begin(), nodeX.end()), nodeX.end());

        // 两两配对填充，确保有偶数个交点
        for (size_t i = 0; i < nodeX.size(); i += 2) {
            if (i + 1 >= nodeX.size()) break;
            int xStart = nodeX[i];
            int xEnd = nodeX[i+1];

            // 确保区间有效
            if (xStart <= xEnd) {
                for (int x = xStart; x <= xEnd; x++) {
                    if (x >= 0 && x < m_canvasWidth && y >= 0 && y < m_canvasHeight) {
                        canvas.PutPixel(x, y, m_currentFillColor);
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