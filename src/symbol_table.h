#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

#define CAPACITY 101



struct symbol
{
    char *key;
    char* value;
};

typedef struct symbol symbol;

struct symbol_table
{
    symbol *symbols[CAPACITY];
    int size;
};

typedef struct symbol_table symbol_table;

symbol_table *create_symbol_table();

int clear_symbol_table(symbol_table*);

int insert(symbol_table *, char *, char *);

symbol *lookup(symbol_table *, char *);

#endif