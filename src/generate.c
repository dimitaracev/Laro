#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "generate.h"

int current_register_count = 0;
int current_temporary_register_count = 0;
int current_label_count = 0;

int stack_pointer = 0;

char *new_register()
{
    char *n_register = (char *)malloc(sizeof(char) * REGISTER_LENGTH);
    snprintf(n_register, REGISTER_LENGTH, "$s%d", current_register_count++);
    if (current_register_count > MAX_REGISTER_COUNT)
        current_register_count = 0;
    return n_register;
}

char *new_temp()
{
    char *n_temp = (char *)malloc(sizeof(char) * REGISTER_LENGTH);
    snprintf(n_temp, REGISTER_LENGTH, "$t%d", current_temporary_register_count++);
    if (current_temporary_register_count > MAX_TEMPORARY_REGISTER_COUNT)
        current_temporary_register_count = 0;
    return n_temp;
}

char *new_label()
{
    char *n_label = (char *)malloc(sizeof(char) * LABEL_LENGTH);
    snprintf(n_label, LABEL_LENGTH, "label%d", current_label_count++);
    return n_label;
}

code *new_code()
{
    code *n_code = (code *)malloc(sizeof(code));
    n_code->characters = 0;
    n_code->inst_code = (char *)calloc(1, sizeof(char));
    strncpy(n_code->inst_code, "", 1);
    return n_code;
}

int append_code(code *s_code, char *instruction)
{
    s_code->characters = s_code->characters + INSTRUCTION_LENGTH;
    s_code->inst_code = (char *)realloc(s_code->inst_code, s_code->characters + 1);
    strncat(s_code->inst_code, instruction, INSTRUCTION_LENGTH);
    return 1;
}

int clear_code(code *s_code)
{
    free(s_code->inst_code);
    free(s_code);
    return 1;
}

int generate_code(ast_node *root)
{
    if (root == NULL)
        return -1;

    char instruction[INSTRUCTION_LENGTH];
    snprintf(instruction, INSTRUCTION_LENGTH, ".text\njal main\nli $v0, 10\nsyscall\n");
    append_code(mips_code, instruction);

    for (int i = 0; i < root->children_size; i++)
    {
        function *func = create_function(root->children[i]->val);
        generate_function(func, root->children[i]);
        clear_symbol_table(func->st);
        free(func);
    }
    generate_print_function();
    return 1;
}

int generate_function(function *func, ast_node *node)
{
    if (node == NULL)
        return 1;

    char instruction[INSTRUCTION_LENGTH];
    snprintf(instruction, INSTRUCTION_LENGTH, "%s:\n", node->val);
    append_code(mips_code, instruction);

    for (int i = 0; i < node->children[0]->children_size; i++)
    {
        ast_node *parameter = node->children[0]->children[i];
        char *n_temp = new_temp();
        snprintf(instruction, INSTRUCTION_LENGTH, "add %s, $0, $a%d\n", n_temp, i);
        append_code(mips_code, instruction);
        function_insert(func, parameter->val, n_temp);
        free(n_temp);
    }
    int func_pointer = stack_pointer;
    snprintf(instruction, INSTRUCTION_LENGTH, "addi $sp, $sp, -4\n");
    append_code(mips_code, instruction);
    snprintf(instruction, INSTRUCTION_LENGTH, "sw $ra, %d($sp)\n", stack_pointer);
    append_code(mips_code, instruction);
    stack_pointer += 4;
    for (int i = 0; i < node->children[1]->children_size; i++)
    {
        generate_statement(func, node->children[1]->children[i]);
    }
    snprintf(instruction, INSTRUCTION_LENGTH, "lw $ra, %d($sp)\n", func_pointer);
    append_code(mips_code, instruction);
    snprintf(instruction, INSTRUCTION_LENGTH, "addi $sp, $sp, 4\njr $ra\n");
    append_code(mips_code, instruction);
    stack_pointer -= 4;
    return 1;
}

int generate_statement(function *func, ast_node *node)
{
    if (node == NULL)
        return 1;

    switch (node->node_type)
    {
    case node_assignment:
        generate_assignment(func, node->children[0], node->children[1]);
        return 1;
        break;
    case node_if:
    case node_while:
        generate_if_while(func, node->children[0], node->children[1], node->node_type);
        return 1;
        break;
    case node_function_call:
        generate_function_call(func, node);
        break;
    default:
        break;
    }
    for (int i = 0; i < node->children_size; i++)
    {
        generate_statement(func, node->children[i]);
    }
    return 1;
}

int generate_assignment(function *func, ast_node *left, ast_node *right)
{
    char instruction[INSTRUCTION_LENGTH];
    symbol *symb = function_lookup(func, left->val);
    char *n_reg = NULL;
    int new_reg = 0;
    if (symb == NULL)
    {
        n_reg = new_register();
        function_insert(func, left->val, n_reg);
        new_reg = 1;
    }
    else
    {
        n_reg = symb->value;
    }

    if (right->children_size == 0)
    {
        if (right->node_type == node_name)
        {
            symbol *r_symb = function_lookup(func, right->val);
            snprintf(instruction, INSTRUCTION_LENGTH, "add %s, $0, %s\n", n_reg, r_symb->value);
            append_code(mips_code, instruction);
        }
        else if (right->node_type == node_integer)
        {
            snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", n_reg, right->val);
            append_code(mips_code, instruction);
        }
    }
    else if (right->children_size == 2)
    {
        char *operand_1 = NULL;
        char *operand_2 = NULL;
        int n_op1 = 0;
        int n_op2 = 0;
        for (int i = 0; i < right->children_size; i++)
        {
            ast_node *operand = right->children[i];
            if (operand->node_type == node_name)
            {
                symbol *r_symb = function_lookup(func, operand->val);
                if (i == 0)
                    operand_1 = r_symb->value;
                else if (i == 1)
                    operand_2 = r_symb->value;
            }
            else if (operand->node_type == node_integer)
            {
                if (i == 0)
                {
                    operand_1 = new_temp();
                    snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", operand_1, operand->val);
                    append_code(mips_code, instruction);
                    n_op1 = 1;
                }
                else if (i == 1)
                {
                    operand_2 = new_temp();
                    snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", operand_2, operand->val);
                    append_code(mips_code, instruction);
                    n_op2 = 1;
                }
            }
        }
        switch (right->node_type)
        {
        case node_plus:
            snprintf(instruction, INSTRUCTION_LENGTH, "add %s, %s, %s\n", n_reg, operand_1, operand_2);
            break;
        case node_minus:
            snprintf(instruction, INSTRUCTION_LENGTH, "sub %s, %s, %s\n", n_reg, operand_1, operand_2);
            break;
        case node_mul:
            snprintf(instruction, INSTRUCTION_LENGTH, "mult %s, %s\n", operand_1, operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mflo %s\n", n_reg);
            break;
        case node_div:
            snprintf(instruction, INSTRUCTION_LENGTH, "div %s, %s\n", operand_1, operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mflo %s\n", n_reg);
            break;
        case node_mod:
            snprintf(instruction, INSTRUCTION_LENGTH, "div %s, %s\n", operand_1, operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mfhi %s\n", n_reg);
        default:
            break;
        }
        append_code(mips_code, instruction);

        if (n_op1)
            free(operand_1);

        if (n_op2)
            free(operand_2);
    }
    if (new_reg)
        free(n_reg);
    return 1;
}

int generate_if_while(function *func, ast_node *condition, ast_node *statements, int if_while)
{
    if (func == NULL || condition == NULL)
        return -1;

    char instruction[INSTRUCTION_LENGTH];

    char *operand_1 = NULL;
    char *operand_2 = NULL;
    int n_op1 = 0;
    int n_op2 = 0;
    for (int i = 0; i < condition->children_size; i++)
    {
        ast_node *operand = condition->children[i];
        if (operand->node_type == node_name)
        {
            symbol *r_symb = function_lookup(func, operand->val);
            if (i == 0)
                operand_1 = r_symb->value;
            else if (i == 1)
                operand_2 = r_symb->value;
        }
        else if (operand->node_type == node_integer)
        {
            if (i == 0)
            {
                operand_1 = new_temp();
                snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", operand_1, operand->val);
                append_code(mips_code, instruction);
                n_op1 = 1;
            }
            else if (i == 1)
            {
                operand_2 = new_temp();
                snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", operand_2, operand->val);
                append_code(mips_code, instruction);
                n_op2 = 1;
            }
        }
    }

    char *while_label;

    if (if_while == node_while)
    {
        while_label = new_label();
        snprintf(instruction, INSTRUCTION_LENGTH, "%s_start:\n", while_label);
        append_code(mips_code, instruction);
    }

    char *exit_label = new_label();

    switch (condition->node_type)
    {
    case node_gt:
        snprintf(instruction, INSTRUCTION_LENGTH, "ble");
        break;
    case node_ge:
        snprintf(instruction, INSTRUCTION_LENGTH, "blt");
        break;
    case node_lt:
        snprintf(instruction, INSTRUCTION_LENGTH, "bge");
        break;
    case node_le:
        snprintf(instruction, INSTRUCTION_LENGTH, "bgt");
        break;
    case node_ee:
        snprintf(instruction, INSTRUCTION_LENGTH, "bne");
        break;
    case node_ne:
        snprintf(instruction, INSTRUCTION_LENGTH, "beq");
        break;
    default:
        break;
    }

    append_code(mips_code, instruction);
    snprintf(instruction, INSTRUCTION_LENGTH, " %s, %s, %s\n", operand_1, operand_2, exit_label);
    append_code(mips_code, instruction);

    if (n_op1)
        free(operand_1);

    if (n_op2)
        free(operand_2);

    for (int i = 0; i < statements->children_size; i++)
    {
        generate_statement(func, statements->children[i]);
    }
    if (if_while == node_while)
    {
        snprintf(instruction, INSTRUCTION_LENGTH, "j %s_start\n", while_label);
        append_code(mips_code, instruction);
        free(while_label);
    }
    snprintf(instruction, INSTRUCTION_LENGTH, "%s:\n", exit_label);
    append_code(mips_code, instruction);

    free(exit_label);
    return 1;
}

int generate_function_call(function *func, ast_node *func_call)
{
    char instruction[INSTRUCTION_LENGTH];
    for (int i = 0; i < func_call->children[0]->children_size; i++)
    {
        ast_node *argument = func_call->children[0]->children[i];
        if (argument->node_type == node_name)
        {
            symbol *symb = function_lookup(func, argument->val);
            snprintf(instruction, INSTRUCTION_LENGTH, "add $a%d, $0, %s\n", i, symb->value);
        }
        else if (argument->node_type == node_integer)
        {
            snprintf(instruction, INSTRUCTION_LENGTH, "addi $a%d, $0, %s\n", i, argument->val);
        }
        append_code(mips_code, instruction);
    }
    snprintf(instruction, INSTRUCTION_LENGTH, "jal %s\n", func_call->val);
    append_code(mips_code, instruction);
    return 1;
}

int generate_print_function()
{
    char instruction[INSTRUCTION_LENGTH];
    snprintf(instruction, INSTRUCTION_LENGTH, "print:\nli $v0, 1\nsyscall\njr $ra\n");
    append_code(mips_code, instruction);
    return 1;
}