#ifndef READ_FILE_H_INCLUDED
#define READ_FILE_H_INCLUDED

#include "stdlib.h"
#include "field.h"

struct Field* getFieldFromFile(struct FILE* f);
int getMaxSizeOfStr(struct FILE* f);

#endif // READ_FILE_H_INCLUDED
