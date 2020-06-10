#ifndef GENERATE_H
#define GENERATE_H

#include "ast.h"
#include "function.h"

#define MAX_REGISTER_COUNT 8
#define MAX_TEMPORARY_REGISTER_COUNT 10
#define REGISTER_LENGTH 10
#define LABEL_LENGTH 10
#define INSTRUCTION_LENGTH 80


struct operands
{
    char* operand_1;
    char* operand_2;
};

typedef struct operands operands;

struct condition
{
    char keyword[4];
    operands* ops;
};

typedef struct condition condition;

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

operands* get_operands(function*, ast_node*);
condition* get_condition(function*, ast_node*);
int generate_while(function*, ast_node*, ast_node*);
int generate_if(function*, ast_node*, ast_node*);
int generate_if_else(function*, ast_node*, ast_node*, ast_node*);

int generate_function_call(function*, ast_node*);

int generate_print_function();


#endif