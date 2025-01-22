// gui.h
// 创建GUI界面的相关函数

#pragma once
#include "gui.types.h"

#define ID_ICON_APP       101

#define GUI_TYPE_MAIN_WINDOW       0
#define GUI_TYPE_WIDGET            1
#define GUI_TYPE_BUTTON            2

#define GUI_CENTERX  0x80000000
#define GUI_CENTERY  0x40000000
#define GUI_LASTX    0x20000000
#define GUI_LASTY    0x10000000


// 创建主窗口
#define GUI_CreateWindow(...)                             \
    GUI_CreateWindowEx( &(GUI_WindowParams){              \
        .className = "syrie.yume.ui",                     \
        .bgColor = RGB(255,255,255),                      \
        .title = "",                                      \
        .style = WS_BORDER | WS_CAPTION | WS_SYSMENU,     \
        .x = CW_USEDEFAULT,                               \
        .y = CW_USEDEFAULT,                               \
        .isShow = TRUE,                                   \
        .alpha = 256,                                     \
        .opacityColor = 0x80000000,                       \
        __VA_ARGS__                                       \
    })

// 创建控件
#define GUI_CreateWidget(...)                             \
    GUI_CreateWindowEx( &(GUI_WindowParams){              \
        .type = GUI_TYPE_WIDGET,                          \
        .isShow = TRUE,                                   \
        .alpha = 256,                                     \
        .opacityColor = 0x80000000,                       \
        __VA_ARGS__                                       \
    })


// 窗口事件主循环
int GUI_MainLoop();

// 创建字体
HFONT GUI_CreateFont(const char *fontName, int fontSize, int weight);

// 尝试绘制文本，从而获得最终绘制的文本数量和大小
GUI_TypeText_Result GUI_TryTypeText(HDC hdc, char* text);

// 绘制文本 (逐字绘制)
// 注意颜色中 r、g、b 的值不能为 0
// flags可选 GUI_CENTERX, GUI_CENTERY, GUI_LASTX, GUI_LASTY
void GUI_TypeText(HWND hwnd, HDC hdc, int x, int y, int xMax, UINT32 flags, int interval, char *formatText, ...);

// 填充窗口
void GUI_FillWindow(HWND hwnd, RECT* rect, COLORREF color);

// 绘制图片
void GUI_DrawImage(HDC hdc, int x, int y, int width, int height, HBITMAP image, int stretchBltMode);

// 获取窗口宽度
int GUI_GetWindowWidth(HWND hwnd);

// 获取窗口高度
int GUI_GetWindowHeight(HWND hwnd);

// 窗口 淡入/ 淡出
int GUI_WindowFade(HWND hwnd, int alphaStart, int alphaEnd, int milliseconds);

// 对话框
void GUI_MsgBox(HWND hwnd, char* formatText, ...);

// 窗口是否被点击
BOOL GUI_IsClicked(HWND hwnd);

// 创建窗口
HWND GUI_CreateWindowEx(GUI_WindowParams *params);