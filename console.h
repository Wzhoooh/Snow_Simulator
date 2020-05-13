#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>
#include "field.h"

BOOL initConsole(Field*);
void destroyConsole();
void writeConsole(Field*);

void writeHelpMessage(void);
void printMessage (const char* str);

#endif // CONSOLE_H_INCLUDED
