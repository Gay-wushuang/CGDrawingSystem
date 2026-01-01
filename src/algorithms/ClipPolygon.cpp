#include "ClipPolygon.h"
#include "PolygonDraw.h"
#include <algorithm>
#include <cmath>

namespace Algo
{
    // 裁剪一条边（Sutherland-Hodgman 算法的一部分）
    static void ClipEdge(const std::vector<std::pair<int, int>>& input,
                         std::vector<std::pair<int, int>>& output,
                         int edge, const ClipRect& rect)
    {
        output.clear();
        if (input.empty())
            return;

        std::pair<int, int> s = input.back();
        
        for (const auto& p : input)
        {
            bool sInside = false, pInside = false;
            int x, y;
            
            // 判断点是否在裁剪边内侧
            switch (edge)
            {
            case 0: // 左边界
                sInside = s.first >= rect.xmin;
                pInside = p.first >= rect.xmin;
                x = rect.xmin;
                if (p.first != s.first)
                    y = s.second + (int)round((double)(p.second - s.second) * (rect.xmin - s.first) / (p.first - s.first));
                else
                    y = s.second;
                break;
            case 1: // 右边界
                sInside = s.first <= rect.xmax;
                pInside = p.first <= rect.xmax;
                x = rect.xmax;
                if (p.first != s.first)
                    y = s.second + (int)round((double)(p.second - s.second) * (rect.xmax - s.first) / (p.first - s.first));
                else
                    y = s.second;
                break;
            case 2: // 下边界
                sInside = s.second >= rect.ymin;
                pInside = p.second >= rect.ymin;
                y = rect.ymin;
                if (p.second != s.second)
                    x = s.first + (int)round((double)(p.first - s.first) * (rect.ymin - s.second) / (p.second - s.second));
                else
                    x = s.first;
                break;
            case 3: // 上边界
                sInside = s.second <= rect.ymax;
                pInside = p.second <= rect.ymax;
                y = rect.ymax;
                if (p.second != s.second)
                    x = s.first + (int)round((double)(p.first - s.first) * (rect.ymax - s.second) / (p.second - s.second));
                else
                    x = s.first;
                break;
            }
            
            if (pInside)
            {
                if (!sInside)
                {
                    output.push_back({x, y});
                }
                output.push_back(p);
            }
            else if (sInside)
            {
                output.push_back({x, y});
            }
            
            s = p;
        }
    }

    std::vector<std::pair<int, int>> ClipPolygonSutherlandHodgman(
        const std::vector<int>& xPoints,
        const std::vector<int>& yPoints,
        const ClipRect& rect)
    {
        if (xPoints.size() < 3)
            return {};

        std::vector<std::pair<int, int>> result;
        for (size_t i = 0; i < xPoints.size(); ++i)
        {
            result.push_back({xPoints[i], yPoints[i]});
        }

        std::vector<std::pair<int, int>> temp;
        
        // 依次对四条边进行裁剪
        for (int edge = 0; edge < 4; ++edge)
        {
            ClipEdge(result, temp, edge, rect);
            result = temp;
            if (result.empty())
                break;
        }

        return result;
    }

    void DrawClippedPolygon(Canvas& canvas,
        const std::vector<int>& xPoints,
        const std::vector<int>& yPoints,
        const ClipRect& rect,
        Color color)
    {
        auto clipped = ClipPolygonSutherlandHodgman(xPoints, yPoints, rect);
        if (clipped.size() < 2)
            return;

        std::vector<int> cx, cy;
        for (const auto& p : clipped)
        {
            cx.push_back(p.first);
            cy.push_back(p.second);
        }
        
        DrawPolygon(canvas, cx, cy, color);
    }
}
