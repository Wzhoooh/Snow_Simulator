#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdlib.h>
#include <windows.h>

#define CHAR_SPACE ' '
#define ATTR_SPACE 0
#define CHAR_FLAKE '*'
#define ATTR_FLAKE 0
#define CHAR_WALL '%'
#define ATTR_WHALL 0

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

void logField(Field*);

#endif // FIELD_H_INCLUDED
