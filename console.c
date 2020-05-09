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
    MoveWindow(GetConsoleWindow(), 0, 0, 0, 0, TRUE);

    screenSize.X = field->size.X;
    screenSize.Y = field->size.Y;
    if (!SetConsoleScreenBufferSize(descriptor, screenSize))
        return FALSE;

    SMALL_RECT newPos;
    newPos.Left = 0;
    newPos.Top = 0;
    newPos.Right = screenSize.X - 1;
    newPos.Bottom = screenSize.Y - 1;
    if (!SetConsoleWindowInfo(descriptor, TRUE, &newPos))
        return FALSE;

    size_t memSize = sizeof(CHAR_INFO) * screenSize.X * screenSize.Y;
    screenBuffer = malloc(memSize);
    if (screenBuffer == NULL)
        return FALSE;

    CHAR_INFO ch;
    ch.Attributes = 0;
    ch.Char.AsciiChar = 0;
    for (int i = 0; i < screenSize.Y; i++)
        for (int j = 0; j < screenSize.X; j++)
            ConsoleSetXYChar(j, i, ch);

    writeConsole(field);
    return TRUE;
}
void ConsoleSetXYChar (int x, int y, CHAR_INFO ch)
{
    screenBuffer[x + y*screenSize.X] = ch;
}

void destroyConsole()
{
    free(screenBuffer);
}

void writeConsole(Field* field)
{
    for (int i = 0; i < screenSize.Y; i++)
        for (int j = 0; j < screenSize.X; j++)
        {
            CHAR_INFO cell = *getCell(field, j, i);
            if (cell.Char.AsciiChar !=
                screenBuffer[j + i*screenSize.X].Char.AsciiChar)
            {

                int index = i * field->size.X + j;
                COORD zeroPoint = { 0, 0 };
                COORD onePoint = { 1, 1 };
                SMALL_RECT region;
                region.Left = j;
                region.Top = i;
                region.Right = j;
                region.Bottom = i;
                WriteConsoleOutput(descriptor, field->screen+index, onePoint,
                                   zeroPoint, &region);
                screenBuffer[j + i*screenSize.X] = cell;
            }
        }
}

void printMessage (const char* str1, const char* str2, DWORD num)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", num);

    // ошибки не проверяем, нет смысла
    if (INVALID_HANDLE_VALUE == descriptor)
    {
        if (NULL != str1)
            printf("%s ", str1);
        if (NULL != str2)
            printf("%s ", str2);
        printf("%s\n", buf);
    }
    else
    {
        DWORD written;
        if (NULL != str1)
        {
            WriteConsole(descriptor, str1, strlen(str1), &written, NULL);
            WriteConsole(descriptor, " ", 1, &written, NULL);
        }
        if (NULL != str1)
        {
            WriteConsole(descriptor, str2, strlen(str2), &written, NULL);
            WriteConsole(descriptor, " ", 1, &written, NULL);
        }
        WriteConsole(descriptor, buf, strlen(buf), &written, NULL);
        WriteConsole(descriptor, "\n", 1, &written, NULL);
    }
}



