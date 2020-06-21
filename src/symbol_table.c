#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

symbol_table *create_symbol_table()
{
    symbol_table *st = (symbol_table *)malloc(sizeof(symbol_table));
    st->size = 0;
    for (int i = 0; i < CAPACITY; i++)
    {
        st->symbols[i] = NULL;
    }
    return st;
}

int hash_code(char *key)
{
    int hash = 9901;
    while (*key)
    {
        hash = ((hash << 13) ^ *key++) % CAPACITY;
    }
    return hash;
}

int clear_symbol_table(symbol_table *st)
{
    for (int i = 0; i < CAPACITY; i++)
    {
        if (st->symbols[i] != NULL)
            free(st->symbols[i]);
    }
    free(st);
    return 1;
}

int insert(symbol_table *st, char *key, char *value)
{
    if (st == NULL || key == NULL)
        return -1;

    symbol *s = (symbol *)malloc(sizeof(symbol));
    s->key = strdup(key);
    s->value = strdup(value);
    st->symbols[hash_code(key)] = s;
    return 1;
}

symbol *lookup(symbol_table *st, char *key)
{
    return st->symbols[hash_code(key)];
}