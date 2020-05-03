#include <stdio.h>
#include "field.h"
#include "console.h"

int main()
{
    printf("%d\n", initConsole(NULL));
    printf("----------------------------");

//    Field* field = constructField(30, 30);
//    if (!field) printf("construct error\n");
//    getCell(field, 15, 5)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 14, 7)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 16, 7)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 13, 9)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 17, 9)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 29, 9)->Char.AsciiChar = CHAR_WALL;
//    getCell(field, 28, 9)->Char.AsciiChar = CHAR_WALL;
//
//    if (!createNewFlake(field, 4)) printf("flake error pos\n");
//    if (!createNewFlake(field, 0)) printf("flake error pos\n");
////    if (!createNewFlake(field, 6)) printf("flake error pos\n");
////    if (!createNewFlake(field, 12)) printf("flake error pos\n");
////    if (!createNewFlake(field, 15)) printf("flake error pos\n");
////    if (!createNewFlake(field, 45)) printf("flake error pos\n");
//    logField(field);
//    printf("\n");
//    updateField(field);logField(field);printf("\n");
//    updateField(field);logField(field);printf("\n");
//    updateField(field);logField(field);printf("\n");
//    for (;;)
//    {
//        if (!createNewFlake(field, 29)) printf("flake error pos\n");
//        updateField(field);logField(field);printf("\n");
//        updateField(field);logField(field);printf("\n");
//    }
//
//    logField(field);
//    destroyField(field);
}
