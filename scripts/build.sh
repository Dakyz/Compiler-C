yacc -d ../source/parser.y
lex ../source/lex.l
gcc -o output y.tab.c lex.yy.c
rm y.tab.c
rm y.tab.h
rm lex.yy.c
mkdir ../build
cp output ../build/
rm output