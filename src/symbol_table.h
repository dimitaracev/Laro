#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

#define CAPACITY 500



struct symbol
{
    char *key;
    ast_node* value;
};

typedef struct symbol symbol;

struct symbol_table
{
    symbol *symbols[CAPACITY];
    int size;
};

typedef struct symbol_table symbol_table;

symbol_table *create_symbol_table();

int hash_code(char *);

int insert(symbol_table *, char *, ast_node*);

symbol *lookup(symbol_table *, char *);

#endif