#ifndef AST_H
#define AST_H

enum ast_node_type {
    node_null, node_if, node_while, node_func_definition, node_func_call,
    node_operator_name, node_operator_integer, node_assignment, node_statement,
    node_param, node_passing_param, node_params, node_passing_params, node_bool_operation,
    node_passing_name_param, node_passing_integer_param, node_variable
};


struct ast_node {
    enum ast_node_type node_type;
    struct ast_node** children;
    char* val;
    int children_size;
};

typedef struct ast_node ast_node;


ast_node* create_ast_node();

ast_node* create_operator_name_node(char*);
ast_node* create_operator_integer_node(char*);

ast_node* create_assignment_node(char*);
ast_node* create_statement_node();

ast_node* create_param_node(char*);
ast_node* create_params_node();

ast_node* create_function_definition_node(char*, ast_node*, ast_node*);
ast_node* create_function_call_node(char*, ast_node*);

ast_node* create_passing_param_name_node(char*);
ast_node* create_passing_param_integer_node(char*);
ast_node* create_passing_params_node();

ast_node* create_if_node(ast_node*, ast_node*);
ast_node* create_while_node(ast_node*, ast_node*);

ast_node* create_bool_operation_node(char*, ast_node*, ast_node*);


int append_child(ast_node*, ast_node*);

void print_ast(ast_node*);


#endif