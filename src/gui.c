#include <windows.h>
#include <commctrl.h>  // commctrl.h 的引入顺序必须在 windows.h 之后
#include <stdio.h>
#include <time.h>
#include "gui.h"
#include "utils.h"

// 默认窗口过程函数
LRESULT CALLBACK GUI_DefWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    GUI_WindowData *data;

    switch (msg) {
    
    case WM_PAINT:
        data = (GUI_WindowData*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
        if(data->onPaint)
            data->onPaint(hwnd, msg);
        break;

    case WM_DESTROY:
        data = (GUI_WindowData*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
        if(data->onDestroy)
            data->onDestroy(hwnd);

        if(data->type == GUI_TYPE_MAIN_WINDOW) {
            PostQuitMessage(0);
            exit(0);
        }

        free(data);
        return 0;
    
    case WM_LBUTTONUP:
        data = (GUI_WindowData*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
        data->lastClickedTime = clock();
        break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}



// 创建窗口
HWND GUI_CreateWindowEx(GUI_WindowParams *p) {
    // 获得当前程序实例
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // 注册窗口类
    WNDCLASSEXA wc;
    if(!GetClassInfoExA(hInstance, p->className, &wc)) {
        wc = (WNDCLASSEXA){
            .cbSize = sizeof(WNDCLASSEXA),
            .lpfnWndProc = GUI_DefWindowProc,
            .hInstance = hInstance,
            .lpszClassName = p->className,
            .hCursor = LoadCursorA(NULL, (p->cursorName) ? (p->cursorName) : IDC_ARROW),
            .hbrBackground = CreateSolidBrush(p->bgColor)
        };

        if(p->type == GUI_TYPE_MAIN_WINDOW) {
            // 初始化 Common Controls
            InitCommonControlsEx(&(INITCOMMONCONTROLSEX){ sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES });

            wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(ID_ICON_APP));
            wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCEA(ID_ICON_APP));
        }

        if(!RegisterClassExA(&wc))
            return NULL;
    }


    // 是否为带透明度的窗口
    DWORD layeredFlags = (p->alpha < 256 ? LWA_ALPHA: 0) |  (p->opacityColor != 0x80000000 ? LWA_COLORKEY: 0);
    if(layeredFlags) 
        p->styleEx |= WS_EX_LAYERED;

    // 创建窗口
    HWND hwnd = CreateWindowExA(p->styleEx, p->className, p->title, p->style, p->x, p->y, p->width, p->height, p->parent, NULL, hInstance, NULL);

    // 设置字体
    SendMessageA(hwnd, WM_SETFONT, (WPARAM)p->font, TRUE);

    // 自定义窗口数据
    GUI_WindowData *data = (GUI_WindowData*)malloc(sizeof(GUI_WindowData));
    *data = (GUI_WindowData){
        .type = p->type,
        .opacityColor = p->opacityColor,
        .onPaint = p->onPaint,
        .onDestroy = p->onDestroy
    };
    SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)data);

    if(layeredFlags)
        SetLayeredWindowAttributes(hwnd, p->opacityColor, p->alpha, layeredFlags);

    // 显示窗口
    if(p->isShow)
        ShowWindow(hwnd, SW_SHOWDEFAULT);

    return hwnd;
}



// 窗口事件主循环
int GUI_MainLoop() {
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return (int)msg.wParam;
}



// 创建字体
HFONT GUI_CreateFont(const char* fontName, int fontSize, int weight) {
    return CreateFontA(fontSize, 0, 0, 0, weight, FALSE, FALSE, FALSE,  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName); 
}



// 计算一个UTF-8字符占的位数(1-4)
int calcCharLength(char c) {
    if(c & 0x80) 
        if(c & 0x40)
            if(c & 0x20)
                if(c & 0x10)
                    return 4;
                else return 3;
            else return 2;
        else return 1;
    else return 1;
}



// 尝试绘制文本，从而获得最终绘制的文本数量和大小
GUI_TypeText_Result GUI_TryTypeText(HDC hdc, char* text) {
    static wchar_t textW[10];

    int count = 0;
    int width = 0; 
    
    SIZE textSize;
    int x = 0, y = 0;
    int byteCount;

    for(int i = 0; ;) {
        char c = text[i];

        if(c == '\0') { 
            width = width > x ? width : x;
            break;
        }

        if(c == '\xFC') 
            i += 4;

        else if(c == '\n') {
            width = width > x ? width : x;
            x = 0;
            y += textSize.cy * 5 / 4;
            i++;
        }

        else {
            byteCount = calcCharLength(c);
            int cTextW = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text + i, byteCount, textW, 10);
            GetTextExtentPoint32W(hdc, textW, cTextW, &textSize);
            
            x += textSize.cx;
            count++;
            i += byteCount;
        }
    }

    return (GUI_TypeText_Result){ .count = count, .width = width, .height = y + textSize.cy };
}


// 绘制文本 (逐字绘制)
// 颜色中 r、g、b 的值不能为 0
// flags可选 GUI_CENTERX, GUI_CENTERY, GUI_LASTX, GUI_LASTY
void GUI_TypeText(HWND hwnd, HDC hdc, int x, int y, int xMax, UINT32 flags, int interval, char* formatText, ...) {
    static char text[255];
    static wchar_t textW[10];
    static int lastX = 0, lastY = 0;

    va_list ap;
    va_start(ap, formatText);
    vsprintf(text, formatText, ap);
    va_end(ap);
    
    SIZE textSize;
    int byteCount;
    int xNow, yNow;

    if((flags & GUI_CENTERX) || (flags & GUI_CENTERY)) {
        GUI_TypeText_Result result = GUI_TryTypeText(hdc, text);
        xNow = (flags & GUI_CENTERX) ? (GUI_GetWindowWidth(hwnd)  - result.width) / 2 : x;
        yNow = (flags & GUI_CENTERY) ? (GUI_GetWindowHeight(hwnd) - result.height) / 2 : y;
    }

    else {
        xNow = (flags & GUI_LASTX) ? lastX : x;
        yNow = (flags & GUI_LASTY) ? lastY : y;
    }


    for(int i = 0; ; i += byteCount) {
        char c = text[i];

        if(c == '\0')
            break;

        if(c == '\xFC') {
            SetTextColor(hdc, RGB(text[i+1], text[i+2], text[i+3]));
            byteCount = 4;
            continue;
        }

        else if(c == '\n') {
            xNow = x;
            yNow += textSize.cy * 5 / 4;
            byteCount = 1;
            Sleep(interval);
            continue;
        }

        byteCount = calcCharLength(c);
        int cTextW = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text + i, byteCount, textW, 10);
        GetTextExtentPoint32W(hdc, textW, cTextW, &textSize);
        
        if(xNow + textSize.cx > xMax) {
            xNow = x;
            yNow += textSize.cy * 5 / 4;
        }

        TextOutW(hdc, xNow, yNow, textW, cTextW);
        InvalidateRect(hwnd, &(RECT){ xNow, yNow, xNow += textSize.cx, yNow + textSize.cy }, FALSE);
        UpdateWindow(hwnd);
        
        Sleep(interval);
    }

    lastX = xNow;
    lastY = yNow;
}



// 填充窗口
void GUI_FillWindow(HWND hwnd, RECT* rect, COLORREF color) {
    HDC hdc = GetDC(hwnd);
    HBRUSH brush = CreateSolidBrush(color);

    if(rect == NULL) { 
        RECT fullRect;
        GetClientRect(hwnd, &fullRect);
        FillRect(hdc, &fullRect, brush);
    }

    else FillRect(hdc, rect, brush);

    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
}



// 绘制图片
void GUI_DrawImage(HDC hdc, int x, int y, int width, int height, HBITMAP image, int stretchBltMode) {
    BITMAP bm;
    GetObject(image, sizeof(BITMAP), &bm);

    float scale = (float)height / bm.bmHeight;
    int scaledWidth = bm.bmWidth * scale;

    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, image);

    SetStretchBltMode(hdc, stretchBltMode == 0? STRETCH_HALFTONE : stretchBltMode);
    StretchBlt(hdc, (width - scaledWidth) / 2, 0, scaledWidth, height, (HDC)hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    DeleteDC(hdcMem);
}



// 获取窗口宽度
int GUI_GetWindowWidth(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    return rect.right - rect.left;
}

// 获取窗口高度
int GUI_GetWindowHeight(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    return rect.bottom - rect.top;
}



// 窗口 淡入/ 淡出
int GUI_WindowFade(HWND hwnd, int alphaStart, int alphaEnd, int milliseconds) {
    GUI_WindowData *data;
    COLORREF opacityColor = 0;
    DWORD layeredFlags = LWA_ALPHA;
    LONG styleEx = GetWindowLongA(hwnd, GWL_EXSTYLE);

    if(data = (GUI_WindowData*)GetWindowLongPtrA((HWND)hwnd, GWLP_USERDATA)) 
        layeredFlags |= (data->opacityColor != 0x80000000 ? LWA_COLORKEY: 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, styleEx | WS_EX_LAYERED);

    int step = (alphaEnd - alphaStart > 0)? 10 : -10;
    alphaStart += (alphaEnd - alphaStart) % 10;
    int interval = milliseconds * step / (alphaEnd - alphaStart);

    clock_t startTime = clock();
    clock_t nextTime = startTime + interval;
    
    foreach(var alpha in range(alphaStart, alphaEnd+step, step)) {
        SetLayeredWindowAttributes(hwnd, opacityColor, alpha, layeredFlags);
        if(clock() < nextTime)
            Sleep(interval);
        nextTime += interval;
    }

    SetWindowLongA(hwnd, GWL_EXSTYLE, styleEx);
}



// 对话框
void GUI_MsgBox(HWND hwnd, char* formatText, ...) {
    char text[255];

    va_list ap;
    va_start(ap, formatText);
    vsprintf(text, formatText, ap);
    va_end(ap);

    MessageBoxA(hwnd, text, "", MB_OK);
}



// 窗口是否被点击
BOOL GUI_IsClicked(HWND hwnd) {
    GUI_WindowData *data;

    if((data = (GUI_WindowData*)GetWindowLongPtrA(hwnd, GWLP_USERDATA)) && data->lastClickedTime) {
        if(clock() - data->lastClickedTime > 500) {
            data->lastClickedTime = 0;
            return FALSE;
        }

        else {
            data->lastClickedTime = 0;
            return TRUE;
        }
    }
    
    return FALSE;
}
