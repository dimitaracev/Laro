#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "function.h"
#include "generate.h"


struct declared_functions
{
    symbol_table* functions;
};

typedef struct declared_functions declared_functions;


declared_functions* dfs;

declared_functions* create_df();
int clear_df(declared_functions*);

int semantic(ast_node*);
int semantic_function(function*, ast_node*);
int semantic_if_else_while(function*, ast_node*);
int semantic_assignment(function*, ast_node*, ast_node*);
int semantic_function_call(function*, ast_node*);


#endif