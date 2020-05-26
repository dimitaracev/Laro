#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "semantic_analysis.h"

int analyze(ast_node *root)
{
    if (root == NULL)
        return -1;

    if (strcmp(root->children[root->children_size - 1]->val, "main") != 0)
    {
        printf("Main function not found.\n");
        exit(0);
    }

    for (int i = 0; i < root->children_size; i++)
    {
        if (root->children[i] != NULL)
        {
            env *environment = create_env(root->children[i]->val);
            analyze_env(environment, root->children[i]);
        }
    }
}

int analyze_env(env *environment, ast_node *node)
{
    if (node == NULL)
        return 1;

    switch (node->node_type)
    {
    case node_assignment:
        insert_to_env(environment, node->children[0]->val, node->children[1]);
        break;
    case node_param:
        insert_to_env(environment, node->val, NULL);
        break;
    case node_operator_name:
    case node_passing_name_param:
        if (!lookup_from_env(environment, node->val))
        {
            printf("Variable %s used before it is declared.\n", node->val);
            exit(0);
        }
        break;
    case node_func_call:
        if (strcmp(node->val, "main") == 0)
        {
            printf("Calling main is not permitted.\n");
            exit(0);
        }
        break;
    default:
        break;
    }
    for (int i = 0; i < node->children_size; i++)
    {
        if (node->children[i] != NULL)
        {
            analyze_env(environment, node->children[i]);
        }
    }
}