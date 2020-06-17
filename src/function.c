#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "function.h"

function* create_function(char* name)
{
    function* func = (function*)malloc(sizeof(function));
    func->name = strdup(name);
    func->st = create_symbol_table();
    return func;
}

int copy_function(function* from_func, function* to_func)
{
    if(from_func == NULL || to_func == NULL)
        return 0;
    
    for(int i = 0; i < from_func->st->size; i++)
    {
        symbol* symb = from_func->st->symbols[i];
        function_insert(to_func, symb->key, symb->value);
    }
    return 1;
}


symbol* function_lookup(function* func, char* key)
{
    if(func == NULL || key == NULL)
        return NULL;
    
    return lookup(func->st, key);
}

int function_insert(function* func, char* key, char* value)
{
    if(func == NULL || key == NULL || value == NULL)
        return -1;
    
    return insert(func->st, key, value);
}