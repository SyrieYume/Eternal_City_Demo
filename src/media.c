#include <windows.h>
#include <stdio.h>

// 向MCI设备发送命令
char* Media_Command(const char *formatCommand, ...) {
    static char command[1024];
    static char buff[256];

    va_list ap;
    va_start(ap, formatCommand);
    vsprintf(command, formatCommand, ap);
    va_end(ap);

    mciSendStringA(command, buff, 255, NULL);

    return buff;
}

BOOL Media_IsPlaying(char *name) {
    return strcmp(Media_Command("status %s mode", name), "playing") == 0;
}

int Media_Position(char *name) {
    return atoi(Media_Command("status %s position", name));
}