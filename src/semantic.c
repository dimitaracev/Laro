#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"

declared_functions *create_df()
{
    declared_functions *df = (declared_functions *)malloc(sizeof(declared_functions));
    df->functions = create_symbol_table();
    return df;
}

int clear_df(declared_functions *df)
{
    clear_symbol_table(df->functions);
    free(df);
    return 1;
}

int semantic(ast_node *root)
{
    if (root == NULL)
        return -1;

    int main_found = 0;
    for (int i = 0; i < root->children_size; i++)
    {
        if (strcmp(root->children[i]->val, "main") == 0)
            main_found = 1;
        else
        {
            char digit[2];
            snprintf(digit, 2, "%d", root->children[i]->children[0]->children_size);
            insert(dfs->functions, root->children[i]->val, digit);
        }
    }

    if (!main_found)
    {
        char instruction[INSTRUCTION_LENGTH];
        snprintf(instruction, INSTRUCTION_LENGTH, "Main function not found.\n");
        append_code(error_code, instruction);
    }

    char digit[2];
    snprintf(digit, 2, "%d", 1);
    insert(dfs->functions, "print", digit);

    for (int i = 0; i < root->children_size; i++)
    {
        function *func = create_function(root->children[i]->val);
        semantic_function(func, root->children[i]);
        clear_symbol_table(func->st);
        free(func);
    }
    return 1;
}

int semantic_function(function *func, ast_node *node)
{
    switch (node->node_type)
    {
    case node_assignment:
        semantic_assignment(func, node->children[0], node->children[1]);
        break;
    case node_if:
    case node_while:
        semantic_if_else_while(func, node);
        return 1;
        break;
    case node_function_call:
        semantic_function_call(func, node);
        break;
    case node_function:
        for (int i = 0; i < node->children[0]->children_size; i++)
        {
            function_insert(func, node->children[0]->children[i]->val, "");
        }
        break;
    default:
        break;
    }
    for (int i = 0; i < node->children_size; i++)
    {
        semantic_function(func, node->children[i]);
    }
    return 1;
}

int semantic_function_call(function *func, ast_node *func_call)
{
    if (func_call == NULL)
        return -1;

    char instruction[INSTRUCTION_LENGTH];
    if (!lookup(dfs->functions, func_call->val))
    {
        snprintf(instruction, INSTRUCTION_LENGTH, "Function %s is not declared.\n", func_call->val);
        append_code(error_code, instruction);
    }
    else
    {
        ast_node *params = func_call->children[0];
        if (params->children_size != atoi(lookup(dfs->functions, func_call->val)->value))
        {
            snprintf(instruction, INSTRUCTION_LENGTH, "Function %s call arguments in function %s not matching.\n", func_call->val, func->name);
            append_code(error_code, instruction);
        }
        for (int i = 0; i < params->children_size; i++)
        {
            ast_node *param = params->children[i];
            if (param->node_type == node_name)
            {
                if (!function_lookup(func, param->val))
                {
                    snprintf(instruction, INSTRUCTION_LENGTH, "Parameter %s in function %s is used before it is declared.\n", param->val, func_call->val);
                    append_code(error_code, instruction);
                }
            }
        }
    }

    return 1;
}

int semantic_assignment(function *func, ast_node *left, ast_node *right)
{
    char instruction[INSTRUCTION_LENGTH];
    if (right->children_size == 0)
    {
        if (right->node_type == node_name)
        {
            if (!function_lookup(func, right->val))
            {
                snprintf(instruction, INSTRUCTION_LENGTH, "Variable %s in function %s is used before it is declared.\n", right->val, func->name);
                append_code(error_code, instruction);
            }
        }
    }
    else if (right->children_size == 2)
    {
        for (int i = 0; i < right->children_size; i++)
        {
            ast_node *operand = right->children[i];
            if (operand->node_type == node_name)
            {
                if (!function_lookup(func, operand->val))
                {
                    snprintf(instruction, INSTRUCTION_LENGTH, "Variable %s in function %s is used before it is declared.\n", operand->val, func->name);
                    append_code(error_code, instruction);
                }
            }
        }
    }
    function_insert(func, left->val, "");
    return 1;
}

int semantic_if_else_while(function *func, ast_node *iew)
{
    if (iew == NULL)
        return -1;

    char instruction[INSTRUCTION_LENGTH];
    function* scope = create_function("if_else_while");
    copy_function(func, scope);
    ast_node* condition = iew->children[0];
    for (int i = 0; i < condition->children_size; i++)
    {
        ast_node *operand = condition->children[i];
        if (operand->node_type == node_name)
        {
            if (!function_lookup(func, operand->val))
            {
                snprintf(instruction, INSTRUCTION_LENGTH, "Operand %s in condition (function %s), is used before it is declared.\n", operand->val, func->name);
                append_code(error_code, instruction);
            }
        }
    }

    for(int i = 0; i < iew->children[1]->children_size; i++)
    {
        semantic_function(scope, iew->children[1]->children[i]);
    }

    clear_symbol_table(scope->st);
    free(scope);
    return 1;
}