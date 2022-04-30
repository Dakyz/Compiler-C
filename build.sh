cd source
yacc -d parser.y
lex lex.l
g++ -c -fPIC -Wall list.cpp
ar rvs liblist.a list.o
gcc -shared -o liblist.so list.o -Wl,--whole-archive liblist.a -Wl,--no-whole-archive
g++ -o output -Wall liblist.a y.tab.c lex.yy.c
cd ..