#include "LineBresenham.h"
#include <cmath>
#include <algorithm>

namespace Algo
{
    // Bresenham 直线算法（任意斜率）
    void DrawLineBresenham(Canvas& canvas, int x0, int y0, int x1, int y1, Color color)
    {
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int x = x0;
        int y = y0;

        while (true)
        {
            canvas.PutPixel(x, y, color);

            if (x == x1 && y == y1)
                break;

            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y += sy;
            }
        }
    }
}
