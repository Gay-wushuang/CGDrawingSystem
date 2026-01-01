#include "LineDDA.h"
#include <cmath>
#include <algorithm>

namespace Algo
{
    // DDA 直线算法（Digital Differential Analyzer）
    // 支持四象限直线绘制
    void DrawLineDDA(Canvas& canvas, int x0, int y0, int x1, int y1, Color color)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int steps = (std::max)(abs(dx), abs(dy));  // 取绝对值较大的作为步数

        if (steps == 0)
        {
            // 起点和终点相同，只画一个点
            canvas.PutPixel(x0, y0, color);
            return;
        }

        // 计算每一步的增量
        float xIncrement = (float)dx / steps;
        float yIncrement = (float)dy / steps;

        // 从起点开始，逐步绘制
        float x = (float)x0;
        float y = (float)y0;

        for (int i = 0; i <= steps; ++i)
        {
            canvas.PutPixel((int)round(x), (int)round(y), color);
            x += xIncrement;
            y += yIncrement;
        }
    }
}
