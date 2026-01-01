#pragma once
#include "../core/Canvas.h"
#include "../core/Color.h"
#include <queue>
#include <vector>
#include <functional>

namespace Algo
{
    // 种子填充算法（队列 BFS）
    // 使用回调函数实现逐步演示
    // boundaryColor: 边界颜色（如果为 Color(-1,-1,-1) 则使用种子点颜色作为边界）
    void FillSeed(Canvas& canvas, 
        int seedX, int seedY, 
        Color fillColor, 
        Color boundaryColor = Color(-1, -1, -1),  // -1 表示使用种子点颜色
        std::function<void(int x, int y)> onPixelFilled = nullptr);  // 每填充一个像素的回调
}
