%{

    #include <stdio.h>


    #include "src/semantic_analysis.h"

    ast_node* root;

    extern FILE* yyin;
    extern char* yytext;

    void yyerror(char*);

%}

%union
{
    struct ast_node* node;
    char* name;
    char* integer;
}

%token LPARAN RPARAN LCURLY RCURLY FUNC EQUALS PLUS IF LT LE GT GE EE NE WHILE COMA
%token <name> NAME
%token <integer> INTEGER

%type <node> operator assignment statement statements func_definition func_call param params passing_param passing_params if while bool_operation

%start program

%%

program
    :
    |   program func_definition                         {
                                                            append_child(root, $2);
                                                        }
    ;



operator
    :   NAME                                            {
                                                            $$ = create_operator_name_node($1);
                                                        }
    |   INTEGER                                         {
                                                            $$ = create_operator_integer_node($1);
                                                        }
    ;


assignment
    :   NAME EQUALS operator                        {
                                                        ast_node* node = create_assignment_node($1);
                                                        append_child(node, $3);
                                                        $$ = node;
                                                    }
    |   NAME EQUALS operator PLUS operator          {
                                                        ast_node* node = create_assignment_node($1);
                                                        ast_node* concat = create_ast_node();

                                                        append_child(concat, $3);
                                                        append_child(concat, $5);

                                                        append_child(node, concat);

                                                        $$ = node;
                                                    }                                 
    ;


statement
    :   func_call                                   {
                                                        $$ = $1;
                                                    }
    |   if                                          {
                                                        $$ = $1;
                                                    }
    |   while                                       {
                                                        $$ = $1;
                                                    }
    |  assignment                                   {
                                                        $$ = $1;
                                                    }
    ;

statements
    :                                               {
                                                        $$ = create_ast_node();
                                                    }
    |   statement                                   {
                                                        ast_node* node = create_statement_node();
                                                        append_child(node, $1);
                                                        $$ = $1;
                                                    }
    |   statements statement                        {
                                                        append_child($$, $2);
                                                    }
    ;

bool_operation
    :   operator GE operator                        {
                                                        $$ = create_bool_operation_node(">=", $1, $3);
                                                    }
    |   operator GT operator                        {
                                                        $$ = create_bool_operation_node(">", $1, $3);
                                                    }
    |   operator LE operator                        {
                                                        $$ = create_bool_operation_node("<=", $1, $3);
                                                    }
    |   operator LT operator                        {
                                                        $$ = create_bool_operation_node("<", $1, $3);
                                                    }
    |   operator EE operator                        {
                                                        $$ = create_bool_operation_node("==", $1, $3);
                                                    }
    |   operator NE operator                        {
                                                        $$ = create_bool_operation_node("!=", $1, $3);
                                                    }
    ;

if
    :   IF LPARAN bool_operation RPARAN LCURLY statements RCURLY    {
                                                                        $$ = create_if_node($3, $6);
                                                                    }
    ;

while
    :   WHILE LPARAN bool_operation RPARAN LCURLY statements RCURLY {
                                                                        $$ = create_while_node($3, $6);
                                                                    }
    ; 



param
    :   NAME                                                    {
                                                                    $$ = create_param_node($1);
                                                                }
    ;

params
    :                                                           {
                                                                    $$ = create_ast_node();
                                                                }
    |   param                                                   {
                                                                    ast_node* node = create_params_node();
                                                                    append_child(node, $1);
                                                                    $$ = node;
                                                                }
    | params COMA param                                         {
                                                                    append_child($$, $3);
                                                                }
    ; 

func_definition
    :   FUNC NAME LPARAN params RPARAN LCURLY statements RCURLY         {
                                                                            $$ = create_function_definition_node($2, $4, $7);
                                                                        }
    ;


passing_param
    :   NAME                                                    {
                                                                    $$ = create_passing_param_name_node($1);
                                                                }
    |   INTEGER                                                 {
                                                                    $$ = create_passing_param_integer_node($1);
                                                                }
    ;

passing_params
    :                                                           {
                                                                    $$ = create_ast_node();
                                                                }
    |   passing_param                                           {
                                                                    ast_node* node = create_passing_params_node();
                                                                    append_child(node, $1);
                                                                    $$ = node;
                                                                }
    |   passing_params COMA passing_param                       {
                                                                    append_child($1, $3);
                                                                }
    ;

func_call
    :  NAME LPARAN passing_params RPARAN                            {
                                                                        $$ = create_function_call_node($1, $3);
                                                                    }
    ;



%%


void yyerror(char* error)
{
    fprintf(stderr, "%s <- %s\n", yytext, error);
}

int main(int argc, char* argv[])
{
    if(argc == 2)
        yyin = fopen(argv[1], "r");

    root = create_ast_node();
    yyparse();
    print_ast(root);
    analyze(root);
}