#include "console.h"
#include "field.h"
#include <stdio.h>

static HANDLE descriptor;
static COORD screenSize;
static CHAR_INFO* screenBuffer;

HWND WINAPI GetConsoleWindow(void);

static void ConsoleSetXYChar (int x, int y, CHAR_INFO ch)
{
    screenBuffer[x + y*screenSize.X] = ch;
}

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
    screenSize.Y = field->size.Y + 1; // for message
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

void destroyConsole()
{
    free(screenBuffer);
}

void writeConsole(Field* field)
{
    for (int i = 0; i < field->size.Y; i++)
        for (int j = 0; j < field->size.X; j++)
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

void writeHelpMessage()
{
    CHAR_INFO buf[screenSize.X];

    CHAR_INFO ch;
    ch.Char.AsciiChar = ' ';
    ch.Attributes = ATTR_HELP;
    for (int i = 0; i < screenSize.X; i++)
        buf[i] = ch;

    char *mess[][2] = {
        { "Esc", "- quit" },
        { "Space", "- drop snowflake" },
        { "<- ->", "- move source" },
        { "R", "- random snowfall" }
    };
    int pos = 1;
    for (int i = 0; i < sizeof(mess)/sizeof(mess[0]); i++)
    {
        ch.Attributes = ATTR_BUTTON;
        for (char *p = mess[i][0]; *p; ++p)
        {
            ch.Char.AsciiChar = *p;
            buf[pos++] = ch;
        }

        pos++;

        ch.Attributes = ATTR_HELP;
        for (char *p = mess[i][1]; *p; ++p)
        {
            ch.Char.AsciiChar = *p;
            buf[pos++] = ch;
        }

        pos++;
        pos++;
    }

    COORD zeroPoint = { 0, 0 };
    COORD sizeOfBlock = { screenSize.X, 1 };
    SMALL_RECT region;
    region.Left = 0;
    region.Top = screenSize.Y-1;
    region.Right = screenSize.X-1;
    region.Bottom = screenSize.Y-1;
    WriteConsoleOutput(descriptor, buf, sizeOfBlock, zeroPoint, &region);
}

void printMessage (const char* str)
{
    if (INVALID_HANDLE_VALUE == descriptor)
    {
        if (str != NULL)
            printf("%s\n", str);
    }
    else
    {
        DWORD written;
        if (str != NULL)
        {
            WriteConsole(descriptor, str, strlen(str), &written, NULL);
            WriteConsole(descriptor, " ", 1, &written, NULL);
        }
    }
}



