#pragma once
#include <cmath>

// 2D 向量
struct Vec2
{
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    Vec2(double _x, double _y) : x(_x), y(_y) {}

    Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(double s) const { return Vec2(x / s, y / s); }

    double Length() const { return sqrt(x * x + y * y); }
    double LengthSq() const { return x * x + y * y; }
};

// 3x3 齐次坐标变换矩阵（行主序存储）
// 用于实现平移、旋转、缩放、错切、对称等变换
struct Matrix3
{
    double m[3][3];

    Matrix3()
    {
        // 初始化为单位矩阵
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0;
    }

    // 矩阵乘法：this = this * rhs
    Matrix3 operator*(const Matrix3& rhs) const;

    // 向量变换：将齐次坐标向量 [x, y, 1] 左乘矩阵
    Vec2 Transform(const Vec2& v) const;

    // 静态工厂方法：创建各种变换矩阵
    static Matrix3 Identity();
    static Matrix3 Translate(double tx, double ty);
    static Matrix3 Scale(double sx, double sy);
    static Matrix3 Scale(double sx, double sy, double cx, double cy);  // 以点(cx, cy)为中心缩放
    static Matrix3 Rotate(double angleRad);  // 绕原点逆时针旋转
    static Matrix3 Rotate(double angleRad, double cx, double cy);      // 以点(cx, cy)为中心旋转
    static Matrix3 Shear(double shx, double shy);
    static Matrix3 Shear(double shx, double shy, double cx, double cy); // 以点(cx, cy)为中心错切
    static Matrix3 ReflectX();  // 关于 x 轴对称
    static Matrix3 ReflectY();  // 关于 y 轴对称
    static Matrix3 ReflectOrigin();  // 关于原点对称
};

// 坐标转换工具：world 坐标系 <-> screen 坐标系
// world: 屏幕中心为 (0,0)，x 向右为正，y 向上为正
// screen: 屏幕左上角为 (0,0)，x 向右为正，y 向下为正
class CoordinateConverter
{
public:
    CoordinateConverter() = default;

    // 设置屏幕尺寸（通常在窗口大小改变时调用）
    void SetScreenSize(int screenWidth, int screenHeight);

    // world -> screen 坐标转换
    void WorldToScreen(const Vec2& world, int& screenX, int& screenY) const;

    // screen -> world 坐标转换
    void ScreenToWorld(int screenX, int screenY, Vec2& world) const;

    // 获取世界坐标系原点在屏幕上的位置
    void GetWorldOrigin(int& screenX, int& screenY) const;

private:
    int m_screenWidth = 0;
    int m_screenHeight = 0;
};


