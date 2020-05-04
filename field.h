#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdlib.h>
#include <windows.h>

#define CHAR_SPACE ' '
#define ATTR_SPACE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define CHAR_FLAKE '*'
#define ATTR_FLAKE (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define CHAR_WALL 177
#define ATTR_WALL (FOREGROUND_RED | BACKGROUND_RED | FOREGROUND_INTENSITY)
#define CHAR_CANNON 'W'
#define ATTR_CANNON (FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)

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

void logField(Field*);

#endif // FIELD_H_INCLUDED
