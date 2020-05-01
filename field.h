#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

#define X_SIZE 50
#define Y_SIZE 20
#define FIELD_SIZE X_SIZE*Y_SIZE
#define MAX_NUM_FLAKES FIELD_SIZE

typedef struct Point
{
    int _x;
    int _y;
} Point;

typedef struct Field
{
    char _field[FIELD_SIZE];
    struct Point _flakes[MAX_NUM_FLAKES]; // buffer of active flakes
    size_t _numFlakes; // index to put new flake into _flakes
} Field;

void constructField(Field*);
void destroyField(Field*);
char* getCell(Field*, size_t, size_t);
void logField(Field*);

#endif // FIELD_H_INCLUDED
