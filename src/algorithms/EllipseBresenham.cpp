#include "EllipseBresenham.h"
#include <cmath>

namespace Algo
{
    // Bresenham 椭圆算法（四分对称）
    void DrawEllipseBresenham(Canvas& canvas, int cx, int cy, int a, int b, Color color)
    {
        if (a <= 0 || b <= 0)
        {
            canvas.PutPixel(cx, cy, color);
            return;
        }

        // 绘制四个对称点
        auto Draw4Points = [&](int x, int y) {
            canvas.PutPixel(cx + x, cy + y, color);
            canvas.PutPixel(cx - x, cy + y, color);
            canvas.PutPixel(cx + x, cy - y, color);
            canvas.PutPixel(cx - x, cy - y, color);
        };

        int x = 0;
        int y = b;
        long long a2 = (long long)a * a;
        long long b2 = (long long)b * b;
        long long d1 = b2 - a2 * b + a2 / 4;

        // 第一区域：斜率绝对值 < 1
        while (b2 * x < a2 * y)
        {
            Draw4Points(x, y);

            if (d1 < 0)
            {
                d1 = d1 + b2 * (2 * x + 3);
            }
            else
            {
                d1 = d1 + b2 * (2 * x + 3) + a2 * (-2 * y + 2);
                y--;
            }
            x++;
        }

        // 第二区域：斜率绝对值 >= 1
        long long d2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
        while (y >= 0)
        {
            Draw4Points(x, y);

            if (d2 < 0)
            {
                d2 = d2 + b2 * (2 * x + 2) + a2 * (-2 * y + 3);
                x++;
            }
            else
            {
                d2 = d2 + a2 * (-2 * y + 3);
            }
            y--;
        }
    }
}
