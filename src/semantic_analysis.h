#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H


#include "env.h"
#include "ast.h"


int analyze(ast_node*);
int analyze_env(env*, ast_node*);


#endif