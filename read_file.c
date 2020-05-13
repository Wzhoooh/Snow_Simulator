#include "read_file.h"

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
