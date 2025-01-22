#include "gui.h"
#include "utils.h"

extern HWND window, hwnd_dialog, hwnd_bg, hwnd_text, hwnd_choice, hwnd_normalBtns[2]; 
extern HDC hdc_window, hdc_dialog, hdc_bg, hdc_text;    
extern int screenWidth, screenHeight;
extern int winWidth, winHeight;

extern HFONT fonts[10];                   
extern HBITMAP images[10];
extern char filepaths[10][MAX_PATH * 2];

extern char* Media_Command(const char *formatCommand, ...);
extern BOOL Media_IsPlaying(char *name);
extern int Media_Position(char *name);
extern int Game_Choice(char *choices[], int choicesCount);


void page1() {
    int x = winWidth / 8;
    int y = winHeight * 4 / 22;
    float residueRate = (float)rand() / RAND_MAX;

    GUI_FillWindow(window, NULL, RGB(255,255,255));
    Sleep(500);
    
    SelectObject(hdc_window, fonts[0]);

    GUI_TypeText(window, hdc_window, x, y, screenWidth, 0, 80, 
        "\xFC\x0A\x0A\x0A" "「神器使」生命值过低....已确认死亡\n"
        "本次挑战记录-完成\n\n\n"

        "搜寻幸存者....\n"
        "检测到残余率上升，目前为" "\xFC\xDC\x1D\x1D" "%1.7f%%\n\n"

        "\xFC\x64\x64\x64" "开始删除实时数据....\n"
    , residueRate);

    Sleep(1000);
    GUI_FillWindow(window, NULL, RGB(255,255,255));

    GUI_TypeText(window, hdc_window, x, y, screenWidth, 0, 80, 
        "\xFC\x0A\x0A\x0A" "新数据重登录中....\n\n"
        
        "全新系统时间已准备完成，\n"
        "时间设置为 7 天\n"
        "新系统已准备完毕，启动中。\n\n"

        "\xFC\xF0\x1D\x1D" "注意：残余率会影响后续运行，请及时清理。\n"
    );

    Sleep(1500);
}


void page2() {
    Sleep(500);

    SetLayeredWindowAttributes(window, 0, 0, LWA_ALPHA);
    
    SetWindowLongA(window, GWL_STYLE, WS_POPUP);
    SetWindowPos(window, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
    GUI_FillWindow(window, NULL, RGB(0,0,0));

    GUI_WindowFade(window, 0, 255, 1400);
    
    SelectObject(hdc_window, fonts[1]);
    GUI_TypeText(window, hdc_window, 0, 0, screenWidth, GUI_CENTERX | GUI_CENTERY, 100, "\xFC\xFF\xFF\xFF" "「世界重构......开始」 ");

    Sleep(2000);
}


void page3() {
    Media_Command("open \"%s\" alias bgm1.mp3 wait", filepaths[0]);
    Media_Command("open \"%s\" alias voices.mp3 wait", filepaths[1]);

    GUI_DrawImage(hdc_bg, 0, 0, screenWidth, screenHeight, images[0], STRETCH_HALFTONE);
    
    Media_Command("setaudio bgm1.mp3 volume to 600");
    Media_Command("play bgm1.mp3 repeat");

    GUI_WindowFade(hwnd_bg, 0, 255, 2000);
    GUI_WindowFade(hwnd_dialog, 0, 230, 1000);
    GUI_FillWindow(window, NULL, RGB(0,0,0));

    int x = screenWidth / 2 - screenHeight * 8 / 15;
    int y = screenHeight * 248 / 320;
    RECT dialogRect = { x, y, screenWidth / 2 + screenHeight * 8 / 15, y + screenHeight * 50 / 320 };

    GUI_FillWindow(hwnd_text, &dialogRect, RGB(128, 128, 128));

    SelectObject(hdc_text, fonts[2]);
    GUI_TypeText(hwnd_text, hdc_text, x, y - screenHeight * 19 / 320, screenWidth, 0, 10, "\xFC\xFA\xFA\xFA" "神秘的少女");

    SelectObject(hdc_text, fonts[3]);
    SetTextColor(hdc_text, RGB(55, 57, 66));

    #define COMMAND_NULL   0x0
    #define COMMAND_CLEAR  0x80000000
    #define COMMAND_CHOICE 0x40000000

    struct Script1 { int start,end; char *text; UINT64 command, data; } scripts[] = {
        {  1580,  3900, "欢迎光临，闯入者。", COMMAND_CLEAR, 0 },
        {  3932,  7066, "不要紧张，我不会赶你出去。\n", COMMAND_NULL, 0 },
        {  7066, 11842, "只是想从你这里得到一些回答罢了。", COMMAND_CLEAR, 0 },
        { 11842, 13527, "你是否有过这样的感觉。\n", COMMAND_NULL, 0 },
        { 13973, 22656, "看到一个陌生的情景，却觉得之前好像发生过一样。", COMMAND_CHOICE, (UINT64)(char*[]){ "是的，有过", "不，从来没有。" } },

        { 23822, 29837, "是了，你一定听说过「既视感」是什么的吧。\n", COMMAND_NULL, 0 },
        { 29914, 35097, "所谓似曾相识的感觉，只是源自于大脑的「错觉」。", COMMAND_CLEAR, 0 },
        { 35097, 38630, "但你真的以为那只是错觉吗？", COMMAND_CHOICE, (UINT64)(char*[]){ "是，我很确定。", "...也许吧？我不知道" } },

        { 41674, 45550, "假如你确实地来过这里。\n", COMMAND_NULL, 0 },
        { 45720, 48798, "只是将一切遗忘了。", COMMAND_CLEAR, 0 },        
        { 49188, 53150, "假如你确实地做过某些事情。\n", COMMAND_NULL, 0 },
        { 53150, 56234, "只是将一切遗忘了。", COMMAND_CLEAR, 0 },        
        { 59279, 60267, "你，", COMMAND_NULL, 0 },
        { 60267, 63938, "会为那些早已被遗忘的错误...\n", COMMAND_NULL, 0 },
        { 63938, 65597, "赎罪吗？", COMMAND_CHOICE, (UINT64)(char*[]){ "会", "不会" } },

        { 69205, 79258, "但又怎么确定，遗忘的是真的记忆，不是错觉呢？", COMMAND_CLEAR, 0 },  
        { 79258, 81337, "说到底....", COMMAND_CLEAR, 0 },  
        { 81337, 83850, "亲身经历的就是「真相」吗？\n", COMMAND_NULL, 0 },  
        { 83850, 87518, "会被遗忘的就是「记忆」吗？", COMMAND_NULL, 0 },  
    };

    UINT typeMode = 0;
    foreach(var i in range(len(scripts))) {
        struct Script1 s = scripts[i];
        Media_Command("play voices.mp3 from %d to %d", s.start, s.end);
        GUI_TypeText(hwnd_text, hdc_text, x, y, dialogRect.right, typeMode, 45, s.text);
        
        while(Media_IsPlaying("voices.mp3") && !GUI_IsClicked(hwnd_bg) && !GUI_IsClicked(hwnd_dialog))
            Sleep(200);

        if(s.command & COMMAND_CHOICE) 
            Game_Choice((char**)s.data, 2);

        if(s.command & (COMMAND_CLEAR | COMMAND_CHOICE)) {
            GUI_FillWindow(hwnd_text, &dialogRect, RGB(128, 128, 128));
            typeMode = 0;
        }
        else typeMode = GUI_LASTX | GUI_LASTY;
    }
    
    GUI_FillWindow(hwnd_text, NULL, RGB(128, 128, 128));
    ShowWindow(hwnd_dialog, SW_HIDE);
    GUI_WindowFade(hwnd_bg, 255, 0, 1000);

    Sleep(1000);
    SelectObject(hdc_window, fonts[5]);

    Media_Command("play voices.mp3 from 88184 to 90711");
    GUI_TypeText(window, hdc_window, 0, 0, screenWidth, GUI_CENTERX | GUI_CENTERY, 50, "\xFC\xFF\xFF\xFF" "——试着想起来吧。");
    while(Media_IsPlaying("voices.mp3")) Sleep(200);
    
    Media_Command("play voices.mp3 from 90711 to 92718");
    GUI_FillWindow(window, NULL, RGB(0,0,0));
    GUI_TypeText(window, hdc_window, 0, 0, screenWidth, GUI_CENTERX | GUI_CENTERY, 50, "\xFC\xDC\x1D\x1D" "在这个世界毁灭之前。");
    while(Media_IsPlaying("voices.mp3")) Sleep(200);

    Sleep(1000);
    Media_Command("close voices.mp3");
    Media_Command("close bgm1.mp3");
}


void page4() {
    int videoWidth = screenHeight * 1920 / 1080;
    SetWindowPos(hwnd_bg, 0, (screenWidth - videoWidth) / 2, 0, videoWidth, screenHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
    
    GUI_FillWindow(window, NULL, RGB(0,0,0));
    
    Media_Command("open \"%s\" alias pv.wmv wait", filepaths[3]);
    Media_Command("window pv.wmv handle %lld", hwnd_bg);

    Media_Command("put pv.wmv destination at %d %d %d %d", 0, 0, videoWidth, screenHeight);

    struct Subtitle { int start,end; char *text; } subtitles[] = {
        { 1333,   5666, "「似曾相识」的话你一定听说过吧" },
        { 7333,  10200, "所谓的「既视感」" },
        { 10333, 13666, "其实只是源自于大脑的错觉" },
        { 15333, 17100, "但是" },
        { 18000, 21450, "你真的认为那只是错觉吗？" },
        { 23000, 27400, "拜托你一定要拯救这个世界" },
        { 29900, 32300, "如果将这个世界上千千万万的人" },
        { 32300, 37500, "和仅存于此的几十位神器使同时衡量" },
        { 38000, 40700, "我会毫不犹豫地选择前者" },
        { 42300, 45000, "这就是让这个人为此牺牲的理由吗" },
        { 45500, 47600, "那这个世界毁灭了也无所谓！" },
        { 51700, 54333, "明明 保护好了所有人" },
        { 54800, 58700, "为什么却只有「   」" },
        { 58800, 64200, "不要 来 救我" },
        { 64600, 68000, "虽然希望渺小，但不需要感到沮丧" },
        { 68666, 70200, "为什么" },
        { 70500, 72450, "世界是一个轮回" },
        { 72750, 75900, "到达终点后就会回归原点" },
        { 76333, 81900, "不想看看终点之后的世界吗？" },
        { 103250, 106250, "哪一个是现实" },
        { 106333, 110600, "哪一个是幻想" },
        { 111600, 115250, "还没结束呢" }
    };

    GUI_WindowFade(hwnd_bg, 0, 255, 200);
    Media_Command("play pv.wmv");

    SelectObject(hdc_text, fonts[6]);
    SetTextColor(hdc_text, RGB(250, 250, 250));
    
    foreach(var i in range(len(subtitles))) {
        struct Subtitle s = subtitles[i];

        while(Media_Position("pv.wmv") < s.start) Sleep(100);

        GUI_TypeText(hwnd_text, hdc_text, 0, screenHeight * 43 / 50, screenWidth, GUI_CENTERX, 50, s.text);

        while(Media_Position("pv.wmv") < s.end) Sleep(100);

        GUI_FillWindow(hwnd_text, NULL, RGB(128, 128, 128));
    }

    while(Media_IsPlaying("pv.wmv")) Sleep(200);

    Media_Command("close pv.wmv");
    ShowWindow(hwnd_text, SW_HIDE);
}


void page5() {
    SetWindowPos(hwnd_bg, 0, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
    SetWindowLongA(window, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU);
    SetWindowPos(window, 0, (screenWidth - winWidth) / 2, (screenHeight - winHeight) / 2, winWidth, winHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);

    GUI_DrawImage(hdc_window, 0, 0, winWidth, winHeight, images[2], STRETCH_HALFTONE);
    ShowWindow(hwnd_normalBtns[0], SW_SHOWDEFAULT);
    ShowWindow(hwnd_normalBtns[1], SW_SHOWDEFAULT);

    GUI_WindowFade(hwnd_bg, 255, 0, 1400);
    ShowWindow(hwnd_bg, SW_HIDE);

    Media_Command("open \"%s\" alias bgm2.mp3", filepaths[2]);
    Media_Command("setaudio bgm2.mp3 volume to 600");
    Media_Command("play bgm2.mp3 repeat");

    while(TRUE) {
        if(GUI_IsClicked(hwnd_normalBtns[0]))
            system("start https://f7d.huijiwiki.com/wiki/首页");

        if(GUI_IsClicked(hwnd_normalBtns[1]))
            system("start https://f7.163.com");

        Sleep(200);
    }
}