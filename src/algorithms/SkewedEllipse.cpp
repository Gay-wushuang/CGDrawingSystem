#include "SkewedEllipse.h"
#include "PolygonDraw.h"
#include "../core/Canvas.h"
#include "../core/Color.h"
#include "../core/Math2D.h"
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Algo
{
    // 绘制倾斜椭圆（通过参数方程生成点）
    void DrawSkewedEllipse(Canvas& canvas, int cx, int cy, int a, int b, double rotation, Color color)
    {
        const int numPoints = 60; // 使用60个点来近似椭圆
        std::vector<int> xPoints, yPoints;
        xPoints.reserve(numPoints);
        yPoints.reserve(numPoints);
        
        double angleStep = 2.0 * M_PI / numPoints;
        
        for (int i = 0; i < numPoints; ++i)
        {
            double angle = i * angleStep;
            
            // 椭圆参数方程
            double x = a * cos(angle);
            double y = b * sin(angle);
            
            // 应用旋转变换
            double rotatedX = x * cos(rotation) - y * sin(rotation);
            double rotatedY = x * sin(rotation) + y * cos(rotation);
            
            // 转换到屏幕坐标
            xPoints.push_back(cx + (int)round(rotatedX));
            yPoints.push_back(cy + (int)round(rotatedY));
        }
        
        // 使用多边形绘制函数连接这些点
        DrawPolygon(canvas, xPoints, yPoints, color);
    }
    
    // 通过变换矩阵绘制倾斜椭圆
    void DrawTransformedEllipse(Canvas& canvas, int cx, int cy, int a, int b, const Matrix3& transform, Color color)
    {
        const int numPoints = 60; // 使用60个点来近似椭圆
        std::vector<int> xPoints, yPoints;
        xPoints.reserve(numPoints);
        yPoints.reserve(numPoints);
        
        double angleStep = 2.0 * M_PI / numPoints;
        
        for (int i = 0; i < numPoints; ++i)
        {
            double angle = i * angleStep;
            
            // 椭圆参数方程
            double x = a * cos(angle);
            double y = b * sin(angle);
            
            // 应用变换矩阵
            Vec2 point = transform.Transform(Vec2(x, y));
            
            // 转换到屏幕坐标（需要将椭圆中心作为变换的参考点）
            xPoints.push_back(cx + (int)round(point.x));
            yPoints.push_back(cy + (int)round(point.y));
        }
        
        // 使用多边形绘制函数连接这些点
        DrawPolygon(canvas, xPoints, yPoints, color);
    }
}