#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "generate.h"

int current_register_count = 0;
int current_temporary_register_count = 0;
int current_label_count = 0;
int stack_pointer = 0;

char instruction[INSTRUCTION_LENGTH];

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

int generate_code(code* mips_code, ast_node *root)
{
    if (root == NULL)
        return -1;

    snprintf(instruction, INSTRUCTION_LENGTH, ".text\njal main\nli $v0, 10\nsyscall\n");
    append_code(mips_code, instruction);

    for (int i = 0; i < root->children_size; i++)
    {
        function *func = create_function(root->children[i]->val);
        generate_function(mips_code, func, root->children[i]);
        clear_symbol_table(func->st);
        free(func);
    }
    generate_print_function(mips_code);
    return 1;
}

int generate_function(code* mips_code, function *func, ast_node *node)
{
    if (node == NULL)
        return 1;

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
        generate_statement(mips_code, func, node->children[1]->children[i]);
    }

    snprintf(instruction, INSTRUCTION_LENGTH, "lw $ra, %d($sp)\n", func_pointer);
    append_code(mips_code, instruction);

    snprintf(instruction, INSTRUCTION_LENGTH, "addi $sp, $sp, 4\njr $ra\n");
    append_code(mips_code, instruction);

    stack_pointer -= 4;
    return 1;
}

int generate_statement(code* mips_code, function *func, ast_node *node)
{
    if (node == NULL)
        return 1;

    switch (node->node_type)
    {
    case node_assignment:
        generate_assignment(mips_code, func, node->children[0], node->children[1]);
        return 1;
        break;
    case node_if:
        generate_if(mips_code, func, node->children[0], node->children[1]);
        return 1;
    case node_while:
        generate_while(mips_code, func, node->children[0], node->children[1]);
        return 1;
        break;
    case node_if_else:
        generate_if_else(mips_code, func, node->children[0], node->children[1], node->children[2]);
        return 1;
        break;
    case node_function_call:
        generate_function_call(mips_code, func, node);
        break;
    default:
        break;
    }
    for (int i = 0; i < node->children_size; i++)
    {
        generate_statement(mips_code, func, node->children[i]);
    }
    return 1;
}

operands *get_operands(code* mips_code, function *func, ast_node *node)
{
    operands *ops = (operands *)malloc(sizeof(operands));
    for (int i = 0; i < node->children_size; i++)
    {
        ast_node *operand = node->children[i];
        if (operand->node_type == node_name)
        {
            symbol *r_symb = function_lookup(func, operand->val);
            if (i == 0)
                ops->operand_1 = strdup(r_symb->value);
            else if (i == 1)
                ops->operand_2 = strdup(r_symb->value);
        }
        else if (operand->node_type == node_integer)
        {
            if (i == 0)
            {
                ops->operand_1 = new_temp();
                snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", ops->operand_1, operand->val);
                append_code(mips_code, instruction);
            }
            else if (i == 1)
            {
                ops->operand_2 = new_temp();
                snprintf(instruction, INSTRUCTION_LENGTH, "addi %s, $0, %s\n", ops->operand_2, operand->val);
                append_code(mips_code, instruction);
            }
        }
    }
    return ops;
}

int generate_assignment(code* mips_code, function *func, ast_node *left, ast_node *right)
{
    symbol *symb = function_lookup(func, left->val);
    char *n_reg = NULL;
    if (symb == NULL)
    {
        n_reg = new_register();
        function_insert(func, left->val, n_reg);
    }
    else
    {
        n_reg = strdup(symb->value);
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
        operands *ops = get_operands(mips_code, func, right);
        switch (right->node_type)
        {
        case node_plus:
            snprintf(instruction, INSTRUCTION_LENGTH, "add %s, %s, %s\n", n_reg, ops->operand_1, ops->operand_2);
            break;
        case node_minus:
            snprintf(instruction, INSTRUCTION_LENGTH, "sub %s, %s, %s\n", n_reg, ops->operand_1, ops->operand_2);
            break;
        case node_mul:
            snprintf(instruction, INSTRUCTION_LENGTH, "mult %s, %s\n", ops->operand_1, ops->operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mflo %s\n", n_reg);
            break;
        case node_div:
            snprintf(instruction, INSTRUCTION_LENGTH, "div %s, %s\n", ops->operand_1, ops->operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mflo %s\n", n_reg);
            break;
        case node_mod:
            snprintf(instruction, INSTRUCTION_LENGTH, "div %s, %s\n", ops->operand_1, ops->operand_2);
            append_code(mips_code, instruction);
            snprintf(instruction, INSTRUCTION_LENGTH, "mfhi %s\n", n_reg);
        default:
            break;
        }
        append_code(mips_code, instruction);
        free(ops);
    }
    free(n_reg);
    return 1;
}

condition *get_condition(code* mips_code, function *func, ast_node *cond)
{
    operands *ops = get_operands(mips_code, func, cond);
    condition *condi = (condition *)malloc(sizeof(condition));
    condi->ops = ops;
    switch (cond->node_type)
    {
    case node_gt:
        snprintf(condi->keyword, 4, "ble");
        break;
    case node_ge:
        snprintf(condi->keyword, 4, "blt");
        break;
    case node_lt:
        snprintf(condi->keyword, 4, "bge");
        break;
    case node_le:
        snprintf(condi->keyword, 4, "bgt");
        break;
    case node_ee:
        snprintf(condi->keyword, 4, "bne");
        break;
    case node_ne:
        snprintf(condi->keyword, 4, "beq");
        break;
    default:
        break;
    }
    return condi;
}

int generate_while(code* mips_code, function *func, ast_node *cond, ast_node *statements)
{
    if (func == NULL || cond == NULL)
        return -1;

    char *exit_label = new_label();
    char *while_label = new_label();

    condition *condi = get_condition(mips_code, func, cond);

    snprintf(instruction, INSTRUCTION_LENGTH, "%s_start:\n", while_label);
    append_code(mips_code, instruction);

    snprintf(instruction, INSTRUCTION_LENGTH, "%s %s, %s, %s\n", condi->keyword, condi->ops->operand_1, condi->ops->operand_2, exit_label);
    append_code(mips_code, instruction);

    for (int i = 0; i < statements->children_size; i++)
    {
        generate_statement(mips_code, func, statements->children[i]);
    }

    snprintf(instruction, INSTRUCTION_LENGTH, "j %s_start\n", while_label);
    append_code(mips_code, instruction);

    snprintf(instruction, INSTRUCTION_LENGTH, "%s:\n", exit_label);
    append_code(mips_code, instruction);

    free(condi->ops);
    free(condi);
    free(while_label);
    free(exit_label);
    return 1;
}

int generate_if(code* mips_code, function *func, ast_node *cond, ast_node *statements)
{
    if (func == NULL || cond == NULL)
        return -1;

    char *exit_label = new_label();
    condition *condi = get_condition(mips_code, func, cond);
    snprintf(instruction, INSTRUCTION_LENGTH, "%s %s, %s, %s\n", condi->keyword, condi->ops->operand_1, condi->ops->operand_2, exit_label);
    append_code(mips_code, instruction);

    for (int i = 0; i < statements->children_size; i++)
    {
        generate_statement(mips_code, func, statements->children[i]);
    }

    snprintf(instruction, INSTRUCTION_LENGTH, "%s:\n", exit_label);
    append_code(mips_code, instruction);

    free(condi->ops);
    free(condi);
    free(exit_label);
    return 1;
}

int generate_if_else(code* mips_code, function *func, ast_node *cond, ast_node *if_statements, ast_node *else_statements)
{
    if (func == NULL || cond == NULL)
        return -1;

    char *else_label = new_label();
    char *exit_label = new_label();

    condition *condi = get_condition(mips_code, func, cond);

    snprintf(instruction, INSTRUCTION_LENGTH, "%s %s, %s, %s\n", condi->keyword, condi->ops->operand_1, condi->ops->operand_2, else_label);
    append_code(mips_code, instruction);

    for (int i = 0; i < if_statements->children_size; i++)
    {
        generate_statement(mips_code, func, if_statements->children[i]);
    }

    snprintf(instruction, INSTRUCTION_LENGTH, "j %s\n%s:\n", exit_label, else_label);
    append_code(mips_code, instruction);

    for (int i = 0; i < else_statements->children_size; i++)
    {
        generate_statement(mips_code, func, else_statements->children[i]);
    }

    snprintf(instruction, INSTRUCTION_LENGTH, "%s:\n", exit_label);
    append_code(mips_code, instruction);

    free(condi->ops);
    free(condi);
    free(else_label);
    free(exit_label);
    return 1;
}

int generate_function_call(code* mips_code, function *func, ast_node *func_call)
{
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

int generate_print_function(code* mips_code)
{
    snprintf(instruction, INSTRUCTION_LENGTH, "print:\nli $v0, 1\nsyscall\naddi $a0, $0, 0xA\naddi $v0, $0, 0xB\nsyscall\njr $ra\n");
    append_code(mips_code, instruction);
    return 1;
}