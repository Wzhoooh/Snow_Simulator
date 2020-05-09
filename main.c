#include <conio.h>
#include "field.h"
#include "console.h"

#define BACK_SPACE 8
#define ENTER 13
#define ESC 27
#define UP 72   // 224 72
#define DOWN 80 // 224 80
#define LEFT 75 // 224 75
#define RIGHT 77// 224 77

int main()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Field* field = constructField(GetLargestConsoleWindowSize(handle).X-1,
                                  GetLargestConsoleWindowSize(handle).Y-1);
    initConsole(field);
    CHAR_INFO wall;
    wall.Attributes = ATTR_WALL;
    wall.Char.AsciiChar = CHAR_WALL;

    *getCell(field, 14, 9) = wall;
    *getCell(field, 15, 9) = wall;
    *getCell(field, 16, 9) = wall;
    *getCell(field, 17, 9) = wall;
    *getCell(field, 18, 9) = wall;
    *getCell(field, 19, 9) = wall;
    *getCell(field, 54, 34) = wall;
    *getCell(field, 85, 23) = wall;
    *getCell(field, 35, 26) = wall;
    *getCell(field, 38, 15) = wall;
    *getCell(field, 37, 16) = wall;
    *getCell(field, 47, 11) = wall;
    *getCell(field, 26, 30) = wall;

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
        else if (keyPressed == LEFT)
            moveCannonLeft(field);
        else if (keyPressed == RIGHT)
            moveCannonRigh(field);

        updateField(field);
        writeConsole(field);
    }
    destroyField(field);
}
