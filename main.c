#include <stdio.h>
#include "field.h"

int main()
{
    Field* field = constructField(3, 4);
//    for (size_t i = 0; i < FIELD_SIZE; i++)
//        field._field[i] = '&';
//
//    if (!createNewFlake(&field, 4)) printf("flake error pos\n");
//    if (!createNewFlake(&field, 0)) printf("flake error pos\n");
//    if (!createNewFlake(&field, 6)) printf("flake error pos\n");
//    if (!createNewFlake(&field, 12)) printf("flake error pos\n");
//    if (!createNewFlake(&field, 15)) printf("flake error pos\n");
//    if (!createNewFlake(&field, 45)) printf("flake error pos\n");
//    updateField(&field);
//    updateField(&field);
//    updateField(&field);
//    updateField(&field);
//    updateField(&field);
//    updateField(&field);
//
//    logField(&field);
    destroyField(&field);
}
