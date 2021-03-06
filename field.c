#include "stdio.h"
#include "field.h"
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
