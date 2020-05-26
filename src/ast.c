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

ast_node *create_operator_name_node(char *name)
{
    ast_node *node = create_ast_node();
    node->node_type = node_operator_name;
    node->val = strdup(name);
    return node;
}

ast_node *create_operator_integer_node(char *integer)
{
    ast_node *node = create_ast_node();
    node->node_type = node_operator_integer;
    node->val = strdup(integer);
    return node;
}

ast_node *create_assignment_node(char *name)
{
    ast_node *node = create_ast_node();
    ast_node *variable = create_ast_node();
    node->node_type = node_assignment;
    node->val = strdup("=");
    variable->node_type = node_variable;
    variable->val = strdup(name);
    
    append_child(node, variable);
    return node;
}

ast_node *create_statement_node()
{
    ast_node *node = create_ast_node();
    node->node_type = node_statement;
    return node;
}

ast_node *create_param_node(char *name)
{
    ast_node *node = create_ast_node();
    node->node_type = node_param;
    node->val = strdup(name);
    return node;
}

ast_node *create_function_definition_node(char *name, ast_node *params, ast_node *statements)
{
    ast_node *node = create_ast_node();
    node->node_type = node_func_definition;
    node->val = strdup(name);
    append_child(node, params);
    append_child(node, statements);
    return node;
}

ast_node *create_function_call_node(char *name, ast_node *passing_params)
{
    ast_node *node = create_ast_node();
    node->node_type = node_func_call;
    node->val = strdup(name);
    append_child(node, passing_params);
    return node;
}

ast_node *create_passing_param_name_node(char *name)
{
    ast_node *node = create_ast_node();
    node->node_type = node_passing_name_param;
    node->val = strdup(name);
    return node;
}

ast_node *create_passing_param_integer_node(char *name)
{
    ast_node *node = create_ast_node();
    node->node_type = node_passing_integer_param;
    node->val = strdup(name);
    return node;
}

ast_node *create_params_node()
{
    ast_node *node = create_ast_node();
    node->node_type = node_params;
    return node;
}

ast_node *create_passing_params_node()
{
    ast_node *node = create_ast_node();
    node->node_type = node_passing_params;
    return node;
}

ast_node *create_if_node(ast_node *condition, ast_node *statements)
{
    ast_node *node = create_ast_node();
    node->node_type = node_if;
    append_child(node, condition);
    append_child(node, statements);
    return node;
}

ast_node *create_while_node(ast_node *condition, ast_node *statements)
{
    ast_node *node = create_ast_node();
    node->node_type = node_while;
    append_child(node, condition);
    append_child(node, statements);
    return node;
}

ast_node *create_bool_operation_node(char *op, ast_node *left, ast_node *right)
{
    ast_node *node = create_ast_node();
    node->node_type = node_bool_operation;
    node->val = strdup(op);
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

void print_ast(ast_node *root)
{
    if (root == NULL)
        return;

    if (root->val)
        printf("%s\n", root->val);

    for (int i = 0; i < root->children_size; i++)
    {
        print_ast(root->children[i]);
    }
}