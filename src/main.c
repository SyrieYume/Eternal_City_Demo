#include <stdio.h>
#include <time.h>
#include "gui.h"
#include "utils.h"

HWND window, hwnd_dialog, hwnd_bg, hwnd_text, hwnd_choice, hwnd_choiceBtns[2], hwnd_normalBtns[2];
HDC hdc_window, hdc_dialog, hdc_bg, hdc_text;
int screenWidth, screenHeight;
int winWidth = 1000, winHeight = 625;

HFONT fonts[10];
HBITMAP images[10];
char filepaths[10][MAX_PATH * 2];

extern BOOL Res_GetTempFileName(char *prefix, char *surfix, char *resultFilepath);
extern BOOL Res_LoadResourceToFile(int resourceId, char *filepath);
extern BOOL Res_LoadFontResource(int resourceId, char *fontName);

extern void Game_ChoiceBtn_OnPaint(HWND hwnd, int msg);
extern void Game_NormalBtn_OnPaint(HWND hwnd, int msg);

extern void page1();
extern void page2();
extern void page3();
extern void page4();
extern void page5();


// 主逻辑线程
DWORD MainThread(void* lpThreadParameter) {
    page1();
    page2();
    page3();
    page4();
    page5();
    return 0;
}


// 程序退出时清理资源
void onDestroy(HWND hwnd) {
    DeleteFileA(filepaths[0]);
    DeleteFileA(filepaths[1]);
    DeleteFileA(filepaths[2]);
    DeleteFileA(filepaths[3]);
}


// 程序初始化
BOOL onInitialize() {
    srand((unsigned)time(NULL));

    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    Res_LoadFontResource(501, "BigYoungMediumGB2.0");
    Res_LoadFontResource(502, "BigYoungBoldGB2.0");

    Res_GetTempFileName("f7d", ".mp3", filepaths[0]);
    Res_GetTempFileName("f7d", ".mp3", filepaths[1]);
    Res_GetTempFileName("f7d", ".mp3", filepaths[2]);
    Res_GetTempFileName("f7d", ".wmv", filepaths[3]);

    Res_LoadResourceToFile(301, filepaths[0]);
    Res_LoadResourceToFile(302, filepaths[1]);
    Res_LoadResourceToFile(303, filepaths[2]);
    Res_LoadResourceToFile(401, filepaths[3]);

    images[0] = (HBITMAP)LoadImageA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(201), IMAGE_BITMAP, 0, 0, 0);
    images[1] = (HBITMAP)LoadImageA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(203), IMAGE_BITMAP, 0, 0, 0);
    images[2] = (HBITMAP)LoadImageA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(202), IMAGE_BITMAP, 0, 0, 0);

    fonts[0] = GUI_CreateFont("BigYoungMediumGB2.0", winHeight / 22, FW_NORMAL);
    fonts[1] = GUI_CreateFont("SimHei", screenHeight / 10, FW_NORMAL);
    fonts[2] = GUI_CreateFont("BigYoungMediumGB2.0", screenHeight * 15 / 400, FW_NORMAL);
    fonts[3] = GUI_CreateFont("BigYoungBoldGB2.0", screenHeight * 15 / 400, FW_BOLD);
    fonts[4] = GUI_CreateFont("BigYoungBoldGB2.0", screenHeight * 3 / 100, FW_NORMAL);
    fonts[5] = GUI_CreateFont("BigYoungBoldGB2.0", screenHeight / 25, FW_NORMAL);
    fonts[6] = GUI_CreateFont("BigYoungBoldGB2.0", screenHeight / 30, FW_NORMAL);
    fonts[7] = GUI_CreateFont("BigYoungBoldGB2.0", winHeight / 20, FW_NORMAL);

    // 主窗口
    window = GUI_CreateWindow(
        .title = "Eternal City",
        .x = (screenWidth - winWidth) / 2, 
        .y = (screenHeight - winHeight) / 2,
        .width = winWidth, 
        .height = winHeight,
        .onDestroy = onDestroy
    );

    // 背景
    hwnd_bg = GUI_CreateWidget(
        .className = "f7d.background",
        .x = 0,
        .y = 0,
        .width = screenWidth,
        .height = screenHeight,
        .parent = window,
        .alpha = 0,
        .style = WS_POPUP
    );

    // 对话选择
    hwnd_choice = GUI_CreateWidget(
        .className = "f7d.choices",
        .x = 0,
        .y = 0,
        .width = screenWidth,
        .height = screenHeight,
        .parent = hwnd_bg,
        .isShow = FALSE,
        .alpha = 0,
        .bgColor = RGB(0, 0, 0),
        .style = WS_POPUP
    );

    // 对话框
    hwnd_dialog = GUI_CreateWidget(
        .className = "f7d.dialog",
        .x = 0,
        .y = screenHeight * 14 / 20,
        .width = screenWidth,
        .height = screenHeight * 5 / 20,
        .parent = hwnd_choice,
        .alpha = 0,
        .opacityColor = RGB(0, 0, 0),
        .bgColor = RGB(0, 0, 0),
        .style = WS_POPUP
    );

    // 文字层
    hwnd_text = GUI_CreateWidget(
        .className = "f7d.text",
        .x = 0, 
        .y = 0,
        .width = screenWidth, 
        .height = screenHeight,
        .parent = hwnd_dialog,
        .opacityColor = RGB(128, 128, 128),
        .bgColor = RGB(128, 128, 128),
        .style = WS_POPUP
    );

    foreach(var i in range(2)) {
        // 对话选择中的按钮
        hwnd_choiceBtns[i] = GUI_CreateWidget(
            .className = "f7d.btn1",
            .x = screenWidth / 20 + i * screenWidth / 50,
            .y = screenHeight * 3 / 10 + i * screenHeight * 2 / 25,
            .width = screenWidth * 3 / 10,
            .height = screenHeight / 25,
            .parent = hwnd_choice,
            .cursorName = IDC_HAND,
            .opacityColor = RGB(128, 128, 128),
            .bgColor = RGB(128, 128, 128),
            .style = WS_CHILD,
            .onPaint = Game_ChoiceBtn_OnPaint
        ); 

        // 查看Wiki和进入官网的按钮
        hwnd_normalBtns[i] = GUI_CreateWidget(
            .className = "f7d.btn2",
            .title = ((char*[]){ "查看Wiki", "进入官网" })[i],
            .x = winWidth - winHeight * 8 / 20,
            .y = winHeight - (i + 2) * winHeight * 4 / 40,
            .width = winHeight * 6 / 20,
            .height = winHeight * 3 / 40,
            .parent = window,
            .cursorName = IDC_HAND,
            .isShow = FALSE,
            .style = WS_CHILD,
            .onPaint = Game_NormalBtn_OnPaint
        );
    }

    hdc_window = GetDC(window);
    hdc_dialog = GetDC(hwnd_dialog);
    hdc_bg = GetDC(hwnd_bg);
    hdc_text = GetDC(hwnd_text);

    SetBkMode(hdc_window, TRANSPARENT);
    SetBkMode(hdc_text, TRANSPARENT);

    GUI_DrawImage(hdc_dialog, 0, 0, GUI_GetWindowWidth(hwnd_dialog), GUI_GetWindowHeight(hwnd_dialog), images[1], STRETCH_ORSCANS);

    return window && hwnd_bg && hwnd_choice && hwnd_dialog && hwnd_text;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if(!onInitialize()) {
        MessageBoxA(NULL, "程序初始化失败！", "Error", MB_OK);
        exit(-1);
    }

    SetFocus(window);
    CreateThread(NULL, 0, MainThread, NULL, 0, NULL);

    return GUI_MainLoop();
}