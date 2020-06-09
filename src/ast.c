#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

ast_node *create_ast_node()
{
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->node_type = node_null;
    node->children = NULL;
    node->val = NULL;
    node->children_size = 0;
    return node;
}


ast_node* create_function_node(char* name, ast_node* params, ast_node* statements)
{
    ast_node* node = create_ast_node();
    node->node_type = node_function;
    node->val = strdup(name);
    
    append_child(node, params);
    append_child(node, statements);
    return node;
}

ast_node* create_function_call_node(char* name, ast_node* params)
{
    ast_node* node = create_ast_node();
    node->node_type = node_function_call;
    node->val = strdup(name);
    append_child(node, params);
    return node;
}


ast_node* create_assignment_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_assignment;
    append_child(node, left);
    append_child(node, right);
    return node;
}

ast_node* create_right_node(ast_node* left, int ant, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = ant;
    append_child(node, left);
    append_child(node, right);
    return node;
}

ast_node* create_if_node(ast_node* condition, ast_node* statements)
{
    ast_node* node = create_ast_node();
    node->node_type = node_if;
    append_child(node, condition);
    append_child(node, statements);
    return node;
}

ast_node* create_if_else_node(ast_node* condition, ast_node* if_statements, ast_node* else_statements)
{
    ast_node* node = create_ast_node();
    node->node_type = node_if_else;
    append_child(node, condition);
    append_child(node, if_statements);
    append_child(node, else_statements);
    return node;
}

ast_node* create_while_node(ast_node* condition, ast_node* statements)
{
    ast_node* node = create_ast_node();
    node->node_type = node_while;
    append_child(node, condition);
    append_child(node, statements);
    return node;
}


ast_node* create_statements_node(ast_node* first)
{
    ast_node* node = create_ast_node();
    node->node_type = node_statements;
    append_child(node, first);
    return node;
}

ast_node* create_identifier_name_node(char* name)
{
    ast_node* node = create_ast_node();
    node->node_type = node_name;
    node->val = strdup(name);
    return node;
}

ast_node* create_identifier_integer_node(char* integer)
{
    ast_node* node = create_ast_node();
    node->node_type = node_integer;
    node->val = strdup(integer);
    return node;
}


ast_node* create_gt_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_gt;
    append_child(node, left);
    append_child(node, right);
    return node;
}

ast_node* create_ge_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_ge;
    append_child(node, left);
    append_child(node, right);
    return node;
}
ast_node* create_lt_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_lt;
    append_child(node, left);
    append_child(node, right);
    return node;
}
ast_node* create_le_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_le;
    append_child(node, left);
    append_child(node, right);
    return node;
}
ast_node* create_ee_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_ee;
    append_child(node, left);
    append_child(node, right);
    return node;
}
ast_node* create_ne_node(ast_node* left, ast_node* right)
{
    ast_node* node = create_ast_node();
    node->node_type = node_ne;
    append_child(node, left);
    append_child(node, right);
    return node;
}

int append_child(ast_node *parent, ast_node *child)
{
    if (parent->children == NULL)
        parent->children = (ast_node **)malloc(sizeof(ast_node *));
    else
        parent->children = (ast_node **)realloc(parent->children, sizeof(ast_node *) * (parent->children_size + 1));

    parent->children[parent->children_size] = child;

    if (parent->children[parent->children_size++] != NULL)
        return 1;
    else
        return 0;
}

int clear_nodes(ast_node* node)
{
    if(node == NULL)
        return -1;
    
    for(int i = 0; i < node->children_size; i++)
    {
        clear_nodes(node->children[i]);
    }
    free(node->val);
    free(node);
    return 1;
}
