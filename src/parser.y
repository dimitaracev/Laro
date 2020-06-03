%{
    #include <stdio.h>
    #include <string.h>
    #include "src/ast.h"
    ast_node* root;
    extern char* yytext;
    extern int yyparse();
    void yyerror(char*);
%}

%union {
    char* name;
    char* integer;
    struct ast_node* node;
    int type;
}
%token LPAREN RPAREN LCURLY RCURLY FUNC GE GT LE LT EE NE EQUALS PLUS MINUS MUL DIV IF WHILE COMA

%type <node> function function_call block_statements statement param params condition identifier if while assignment variable argument arguments
%token <name> NAME
%token <integer> INTEGER
%type <type> operator
%start program

%%


program
    :                                   {
                                            
                                        }
    |   function                        {
                                            append_child(root, $1);
                                        }
    |   program function                {
                                            append_child(root, $2);
                                        }
    ;

operator
    :   PLUS                            {
                                            $$ = node_plus;
                                        }
    |   MINUS                           {
                                            $$ = node_minus;
                                        }
    |   MUL                             {
                                            $$ = node_mul;
                                        }
    |   DIV                             {
                                            $$ = node_div;
                                        }
    ;

variable
    : NAME                              {
                                            $$ = create_identifier_name_node($1);
                                        }
    ;

identifier
    :   NAME                            {
                                            $$ = create_identifier_name_node($1);
                                        }
    |   INTEGER                         {
                                            $$ = create_identifier_integer_node($1);
                                        }
    ;


assignment
    :   variable EQUALS identifier                              {
                                                                    $$ = create_assignment_node($1, $3);
                                                                }
    |   variable EQUALS identifier operator identifier          {
                                                                    $$ = create_assignment_node($1, create_right_node($3, $4, $5));
                                                                }
    ;


statement
    :   assignment                                              {
                                                                    $$ = $1;
                                                                }
    |   if                                                      {
                                                                    $$ = $1;
                                                                }
    |   while                                                   {
                                                                    $$ = $1;
                                                                }
    |   function_call                                           {
                                                                    $$ = $1;
                                                                }
    ;



block_statements
    :                                                           {
                                                                    $$ = create_ast_node();
                                                                }
    |   statement                                               {
                                                                    $$ = create_statements_node($1);
                                                                }
    |   block_statements statement                              {
                                                                    append_child($$, $2);
                                                                }
    ;

condition
    :   identifier GT identifier                                    {
                                                                        $$ = create_gt_node($1, $3);
                                                                    }
    |   identifier GE identifier                                    {
                                                                        $$ = create_ge_node($1, $3);
                                                                    }
    |   identifier LT identifier                                    {
                                                                        $$ = create_lt_node($1, $3);
                                                                    }
    |   identifier LE identifier                                    {
                                                                        $$ = create_ge_node($1, $3);
                                                                    }
    |   identifier EE identifier                                    {
                                                                        $$ = create_ee_node($1, $3);
                                                                    }
    |   identifier NE identifier                                    {
                                                                        $$ = create_ne_node($1, $3);
                                                                    }
    ;

if
    :   IF LPAREN condition RPAREN LCURLY block_statements RCURLY       {
                                                                            $$ = create_if_node($3, $6);
                                                                        }
    ;

while
    :   WHILE LPAREN condition RPAREN LCURLY block_statements RCURLY    {
                                                                            $$ = create_while_node($3, $6);
                                                                        }
    ;



param
    :   NAME                                                    {
                                                                    $$ = create_identifier_name_node($1);
                                                                }
    ;

params
    :                                                           {
                                                                    $$ = create_ast_node();
                                                                }
    | param                                                     {
                                                                    $$ = create_ast_node();
                                                                    append_child($$, $1);
                                                                }
    | params COMA param                                         {
                                                                    append_child($$, $3);
                                                                }
    ;


function
    :   FUNC NAME LPAREN params RPAREN LCURLY block_statements RCURLY       {
                                                                                $$ = create_function_node($2, $4, $7);
                                                                            }
    ;


argument
    :   NAME                                                                {
                                                                                $$ = create_identifier_name_node($1);
                                                                            }
    |   INTEGER                                                             {
                                                                                $$ = create_identifier_integer_node($1);
                                                                            }
    ;

arguments
    :                                                                       {
                                                                                $$ = create_ast_node();
                                                                            }
    |   argument                                                            {
                                                                                $$ = create_ast_node();
                                                                                append_child($$, $1);
                                                                            }
    |   arguments COMA argument                                             {
                                                                                append_child($$, $3);
                                                                            }

function_call
    :   NAME LPAREN arguments RPAREN                                        {
                                                                                $$ = create_function_call_node($1, $3);
                                                                            }
    ;


%%


void yyerror(char* error)
{
    fprintf(stderr, "%s <- %s\n", yytext, error);
}