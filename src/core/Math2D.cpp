#include "Math2D.h"
#include <cmath>

// Matrix3 矩阵乘法
Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            result.m[i][j] = 0.0;
            for (int k = 0; k < 3; ++k)
            {
                result.m[i][j] += m[i][k] * rhs.m[k][j];
            }
        }
    }
    return result;
}

// 向量变换（齐次坐标：v = [x, y, 1]）
Vec2 Matrix3::Transform(const Vec2& v) const
{
    double x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    double y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    return Vec2(x, y);
}

Matrix3 Matrix3::Identity()
{
    return Matrix3();
}

Matrix3 Matrix3::Translate(double tx, double ty)
{
    Matrix3 mat;
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

Matrix3 Matrix3::Scale(double sx, double sy)
{
    Matrix3 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    return mat;
}

Matrix3 Matrix3::Scale(double sx, double sy, double cx, double cy)
{
    // T(cx, cy) * S(sx, sy) * T(-cx, -cy)
    Matrix3 translateToOrigin = Translate(-cx, -cy);
    Matrix3 scale = Scale(sx, sy);
    Matrix3 translateBack = Translate(cx, cy);
    
    return translateBack * scale * translateToOrigin;
}

Matrix3 Matrix3::Rotate(double angleRad)
{
    Matrix3 mat;
    double c = cos(angleRad);
    double s = sin(angleRad);
    mat.m[0][0] = c;  mat.m[0][1] = -s;
    mat.m[1][0] = s;  mat.m[1][1] = c;
    return mat;
}

Matrix3 Matrix3::Rotate(double angleRad, double cx, double cy)
{
    // T(cx, cy) * R(angle) * T(-cx, -cy)
    Matrix3 translateToOrigin = Translate(-cx, -cy);
    Matrix3 rotate = Rotate(angleRad);
    Matrix3 translateBack = Translate(cx, cy);
    
    return translateBack * rotate * translateToOrigin;
}

Matrix3 Matrix3::Shear(double shx, double shy)
{
    Matrix3 mat;
    mat.m[0][1] = shx;
    mat.m[1][0] = shy;
    return mat;
}

Matrix3 Matrix3::Shear(double shx, double shy, double cx, double cy)
{
    // T(cx, cy) * Sh(shx, shy) * T(-cx, -cy)
    Matrix3 translateToOrigin = Translate(-cx, -cy);
    Matrix3 shear = Shear(shx, shy);
    Matrix3 translateBack = Translate(cx, cy);
    
    return translateBack * shear * translateToOrigin;
}

Matrix3 Matrix3::ReflectX()
{
    Matrix3 mat;
    mat.m[1][1] = -1.0;
    return mat;
}

Matrix3 Matrix3::ReflectY()
{
    Matrix3 mat;
    mat.m[0][0] = -1.0;
    return mat;
}

Matrix3 Matrix3::ReflectOrigin()
{
    Matrix3 mat;
    mat.m[0][0] = -1.0;
    mat.m[1][1] = -1.0;
    return mat;
}

// CoordinateConverter 实现
void CoordinateConverter::SetScreenSize(int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void CoordinateConverter::WorldToScreen(const Vec2& world, int& screenX, int& screenY) const
{
    // world 原点在屏幕中心
    int originX = m_screenWidth / 2;
    int originY = m_screenHeight / 2;

    // world: x 向右为正，y 向上为正
    // screen: x 向右为正，y 向下为正
    screenX = originX + (int)world.x;
    screenY = originY - (int)world.y;  // y 轴翻转
}

void CoordinateConverter::ScreenToWorld(int screenX, int screenY, Vec2& world) const
{
    int originX = m_screenWidth / 2;
    int originY = m_screenHeight / 2;

    world.x = (double)(screenX - originX);
    world.y = (double)(originY - screenY);  // y 轴翻转
}

void CoordinateConverter::GetWorldOrigin(int& screenX, int& screenY) const
{
    screenX = m_screenWidth / 2;
    screenY = m_screenHeight / 2;
}
