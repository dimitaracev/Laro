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

int hash_code(char *key)
{
    if (key == NULL)
        return -1;
    int hs = 0;
    int power = 1;
    for (int i = 0; i < strlen(key); i++)
    {
        hs += power * key[i];
        power *= 10;
    }
    return (hs * strlen(key)) % CAPACITY;
}

int insert(symbol_table *st, char *key, ast_node* node)
{
    if (st == NULL || key == NULL)
        return -1;

    symbol *s = (symbol *)malloc(sizeof(symbol));
    s->key = strdup(key);
    s->value = node;
    int hs = hash_code(key);
    st->symbols[hs] = s;
    if (st->symbols[hs] != NULL){
        st->size++;
        return 1;
    }
    return 0;
}

symbol *lookup(symbol_table *st, char *key)
{
    int hs = hash_code(key);
    return st->symbols[hs];
}