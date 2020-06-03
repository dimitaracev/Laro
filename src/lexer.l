%{
    #include "y.tab.h"

    int yywrap(void);
%}

%%

"func"  {   return FUNC;    }
"("     {   return LPAREN;  }
")"     {   return RPAREN;  }
"{"     {   return LCURLY;  }
"}"     {   return RCURLY;  }
">="    {   return GE;      }
"<="    {   return LE;      }
">"     {   return GT;      }
"<"     {   return LT;      }
"=="    {   return EE;      }
"!="    {   return NE;      }
"="     {   return EQUALS;  }
"+"     {   return PLUS;    }
"-"     {   return MINUS;   }
"*"     {   return MUL;     }
"/"     {   return DIV;     }
"if"    {   return IF;      }
"while" {   return WHILE;   }
","     {   return COMA;    }

[0-9]+                  {
                            yylval.integer = strdup(yytext);
                            return INTEGER;
                        }
[a-zA-Z][a-zA-Z0-9]*    {
                            yylval.name = strdup(yytext);
                            return NAME;
                        }
[ \t\n]                 ;
.                       ;
%%

int yywrap(void)
{
    return 1;
}