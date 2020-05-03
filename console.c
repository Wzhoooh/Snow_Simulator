#include "console.h"
#include "field.h"
#include <stdio.h>

HWND WINAPI GetConsoleWindow(void);

BOOL initConsole(Field* field)
{
    AllocConsole();

    descriptor = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    if (descriptor == INVALID_HANDLE_VALUE)
        return FALSE;

    if (!SetConsoleActiveScreenBuffer(descriptor))
        return FALSE;

    CONSOLE_CURSOR_INFO cInfo = { 1, FALSE };
    SetConsoleCursorInfo(descriptor, &cInfo);
    MoveWindow(GetConsoleWindow(), 3, 3, 3, 3, TRUE);

    maxSize = GetLargestConsoleWindowSize(descriptor);
    const int delta = 3;
    maxSize.X -= delta*2;
    maxSize.Y -= delta*2;
    if (!SetConsoleScreenBufferSize(descriptor, maxSize))
        return FALSE;

    SMALL_RECT newPos;
    newPos.Left = 0;
    newPos.Top = 0;
    newPos.Right = maxSize.X - 1;
    newPos.Bottom = maxSize.Y - 1;
    if (!SetConsoleWindowInfo(descriptor, TRUE, &newPos))
        return FALSE;


    return TRUE;
}
void writeConsole(Field* field);
