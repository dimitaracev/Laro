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



declared_functions* create_df();
int clear_df(declared_functions*);

int semantic(code*, declared_functions*, ast_node*);
int semantic_function(code*, declared_functions*, function*, ast_node*);
int semantic_if_else_while(code*, declared_functions*, function*, ast_node*);
int semantic_assignment(code*, function*, ast_node*, ast_node*);
int semantic_function_call(code*, declared_functions*, function*, ast_node*);


#endif