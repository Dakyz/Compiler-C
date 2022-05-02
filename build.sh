cd source
yacc -d parser.y
lex lex.l
g++ -o output -std=c++17 -Wall y.tab.c lex.yy.c
rm y.tab.c
rm y.tab.h
rm lex.yy.c
cp output ../
rm output
cd ..