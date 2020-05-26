

all: lexer parser
	gcc -o Laro lex.yy.c y.tab.c src/ast.c src/symbol_table.c src/env.c src/semantic_analysis.c

lexer: src/lexer.l
	flex src/lexer.l

parser: src/parser.y
	yacc -d src/parser.y