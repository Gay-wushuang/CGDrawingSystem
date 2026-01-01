#include "ClipLine.h"
#include "LineBresenham.h"
#include <cmath>

namespace Algo
{
    static OutCode ComputeOutCode(int x, int y, const ClipRect& rect)
    {
        OutCode code = OutCode::INSIDE;
        
        if (x < rect.xmin)
            code |= OutCode::LEFT;
        else if (x > rect.xmax)
            code |= OutCode::RIGHT;
        
        if (y < rect.ymin)
            code |= OutCode::BOTTOM;
        else if (y > rect.ymax)
            code |= OutCode::TOP;
        
        return code;
    }

    bool ClipLineCohenSutherland(int& x0, int& y0, int& x1, int& y1, const ClipRect& rect)
    {
        OutCode outcode0 = ComputeOutCode(x0, y0, rect);
        OutCode outcode1 = ComputeOutCode(x1, y1, rect);
        
        while (true)
        {
            // 完全在窗口内
            if ((outcode0 | outcode1) == OutCode::INSIDE)
                return true;
            
            // 完全在窗口外
            if ((outcode0 & outcode1) != OutCode::INSIDE)
                return false;
            
            // 选择在窗口外的点
            OutCode outcodeOut = (outcode0 != OutCode::INSIDE) ? outcode0 : outcode1;
            
            int x, y;
            
            // 计算交点（避免除零错误）
            if ((outcodeOut & OutCode::TOP) != OutCode::INSIDE)
            {
                if (y1 != y0)
                {
                    x = x0 + (int)round((double)(x1 - x0) * (rect.ymax - y0) / (y1 - y0));
                    y = rect.ymax;
                }
                else
                {
                    x = x0;
                    y = rect.ymax;
                }
            }
            else if ((outcodeOut & OutCode::BOTTOM) != OutCode::INSIDE)
            {
                if (y1 != y0)
                {
                    x = x0 + (int)round((double)(x1 - x0) * (rect.ymin - y0) / (y1 - y0));
                    y = rect.ymin;
                }
                else
                {
                    x = x0;
                    y = rect.ymin;
                }
            }
            else if ((outcodeOut & OutCode::RIGHT) != OutCode::INSIDE)
            {
                if (x1 != x0)
                {
                    y = y0 + (int)round((double)(y1 - y0) * (rect.xmax - x0) / (x1 - x0));
                    x = rect.xmax;
                }
                else
                {
                    y = y0;
                    x = rect.xmax;
                }
            }
            else // LEFT
            {
                if (x1 != x0)
                {
                    y = y0 + (int)round((double)(y1 - y0) * (rect.xmin - x0) / (x1 - x0));
                    x = rect.xmin;
                }
                else
                {
                    y = y0;
                    x = rect.xmin;
                }
            }
            
            // 更新被裁剪的点
            if (outcodeOut == outcode0)
            {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0, rect);
            }
            else
            {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1, rect);
            }
        }
    }

    void DrawClippedLine(Canvas& canvas, int x0, int y0, int x1, int y1, const ClipRect& rect, Color color)
    {
        int cx0 = x0, cy0 = y0, cx1 = x1, cy1 = y1;
        if (ClipLineCohenSutherland(cx0, cy0, cx1, cy1, rect))
        {
            DrawLineBresenham(canvas, cx0, cy0, cx1, cy1, color);
        }
    }
}
