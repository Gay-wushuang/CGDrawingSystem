#include "Canvas.h"
#include <cstring>
#include <cwchar>  // for wcslen

Canvas::Canvas()
{
    // 初始化 BITMAPINFO 结构
    ZeroMemory(&m_bmi, sizeof(BITMAPINFO));
    m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_bmi.bmiHeader.biPlanes = 1;
    m_bmi.bmiHeader.biBitCount = 32;  // 32bit ARGB
    m_bmi.bmiHeader.biCompression = BI_RGB;
}

Canvas::~Canvas()
{
    Cleanup();
}

void Canvas::Cleanup()
{
    if (m_hMemDC && m_hOldBitmap)
    {
        SelectObject(m_hMemDC, m_hOldBitmap);
        m_hOldBitmap = nullptr;
    }

    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
    }

    if (m_hMemDC)
    {
        DeleteDC(m_hMemDC);
        m_hMemDC = nullptr;
    }

    m_pBits = nullptr;
    m_width = 0;
    m_height = 0;
}

bool Canvas::Resize(int width, int height)
{
    if (width <= 0 || height <= 0)
        return false;

    // 如果尺寸没变，不需要重建
    if (width == m_width && height == m_height)
        return true;

    Cleanup();

    m_width = width;
    m_height = height;

    // 创建内存 DC
    HDC hScreenDC = GetDC(nullptr);
    if (!hScreenDC)
        return false;

    m_hMemDC = CreateCompatibleDC(hScreenDC);
    ReleaseDC(nullptr, hScreenDC);

    if (!m_hMemDC)
        return false;

    // 设置 BITMAPINFO 尺寸
    m_bmi.bmiHeader.biWidth = width;
    m_bmi.bmiHeader.biHeight = -height;  // 负值表示从上到下的行序

    // 创建 DIBSection（返回像素数据指针）
    m_hBitmap = CreateDIBSection(
        m_hMemDC,
        &m_bmi,
        DIB_RGB_COLORS,
        (void**)&m_pBits,
        nullptr,
        0);

    if (!m_hBitmap || !m_pBits)
    {
        Cleanup();
        return false;
    }

    // 将位图选入内存 DC
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

    // 初始化为白色
    Clear(Color::White());

    return true;
}

void Canvas::Clear(Color color)
{
    if (!m_pBits || m_width <= 0 || m_height <= 0)
        return;

    // 将颜色转换为 32bit BGRA 格式（Windows DIB 使用 BGRA 字节序，小端序）
    // 内存布局：低字节=Blue, 8-15位=Green, 16-23位=Red, 24-31位=Alpha
    uint32_t pixel = (color.b << 0) | (color.g << 8) | (color.r << 16) | (color.a << 24);

    // 批量填充整个缓冲区
    const int pixelCount = m_width * m_height;
    for (int i = 0; i < pixelCount; ++i)
    {
        m_pBits[i] = pixel;
    }
}

void Canvas::PutPixel(int x, int y, Color color)
{
    // 边界检查
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    if (!m_pBits)
        return;

    // 计算像素索引（注意：BITMAPINFO 中 biHeight 为负，所以 y 直接使用）
    int index = y * m_width + x;

    // 转换为 32bit BGRA（Windows DIB 使用 BGRA 字节序，小端序）
    // 内存布局：低字节=Blue, 8-15位=Green, 16-23位=Red, 24-31位=Alpha
    uint32_t pixel = (color.b << 0) | (color.g << 8) | (color.r << 16) | (color.a << 24);
    m_pBits[index] = pixel;
}

Color Canvas::GetPixel(int x, int y) const
{
    // 边界检查
    if (x < 0 || x >= m_width || y < 0 || y >= m_height || !m_pBits)
        return Color::Black();

    int index = y * m_width + x;
    uint32_t pixel = m_pBits[index];

    // 从 BGRA 提取颜色分量
    uint8_t b = (pixel >> 0) & 0xFF;
    uint8_t g = (pixel >> 8) & 0xFF;
    uint8_t r = (pixel >> 16) & 0xFF;
    uint8_t a = (pixel >> 24) & 0xFF;

    return Color(r, g, b, a);
}

void Canvas::DrawText(int x, int y, const wchar_t* text, COLORREF color)
{
    if (!m_hMemDC || !text)
        return;

    SetTextColor(m_hMemDC, color);
    SetBkMode(m_hMemDC, TRANSPARENT);
    TextOutW(m_hMemDC, x, y, text, (int)wcslen(text));
}

void Canvas::Present(HDC hdc)
{
    if (!m_hMemDC || !m_hBitmap || !hdc)
        return;

    // 使用 BitBlt 将离屏缓冲复制到目标 HDC
    BitBlt(
        hdc,
        0, 0,
        m_width, m_height,
        m_hMemDC,
        0, 0,
        SRCCOPY);
}
