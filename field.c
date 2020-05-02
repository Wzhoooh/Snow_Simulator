#include "stdio.h"
#include "field.h"
#include "time.h"

#define FALLING         1
#define LEFT_SLIDE      2
#define RIGHT_SLIDE     3
#define RANDOM_SLIDE    4
#define NOT_ACTIVE      5

Field* constructField(short x, short y)
{
    if (x <= 0 || y <= 0)
        return NULL;

    Field* f = (Field*)malloc(sizeof(Field));
    f->_field = (CHAR_INFO*)malloc(sizeof(CHAR_INFO)*x*y);
    return f;
}

void destroyField(Field* field)
{
    free(field->_field);
    free(field);
}

bool isCorrectPos(Field* field, short x, short y)
{
    return x < field->_size.X && y < field->_size.Y;
}

CHAR_INFO* getCell(Field*, short, short);
bool createNewFlake(Field*, short);
int getFlakeStatus(Field*, short, short);
bool updateField(Field*);

void logField(Field*);
