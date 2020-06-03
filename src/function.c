#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "function.h"

function* create_function(char* name)
{
    function* proc = (function*)malloc(sizeof(function));
    proc->name = strdup(name);
    proc->st = create_symbol_table();
    return proc;
}

symbol* function_lookup(function* proc, char* key)
{
    if(proc == NULL || key == NULL)
        return NULL;
    
    return lookup(proc->st, key);
}

int function_insert(function* proc, char* key, char* value)
{
    if(proc == NULL || key == NULL || value == NULL)
        return -1;
    
    return insert(proc->st, key, value);
}