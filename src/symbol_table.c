#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

symbol_table *create_symbol_table()
{
    symbol_table *st = (symbol_table *)malloc(sizeof(symbol_table));
    st->size = 0;
    return st;
}

int clear_symbol_table(symbol_table* st)
{
    for(int i = 0; i < st->size; i++)
    {
        free(st->symbols[i]);
    }
    free(st);
}

int insert(symbol_table *st, char *key, char* value)
{
    if (st == NULL || key == NULL)
        return -1;

    symbol *s = (symbol *)malloc(sizeof(symbol));
    s->key = strdup(key);
    s->value = strdup(value);
    st->symbols[st->size++] = s;
    return 1;
}

symbol *lookup(symbol_table *st, char *key)
{
    for(int i = 0; i < st->size; i++)
    {
        if(strcmp(st->symbols[i]->key, key) == 0)
        {
            return st->symbols[i];
        }
    }
    return NULL;
}