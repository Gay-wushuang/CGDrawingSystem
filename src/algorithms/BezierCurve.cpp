#include "BezierCurve.h"
#include "LineBresenham.h"
#include <cmath>
#include <algorithm>

namespace Algo
{
    // 计算 Bezier 曲线上的点（de Casteljau 算法）
    void DrawBezierCurve(Canvas& canvas, const std::vector<int>& xPoints, const std::vector<int>& yPoints, Color color)
    {
        size_t n = xPoints.size();
        if (n < 2)
            return;

        if (n == 2)
        {
            // 一次 Bezier（直线）
            DrawLineBresenham(canvas, xPoints[0], yPoints[0], xPoints[1], yPoints[1], color);
            return;
        }

        // 采样参数 t 从 0 到 1
        const int samples = 100;  // 采样点数
        std::vector<int> curveX, curveY;
        curveX.reserve(samples + 1);
        curveY.reserve(samples + 1);

        for (int i = 0; i <= samples; ++i)
        {
            double t = (double)i / samples;
            
            // de Casteljau 算法计算 Bezier 点
            std::vector<double> x(n), y(n);
            for (size_t j = 0; j < n; ++j)
            {
                x[j] = xPoints[j];
                y[j] = yPoints[j];
            }

            // 迭代计算
            for (size_t level = n - 1; level > 0; --level)
            {
                for (size_t j = 0; j < level; ++j)
                {
                    x[j] = x[j] * (1.0 - t) + x[j + 1] * t;
                    y[j] = y[j] * (1.0 - t) + y[j + 1] * t;
                }
            }

            curveX.push_back((int)round(x[0]));
            curveY.push_back((int)round(y[0]));
        }

        // 用直线连接采样点
        for (size_t i = 0; i < curveX.size() - 1; ++i)
        {
            DrawLineBresenham(canvas, curveX[i], curveY[i], curveX[i + 1], curveY[i + 1], color);
        }
    }
}


