#include "CoordinateAxis.h"
#include "Color.h"

// 使用 Bresenham 直线算法绘制轴线（保证是"自己画"的）
static void DrawLine(Canvas& canvas, int x0, int y0, int x1, int y1, Color color)
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

void CoordinateAxis::Draw(Canvas& canvas) const
{
    const int width = canvas.Width();
    const int height = canvas.Height();

    if (width <= 0 || height <= 0)
        return;

    const int cx = width / 2;
    const int cy = height / 2;

    Color axisColor = Color::Black();

    // 绘制 x 轴（从左到右）
    DrawLine(canvas, 0, cy, width - 1, cy, axisColor);

    // x 轴箭头（右侧）
    DrawLine(canvas, width - 1, cy, width - 11, cy - 5, axisColor);
    DrawLine(canvas, width - 1, cy, width - 11, cy + 5, axisColor);

    // 绘制 y 轴（从下到上，注意屏幕坐标系 y 向下）
    DrawLine(canvas, cx, height - 1, cx, 0, axisColor);

    // y 轴箭头（顶部）
    DrawLine(canvas, cx, 0, cx - 5, 10, axisColor);
    DrawLine(canvas, cx, 0, cx + 5, 10, axisColor);

    // 标注文字：O, x, y（使用 Canvas 的 DrawText）
    canvas.DrawText(cx + 4, cy + 4, L"O", RGB(0, 0, 0));
    canvas.DrawText(width - 20, cy + 4, L"x", RGB(0, 0, 0));
    canvas.DrawText(cx + 4, 4, L"y", RGB(0, 0, 0));
}


