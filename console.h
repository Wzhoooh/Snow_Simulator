#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>
#include "field.h"

struct Field;
static HANDLE descriptor;
static COORD screenSize;
static CHAR_INFO* screenBuffer;

BOOL initConsole(Field*);
void destroyConsole();
void writeConsole(Field*);

#endif // CONSOLE_H_INCLUDED
