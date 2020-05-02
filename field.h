#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>

typedef struct _Field
{
    CHAR_INFO* _field;
    COORD _size;
} Field, *PField;

Field* constructField(short, short);
void destroyField(Field*);
bool isCorrectPos(Field*, short, short);
CHAR_INFO* getCell(Field*, short, short);
bool createNewFlake(Field*, short);
int getFlakeStatus(Field*, short, short);
bool updateField(Field*);

void logField(Field*);

#endif // FIELD_H_INCLUDED
