#include <conio.h>
#include <stdio.h>
#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdlib.h>
#include <windows.h>

#define CHAR_SPACE ' '
#define ATTR_SPACE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define CHAR_FLAKE '*'
#define ATTR_FLAKE (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define CHAR_WALL '@'
#define ATTR_WALL (FOREGROUND_RED | BACKGROUND_RED | FOREGROUND_INTENSITY)
#define CHAR_CANNON 'W'
#define ATTR_CANNON (FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)

#define ATTR_HELP (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define ATTR_BUTTON (FOREGROUND_GREEN | FOREGROUND_INTENSITY)

typedef struct
{
    CHAR_INFO* screen;
    COORD size;
} Field;

Field* constructField(short, short);
void destroyField(Field*);
BOOL isCorrectPos(Field*, short, short);
CHAR_INFO* getCell(Field*, short, short);
BOOL createNewFlake(Field*, short);
int getFlakeStatus(Field*, short, short);
void updateField(Field*);
COORD getCannonCoord(Field*);
BOOL moveCannonRigh(Field*);
BOOL moveCannonLeft(Field*);
BOOL createNewFlakeFromCannon(Field*);

#endif // FIELD_H_INCLUDED

#include "stdio.h"
#include "time.h"

#define FALLING         1
#define LEFT_SLIDE      2
#define RIGHT_SLIDE    3
#define NOT_ACTIVE      4

Field* constructField(short x, short y)
{
    if (x <= 0 || y <= 0)
        return NULL;

    Field* f = NULL;
    if ( !(f = (Field*)malloc(sizeof(Field))) )
        return NULL;

    if ( !(f->screen = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * x * y)) )
        return NULL;
    for (int i = 0; i < x*y; ++i)
    {
        f->screen[i].Char.AsciiChar = CHAR_SPACE;
        f->screen[i].Attributes = ATTR_SPACE;
    }
    f->size.X = x;
    f->size.Y = y;

    f->screen[0].Char.AsciiChar = CHAR_CANNON;
    f->screen[0].Attributes = ATTR_CANNON;
    return f;
}

void destroyField(Field* field)
{
    free(field->screen);
    free(field);
}

BOOL isCorrectPos(Field* field, short x, short y)
{
    return x >= 0 && x < field->size.X && y >= 0 && y < field->size.Y;
}

CHAR_INFO* getCell(Field* field, short x, short y)
{
    if (isCorrectPos(field, x, y))
        return field->screen + (field->size.X * y + x);

    return NULL;
}

BOOL createNewFlake(Field* field, short x)
{
    CHAR_INFO* cell = getCell(field, x, 1);
    if (cell != NULL && cell->Char.AsciiChar == CHAR_SPACE)
    {
        cell->Char.AsciiChar = CHAR_FLAKE;
        cell->Attributes = ATTR_FLAKE;
        return TRUE;
    }
    return FALSE;
}

int getFlakeStatus(Field* field, short x, short y)
{
    int down = isCorrectPos(field, x, y+1) ?
        getCell(field, x, y+1)->Char.AsciiChar : CHAR_WALL;
    int left = isCorrectPos(field, x-1, y) ?
        getCell(field, x-1, y)->Char.AsciiChar : CHAR_WALL;
    int right = isCorrectPos(field, x+1, y) ?
        getCell(field, x+1, y)->Char.AsciiChar : CHAR_WALL;
    int leftDown = isCorrectPos(field, x-1, y+1) ?
        getCell(field, x-1, y+1)->Char.AsciiChar : CHAR_WALL;
    int rightDown = isCorrectPos(field, x+1, y+1) ?
        getCell(field, x+1, y+1)->Char.AsciiChar : CHAR_WALL;

    if (down == CHAR_SPACE)
        return FALLING;

    if  ((down == CHAR_WALL) || ((down == CHAR_FLAKE) &&
          (left != CHAR_SPACE || leftDown != CHAR_SPACE) &&
          (right != CHAR_SPACE || rightDown != CHAR_SPACE)))
        return NOT_ACTIVE;

    // down == CHAR_FLAKE)
    if (leftDown == CHAR_SPACE && rightDown == CHAR_SPACE &&
        left == CHAR_SPACE && right == CHAR_SPACE)
    {
        return rand() > RAND_MAX / 2 ? LEFT_SLIDE : RIGHT_SLIDE;
    }
    if (right != CHAR_SPACE || rightDown != CHAR_SPACE)
            return LEFT_SLIDE;
    if (left != CHAR_SPACE || leftDown != CHAR_SPACE)
        return RIGHT_SLIDE;

    return 0;
}

void updateField(Field* field)
{
    for (int i = field->size.Y-1; i >= 0; --i)
        for (int j = field->size.X-1; j >= 0; --j)
        {
            CHAR_INFO* cell = getCell(field, j, i);
            if (cell->Char.AsciiChar != CHAR_FLAKE)
                continue;
            else
            {
                int status = getFlakeStatus(field, j, i);
                if (status == FALLING)
                {
                    *getCell(field, j, i+1) = *cell;
                    cell->Char.AsciiChar = CHAR_SPACE;
                    cell->Attributes = ATTR_SPACE;
                }
                else if (status == LEFT_SLIDE)
                {
                    *getCell(field, j-1, i+1) = *cell;
                    cell->Char.AsciiChar = CHAR_SPACE;
                    cell->Attributes = ATTR_SPACE;
                }
                else if (status == RIGHT_SLIDE)
                {
                    *getCell(field, j+1, i+1) = *cell;
                    cell->Char.AsciiChar = CHAR_SPACE;
                    cell->Attributes = ATTR_SPACE;
                }
                else if (status == NOT_ACTIVE)
                    ;
            }
        }
}

COORD getCannonCoord(Field* field)
{
    COORD cannonCoord = {-1, -1};
    for (int j = 0; j < field->size.X; j++)
        if (getCell(field, j, 0)->Char.AsciiChar == CHAR_CANNON)
        {
            cannonCoord.X = j;
            cannonCoord.Y = 0;
        }
    return cannonCoord;
}
BOOL moveCannonRigh(Field* field)
{
    COORD cannonCoord = getCannonCoord(field);
    if (cannonCoord.X < field->size.X-1)
    {
        getCell(field, cannonCoord.X, cannonCoord.Y)->Char.AsciiChar = CHAR_SPACE;
        getCell(field, cannonCoord.X, cannonCoord.Y)->Attributes = ATTR_SPACE;
        getCell(field, cannonCoord.X+1, cannonCoord.Y)->Char.AsciiChar = CHAR_CANNON;
        getCell(field, cannonCoord.X+1, cannonCoord.Y)->Attributes = ATTR_CANNON;
        return TRUE;
    }
    else
        return FALSE;
}
BOOL moveCannonLeft(Field* field)
{
    COORD cannonCoord = getCannonCoord(field);
    if (cannonCoord.X > 0)
    {
        getCell(field, cannonCoord.X, cannonCoord.Y)->Char.AsciiChar = CHAR_SPACE;
        getCell(field, cannonCoord.X, cannonCoord.Y)->Attributes = ATTR_SPACE;
        getCell(field, cannonCoord.X-1, cannonCoord.Y)->Char.AsciiChar = CHAR_CANNON;
        getCell(field, cannonCoord.X-1, cannonCoord.Y)->Attributes = ATTR_CANNON;
        return TRUE;
    }
    else
        return FALSE;
}

BOOL createNewFlakeFromCannon(Field* field)
{
    COORD cannonCoord = getCannonCoord(field);
    return createNewFlake(field, cannonCoord.X);
}


#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>
#include "field.h"

BOOL initConsole(Field*);
void destroyConsole();
void writeConsole(Field*);

void writeHelpMessage(void);
void printMessage (const char* str);

#endif // CONSOLE_H_INCLUDED


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


#ifndef READ_FILE_H_INCLUDED
#define READ_FILE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "field.h"

Field* getFieldFromFile(FILE* f);

#endif // READ_FILE_H_INCLUDED


Field* getFieldFromFile(FILE* f)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD sizeFeeld = GetLargestConsoleWindowSize(handle);
    sizeFeeld.X -= 6;
    sizeFeeld.Y -= 3;

    Field* field = constructField(sizeFeeld.X, sizeFeeld.Y);
    if (!field)
        return NULL;

    for (int Y = 1; Y < field->size.Y; Y++)
    {
        char buffer[10*1024];
        if ((NULL == fgets(buffer, sizeof(buffer), f)) || feof(f))
            break;

        for (int X = 0; X < field->size.X; X++)
        {
            if (X < strlen(buffer) && !isspace(buffer[X]))
            {
                getCell(field, X, Y)->Char.AsciiChar = CHAR_WALL;
                getCell(field, X, Y)->Attributes = ATTR_WALL;
            }
        }
    }

    return field;
};


#define BACK_SPACE 8
#define ENTER 13
#define ESC 27
#define UP 72   // 224 72
#define DOWN 80 // 224 80
#define LEFT 75 // 224 75
#define RIGHT 77// 224 77

int main()
{
    SetConsoleTitle("Snow Simulator");

    SetConsoleOutputCP(1251);
    printf("Вы можете выбрать шрифт и его размер в свойствах этого окна.\n"
           "Укажите имя текстового файла с преградами: ");
    char nameFile[1024];
    FILE* f = NULL;
    while (!f)
    {
        fgets(nameFile, 1024, stdin);
        if (nameFile[strlen(nameFile)-1] == '\n' || nameFile[strlen(nameFile)-1] == '\r')
            nameFile[strlen(nameFile)-1] = '\0';
        f = fopen(nameFile, "r");
        if (!f)
            printf("\nНе смог открыть файл. Укажите снова: ");
    }

    Field* field = getFieldFromFile(f);
    if (!field)
    {
        printf("ERROR: invalid field initialization");
        return 0;
    }
    if (!initConsole(field))
    {
        printMessage("ERROR: invalid console initialization");
        return 0;
    }
    writeHelpMessage();

    int isRandomSnowfall = FALSE;
    for (;; Sleep(50))
    {
        int keyPressed = 0;
        if (kbhit())
        {
            int ch = getch();
            if (ch == ESC) // ESC
                break;

            if (ch == 0 || ch == 224)
            {
                ch = getch();
                if (ch == LEFT || ch == RIGHT)
                    keyPressed = ch;
            }
            else
                keyPressed = ch;
        }

        if (keyPressed == ' ' || keyPressed == ENTER)
            createNewFlakeFromCannon(field);
        else if ((keyPressed == LEFT) || (keyPressed == '4'))
            moveCannonLeft(field);
        else if ((keyPressed == RIGHT) || (keyPressed == '6'))
            moveCannonRigh(field);
        else if ((keyPressed == 'r') || (keyPressed == 'R'))
            isRandomSnowfall = !isRandomSnowfall;

        if (isRandomSnowfall)
        {
            #define MAXFLAKES 8.
            int numFlakes = (int)(rand()*MAXFLAKES/RAND_MAX);
            for (int i = 0; i < numFlakes; ++i)
                createNewFlake(field, (int)(rand()*field->size.X/RAND_MAX));
        }

        writeConsole(field);
        updateField(field);
    }
    destroyField(field);
    destroyConsole();
    fclose(f);
}
