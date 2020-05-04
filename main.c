#include <stdio.h>
#include "field.h"
#include "console.h"

int main()
{
    Field* field = constructField(100, 40);
    CHAR_INFO wall;
    wall.Attributes = ATTR_WALL;
    wall.Char.AsciiChar = CHAR_WALL;

    *getCell(field, 14, 9) = wall;
    *getCell(field, 15, 9) = wall;
    *getCell(field, 16, 9) = wall;
    *getCell(field, 17, 9) = wall;
    *getCell(field, 18, 9) = wall;
    *getCell(field, 19, 9) = wall;
    *getCell(field, 20, 9) = wall;
    initConsole(field);

    for (;;)
    {
        if (!createNewFlake(field, 17)) printf("flake error pos\n");
        updateField(field);
        writeConsole(field);
        Sleep(200);
        updateField(field);
        writeConsole(field);
        Sleep(200);
    }
    destroyField(field);
}
