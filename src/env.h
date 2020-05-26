#ifndef ENV_H
#define ENV_H

#include "symbol_table.h"
#include "ast.h"

struct env
{
    char *name;
    symbol_table *symbols;
};



typedef struct env env;

env *create_env(char*);

int insert_to_env(env *, char *, ast_node*);
int lookup_from_env(env *, char *);

#endif