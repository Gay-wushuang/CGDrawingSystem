#include "BSplineCurve.h"
#include "LineBresenham.h"
#include <cmath>
#include <algorithm>

namespace Algo
{
    // 均匀周期 B 样条基函数（三次）
    static double B0(double t) { double u = 1.0 - t; return u * u * u / 6.0; }
    static double B1(double t) { return (3.0 * t * t * t - 6.0 * t * t + 4.0) / 6.0; }
    static double B2(double t) { return (-3.0 * t * t * t + 3.0 * t * t + 3.0 * t + 1.0) / 6.0; }
    static double B3(double t) { return t * t * t / 6.0; }

    void DrawBSplineCurve(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color)
    {
        size_t n = xPoints.size();
        if (n < 4)
        {
            // 控制点少于4个，用直线连接
            if (n >= 2)
            {
                for (size_t i = 0; i < n - 1; ++i)
                {
                    DrawLineBresenham(canvas, xPoints[i], yPoints[i], xPoints[i + 1], yPoints[i + 1], color);
                }
            }
            return;
        }

        // 均匀周期 B 样条：每个区间需要4个控制点
        const int samples = 50;  // 每个区间的采样点数

        for (size_t i = 0; i <= n - 4; ++i)
        {
            // 当前区间的4个控制点
            int p0x = xPoints[i], p0y = yPoints[i];
            int p1x = xPoints[i + 1], p1y = yPoints[i + 1];
            int p2x = xPoints[i + 2], p2y = yPoints[i + 2];
            int p3x = xPoints[i + 3], p3y = yPoints[i + 3];

            std::vector<int> curveX, curveY;
            curveX.reserve(samples + 1);
            curveY.reserve(samples + 1);

            for (int j = 0; j <= samples; ++j)
            {
                double t = (double)j / samples;
                
                // 计算 B 样条基函数
                double b0 = B0(t);
                double b1 = B1(t);
                double b2 = B2(t);
                double b3 = B3(t);

                // 计算曲线点
                double x = b0 * p0x + b1 * p1x + b2 * p2x + b3 * p3x;
                double y = b0 * p0y + b1 * p1y + b2 * p2y + b3 * p3y;

                curveX.push_back((int)round(x));
                curveY.push_back((int)round(y));
            }

            // 用直线连接采样点
            for (size_t j = 0; j < curveX.size() - 1; ++j)
            {
                DrawLineBresenham(canvas, curveX[j], curveY[j], curveX[j + 1], curveY[j + 1], color);
            }
        }
    }
}


