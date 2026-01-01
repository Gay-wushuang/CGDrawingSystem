#pragma once
#include <windows.h>
#include "Color.h"
#include <memory>

// Canvas：使用 DIBSection 实现 32bit ARGB 离屏像素缓冲
// 所有算法绘制最终都调用 PutPixel，保证是"自己画"的，而不是调用系统函数
class Canvas
{
public:
    Canvas();
    ~Canvas();

    // 禁止拷贝（管理资源）
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    // 调整画布大小（会重新创建 DIBSection）
    bool Resize(int width, int height);

    int Width() const { return m_width; }
    int Height() const { return m_height; }

    // 清空画布为指定颜色
    void Clear(Color color);

    // 在 (x,y) 位置绘制一个像素（核心绘制函数）
    // 注意：x,y 必须在 [0, width) x [0, height) 范围内
    void PutPixel(int x, int y, Color color);

    // 获取 (x,y) 位置像素的颜色
    Color GetPixel(int x, int y) const;

    // 在 (x,y) 位置绘制文字（可选功能）
    void DrawText(int x, int y, const wchar_t* text, COLORREF color);

    // 将离屏缓冲呈现到目标 HDC（通常在 WM_PAINT 中调用）
    void Present(HDC hdc);

    // 获取指向像素数据的指针（用于高级操作，如批量填充）
    uint32_t* GetPixelData() { return m_pBits; }

private:
    void Cleanup();

    int m_width  = 0;
    int m_height = 0;

    // DIBSection 相关句柄
    HDC      m_hMemDC = nullptr;
    HBITMAP  m_hBitmap = nullptr;
    HBITMAP  m_hOldBitmap = nullptr;

    // 像素数据指针（32bit ARGB，行主序）
    uint32_t* m_pBits = nullptr;

    // BITMAPINFO 结构（用于创建 DIBSection）
    BITMAPINFO m_bmi = {};
};


