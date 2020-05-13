#ifndef READ_FILE_H_INCLUDED
#define READ_FILE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "field.h"

Field* getFieldFromFile(FILE* f);

#endif // READ_FILE_H_INCLUDED
