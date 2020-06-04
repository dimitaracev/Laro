#ifndef AST_H
#define AST_H

enum ast_node_type {
    node_null, node_function, node_function_call, node_if, node_while, 
    node_gt, node_ge, node_lt, node_le, node_ee, node_ne,
    node_name, node_integer,
    node_plus, node_minus, node_mul, node_div, node_mod, node_assignment, 
    node_statements
};


struct ast_node {
    enum ast_node_type node_type;
    struct ast_node** children;
    char* val;
    int children_size;
};

typedef struct ast_node ast_node;


ast_node* create_ast_node();

ast_node* create_function_node(char*, ast_node*, ast_node*);
ast_node* create_function_call_node(char*, ast_node*);

ast_node* create_if_node(ast_node*, ast_node*);
ast_node* create_while_node(ast_node*, ast_node*);

ast_node* create_assignment_node(ast_node*, ast_node*);
ast_node* create_right_node(ast_node*, int, ast_node*);



ast_node* create_statements_node(ast_node*);

ast_node* create_identifier_name_node(char*);
ast_node* create_identifier_integer_node(char*);

ast_node* create_gt_node(ast_node*, ast_node*);
ast_node* create_ge_node(ast_node*, ast_node*);
ast_node* create_lt_node(ast_node*, ast_node*);
ast_node* create_le_node(ast_node*, ast_node*);
ast_node* create_ee_node(ast_node*, ast_node*);
ast_node* create_ne_node(ast_node*, ast_node*);


int append_child(ast_node*, ast_node*);


int clear_nodes(ast_node*);

#endif