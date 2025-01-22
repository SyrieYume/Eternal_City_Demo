#include "gui.h"
#include "utils.h"

extern HWND hwnd_choice, hwnd_choiceBtns[2];
extern HFONT fonts[10];

// 普通按钮的绘制函数
void Game_NormalBtn_OnPaint(HWND hwnd, int msg) {
    static char szWindowTitle[MAX_PATH];
    
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT rect = ps.rcPaint;
    HBRUSH brush = CreateSolidBrush(RGB(24, 23, 28));
    HPEN pen = CreatePen(PS_SOLID, 4, RGB(240, 240, 240));

    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    SelectObject(hdc, fonts[7]);
    SetTextColor(hdc, RGB(240, 240, 240));
    SetBkMode(hdc, TRANSPARENT);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    GetWindowTextA(hwnd, szWindowTitle, MAX_PATH);
    GUI_TypeText_Result result = GUI_TryTypeText(hdc, szWindowTitle);
    TextOutA(hdc, rect.left + (width - result.width) / 2, rect.top + (height - result.height) / 2, szWindowTitle, GetWindowTextLengthA(hwnd));

    DeleteObject(brush);
    DeleteObject(pen);
    ReleaseDC(hwnd, hdc);
    EndPaint(hwnd, &ps);
}


// 对话选择按钮的绘制函数
void Game_ChoiceBtn_OnPaint(HWND hwnd, int msg) {
    static char szWindowTitle[MAX_PATH];
    
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT rect = ps.rcPaint;
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

    SelectObject(hdc, brush);
    SelectObject(hdc, fonts[4]);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    Polygon(hdc, (POINT[]) { 
        { rect.left + width / 15, rect.bottom },
        { rect.left, rect.top },
        { rect.right - width / 15, rect.top },
        { rect.right, rect.bottom },
     }, 4);

    GetWindowTextA(hwnd, szWindowTitle, MAX_PATH);
    TextOutA(hdc, rect.left + width * 2 / 15, rect.top + height * 2 / 10, szWindowTitle, GetWindowTextLengthA(hwnd));

    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
    EndPaint(hwnd, &ps);
}


// 对话选择
int Game_Choice(char *choices[], int choicesCount) {
    RECT rect;
    GetWindowRect(hwnd_choice, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    int btnWidth = width * 3 / 10;
    int btnHeight = height / 25;
    int x = width / 20;
    int y = height * 3 / 10;
    
    ShowWindow(hwnd_choice, SW_SHOWNORMAL);

    foreach(var i in range(2)) {
        SetWindowTextA(hwnd_choiceBtns[i], choices[i]);
        InvalidateRect(hwnd_choiceBtns[i], NULL, TRUE);
        UpdateWindow(hwnd_choiceBtns[i]);
    }
    
    GUI_WindowFade(hwnd_choice, 0, 160, 300);


    while(!GUI_IsClicked(hwnd_choiceBtns[0]) && !GUI_IsClicked(hwnd_choiceBtns[1]))
        Sleep(200);
    
    GUI_WindowFade(hwnd_choice, 160, 0, 300);
    ShowWindow(hwnd_choice, SW_HIDE);

    return 0;
}