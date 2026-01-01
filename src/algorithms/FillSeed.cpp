#include "FillSeed.h"
#include <queue>
#include <set>
#include <algorithm>

namespace Algo
{
    void FillSeed(Canvas& canvas,
        int seedX, int seedY,
        Color fillColor,
        Color boundaryColor,
        std::function<void(int x, int y)> onPixelFilled)
    {
        int width = canvas.Width();
        int height = canvas.Height();

        if (seedX < 0 || seedX >= width || seedY < 0 || seedY >= height)
            return;

        // 使用队列实现 BFS
        std::queue<std::pair<int, int>> queue;
        std::set<std::pair<int, int>> visited;

        // 获取种子点的原始颜色（作为边界色，如果 boundaryColor 未指定）
        Color seedColor = canvas.GetPixel(seedX, seedY);
        
        // 如果未指定边界色（r=-1），使用种子点颜色作为边界
        Color actualBoundaryColor = boundaryColor;
        if (boundaryColor.r == 255 && boundaryColor.g == 255 && boundaryColor.b == 255)
        {
            actualBoundaryColor = seedColor;
        }
        
        // 如果种子点已经是填充色，直接返回
        if (seedColor.r == fillColor.r && seedColor.g == fillColor.g && seedColor.b == fillColor.b)
            return;

        // 检查像素颜色是否等于边界色或填充色
        auto IsBoundaryOrFilled = [&](int x, int y) -> bool {
            if (x < 0 || x >= width || y < 0 || y >= height)
                return true;
            
            Color pixelColor = canvas.GetPixel(x, y);
            
            // 检查是否是边界色或填充色
            bool isBoundary = (pixelColor.r == actualBoundaryColor.r && 
                              pixelColor.g == actualBoundaryColor.g && 
                              pixelColor.b == actualBoundaryColor.b);
            bool isFilled = (pixelColor.r == fillColor.r && 
                            pixelColor.g == fillColor.g && 
                            pixelColor.b == fillColor.b);
            
            return isBoundary || isFilled;
        };

        queue.push({seedX, seedY});
        visited.insert({seedX, seedY});

        while (!queue.empty())
        {
            auto [x, y] = queue.front();
            queue.pop();

            // 填充当前像素
            canvas.PutPixel(x, y, fillColor);
            if (onPixelFilled)
            {
                onPixelFilled(x, y);
            }

            // 检查四个方向的邻居
            int dx[] = {0, 1, 0, -1};
            int dy[] = {-1, 0, 1, 0};

            for (int i = 0; i < 4; ++i)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    auto key = std::make_pair(nx, ny);
                    if (visited.find(key) == visited.end() && !IsBoundaryOrFilled(nx, ny))
                    {
                        visited.insert(key);
                        queue.push(key);
                    }
                }
            }
        }
    }
}
