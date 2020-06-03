#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "symbol_table.h"

struct function {
    char* name;
    symbol_table* st;
};

typedef struct function function;

function* create_function(char*);

symbol* function_lookup(function*, char*);
int function_insert(function*, char*, char*);

#endif