#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>
#include "field.h"

struct Field;
static HANDLE descriptor;
static COORD maxSize;

BOOL initConsole(Field*);
void writeConsole(Field*);

#endif // CONSOLE_H_INCLUDED
