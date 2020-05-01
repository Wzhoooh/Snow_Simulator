#include "stdio.h"
#include "field.h"

void constructField(Field* field)
{
    for (size_t i = 0; i < FIELD_SIZE; i++)
        field->_field[i] = '\0';

    for (size_t i = 0; i < MAX_NUM_FLAKES; i++)
        (field->_flakes)[i]._x = (field->_flakes)[i]._y = -1;

    field->_numFlakes = 0;
}

void destroyField(Field* field){}

char* getCell(Field* field, size_t y, size_t x)
{
    size_t index = X_SIZE * y + x;
    if (index < FIELD_SIZE)
        return field->_field + index;
    else
        return NULL;
}

void logField(Field* field)
{
    printf("Flakes:\n");
    for (size_t i = 0; i < field->_numFlakes; i++)
        printf("(%d, %d); ",  (field->_flakes)[i]._x, (field->_flakes)[i]._y);
    printf("\n");

    printf("Field:\n");
    for (size_t i = 0; i < Y_SIZE; i++)
    {
        for (size_t j = 0; j < X_SIZE; j++)
            printf("%c", *getCell(field, i, j));
        printf("\n");
    }

}
