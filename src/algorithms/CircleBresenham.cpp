#include "CircleBresenham.h"
#include <cmath>

namespace Algo
{
    // Bresenham 圆算法（八分对称）
    void DrawCircleBresenham(Canvas& canvas, int cx, int cy, int radius, Color color)
    {
        if (radius <= 0)
        {
            canvas.PutPixel(cx, cy, color);
            return;
        }

        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;

        // 绘制八个对称点
        auto Draw8Points = [&](int x, int y) {
            canvas.PutPixel(cx + x, cy + y, color);
            canvas.PutPixel(cx - x, cy + y, color);
            canvas.PutPixel(cx + x, cy - y, color);
            canvas.PutPixel(cx - x, cy - y, color);
            canvas.PutPixel(cx + y, cy + x, color);
            canvas.PutPixel(cx - y, cy + x, color);
            canvas.PutPixel(cx + y, cy - x, color);
            canvas.PutPixel(cx - y, cy - x, color);
        };

        Draw8Points(x, y);

        while (x < y)
        {
            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
            Draw8Points(x, y);
        }
    }
}
