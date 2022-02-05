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

code* new_code();
int append_code(code*, char*);
int clear_code(code*);

char* new_register();
char* new_temp();
char* new_label();

int generate_code(code*, ast_node*);
int generate_function(code*, function*, ast_node*);
int generate_statement(code*, function*, ast_node*);
int generate_assignment(code*, function*, ast_node*, ast_node*);

operands* get_operands(code*, function*, ast_node*);
condition* get_condition(code*, function*, ast_node*);
int generate_while(code*, function*, ast_node*, ast_node*);
int generate_if(code*, function*, ast_node*, ast_node*);
int generate_if_else(code*, function*, ast_node*, ast_node*, ast_node*);

int generate_function_call(code*, function*, ast_node*);

int generate_print_function();


#endif