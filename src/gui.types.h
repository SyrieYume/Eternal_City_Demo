#pragma once
#include <windows.h>
#include <time.h>

// 创建窗口时的参数
typedef struct {
    int type;
    char *className;
    char *title;
    int x, y;
    int width, height;
    COLORREF bgColor;
    DWORD style;
    DWORD styleEx;
    HFONT font;
    HWND parent;
    BOOL isShow;
    int alpha;
    COLORREF opacityColor;
    LPCSTR cursorName;
    void(*onDestroy)(HWND hwnd);
    void(*onPaint)(HWND hwnd, int msg);
} GUI_WindowParams;


// 窗口的自定义数据
typedef struct {
    int type;
    COLORREF opacityColor;
    clock_t lastClickedTime;
    void(*onDestroy)(HWND hwnd);
    void(*onPaint)(HWND hwnd, int msg);
} GUI_WindowData;


// GUI_TryTypeText返回的数据
typedef struct {
    int count;
    int width;
    int height;
} GUI_TypeText_Result;