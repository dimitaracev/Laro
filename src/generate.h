#ifndef GENERATE_H
#define GENERATE_H

#include "ast.h"
#include "function.h"

#define MAX_REGISTER_COUNT 8
#define MAX_TEMPORARY_REGISTER_COUNT 10
#define REGISTER_LENGTH 10
#define LABEL_LENGTH 10
#define INSTRUCTION_LENGTH 60

struct code
{
    char* inst_code;
    int characters;
};

typedef struct code code;

code* mips_code;
code* error_code;

code* new_code();
int append_code(code*, char*);
int clear_code(code*);

char* new_register();
char* new_temp();
char* new_label();

int generate_code(ast_node*);
int generate_function(function*, ast_node*);
int generate_statement(function*, ast_node*);
int generate_assignment(function*, ast_node*, ast_node*);
int generate_if_while(function*, ast_node*, ast_node*, int);
int generate_function_call(function*, ast_node*);

int generate_print_function();


#endif