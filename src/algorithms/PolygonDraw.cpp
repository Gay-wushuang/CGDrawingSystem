#include "PolygonDraw.h"
#include "LineBresenham.h"

namespace Algo
{
    // 多边形边界绘制：依次连接所有顶点
    void DrawPolygon(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color)
    {
        size_t count = xPoints.size();
        if (count < 2)
            return;

        if (count == 2)
        {
            // 只有两个点，画一条线
            DrawLineBresenham(canvas, xPoints[0], yPoints[0], xPoints[1], yPoints[1], color);
            return;
        }

        // 连接相邻顶点
        for (size_t i = 0; i < count - 1; ++i)
        {
            DrawLineBresenham(canvas, xPoints[i], yPoints[i], xPoints[i + 1], yPoints[i + 1], color);
        }

        // 连接最后一个顶点和第一个顶点（闭合）
        DrawLineBresenham(canvas, xPoints[count - 1], yPoints[count - 1], xPoints[0], yPoints[0], color);
    }
}
