#include <stdio.h>
#include "field.h"

int main()
{
    Field field;
    constructField(&field);
    for (size_t i = 0; i < FIELD_SIZE; i++)
        field._field[i] = '*';

    logField(&field);
    destroyField(&field);
}
