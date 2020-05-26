#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "env.h"

env *create_env(char* name)
{
    env *environment = (env *)malloc(sizeof(env));
    environment->name = strdup(name);
    environment->symbols = create_symbol_table();
    return environment;
}

int insert_to_env(env *environment, char *key, ast_node* node)
{
    insert(environment->symbols, key, node);
}

int lookup_from_env(env *environment, char *key)
{
    if (lookup(environment->symbols, key) != NULL)
        return 1;
    else
        return 0;
}