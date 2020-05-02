#include <stdio.h>
#include "field.h"

int main()
{
    Field* field = constructField(20, 20);
    if (!field) printf("construct error\n");
    getCell(field, 4, 5)->Char.AsciiChar = CHAR_WALL;
    getCell(field, 3, 5)->Char.AsciiChar = CHAR_WALL;
    getCell(field, 2, 5)->Char.AsciiChar = CHAR_WALL;

    if (!createNewFlake(field, 4)) printf("flake error pos\n");
    if (!createNewFlake(field, 0)) printf("flake error pos\n");
//    if (!createNewFlake(field, 6)) printf("flake error pos\n");
//    if (!createNewFlake(field, 12)) printf("flake error pos\n");
//    if (!createNewFlake(field, 15)) printf("flake error pos\n");
//    if (!createNewFlake(field, 45)) printf("flake error pos\n");
    logField(field);
    printf("\n");
    updateField(field);logField(field);printf("\n");
    updateField(field);logField(field);printf("\n");
    updateField(field);logField(field);printf("\n");
    for (;;)
    {
        if (!createNewFlake(field, 4)) printf("flake error pos\n");
        updateField(field);logField(field);printf("\n");
        updateField(field);logField(field);printf("\n");
    }

    logField(field);
    destroyField(field);
}
