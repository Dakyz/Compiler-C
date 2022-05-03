all: test

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

lex.yy.c: lex.l y.tab.h
	lex lex.l

test: lex.yy.c y.tab.c y.tab.h
	g++ -o output -std=c++17 -Wall y.tab.c lex.yy.c

