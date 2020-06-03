/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LPAREN = 258,
    RPAREN = 259,
    LCURLY = 260,
    RCURLY = 261,
    FUNC = 262,
    GE = 263,
    GT = 264,
    LE = 265,
    LT = 266,
    EE = 267,
    NE = 268,
    EQUALS = 269,
    PLUS = 270,
    MINUS = 271,
    MUL = 272,
    DIV = 273,
    IF = 274,
    WHILE = 275,
    COMA = 276,
    NAME = 277,
    INTEGER = 278
  };
#endif
/* Tokens.  */
#define LPAREN 258
#define RPAREN 259
#define LCURLY 260
#define RCURLY 261
#define FUNC 262
#define GE 263
#define GT 264
#define LE 265
#define LT 266
#define EE 267
#define NE 268
#define EQUALS 269
#define PLUS 270
#define MINUS 271
#define MUL 272
#define DIV 273
#define IF 274
#define WHILE 275
#define COMA 276
#define NAME 277
#define INTEGER 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 9 "src/parser.y" /* yacc.c:1921  */

    char* name;
    char* integer;
    struct ast_node* node;
    int type;

#line 111 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
