#pragma once
#include <windows.h>

// 统一的类型定义
struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x_, int y_) : x(x_), y(y_) {}
};