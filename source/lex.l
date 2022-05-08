D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*
LOCAL		"\""[a-zA-Z0-9_.]*"\""
SYSTEM		<[a-zA-Z0-9_.]*>
%{
#include <stdio.h>

#include "y.tab.h"
void count();
void check();
int comment();
int check_type();
extern "C" {
    int yylex(void);
}
extern void new_line();
extern bool is_type_name(char * id);
extern bool is_find(char * id);
extern "C" int yywrap ( void );
#ifdef __cplusplus
static int yyinput (void);
static int (*input) (void) = yyinput;
#else
static int input (void );
#endif
void line_comment();

%}
%%
"#".* {}
"/\*"			{ comment(); }
"//"			{ line_comment(); }
"__inline"
"__extension__"
"__restrict"
"__attribute__"[^;]*";" { check(); return ';';}
"__asm__"[^;]*";" { check(); return ';';}
"auto"			{ return(AUTO); }
"break"			{ return(BREAK); }
"case"			{ return(CASE); }
"char"			{ return(CHAR); }
"const"			{ return(CONST); }
"continue"		{ return(CONTINUE); }
"default"		{ return(DEFAULT); }
"do"			{ return(DO); }
"double"		{ return(DOUBLE); }
"else"			{ return(ELSE); }
"enum"			{ return(ENUM); }
"extern"		{ return(EXTERN); }
"float"			{ return(FLOAT); }
"for"			{ return(FOR); }
"goto"			{ return(GOTO); }
"if"			{ return(IF); }
"int"			{ return(INT); }
"long"			{ return(LONG); }
"register"		{ return(REGISTER); }
"return"		{ return(RETURN); }
"short"			{ return(SHORT); }
"signed"		{ return(SIGNED); }
"sizeof"		{ return(SIZEOF); }
"static"		{ return(STATIC); }
"struct"		{ return(STRUCT); }
"switch"		{ return(SWITCH); }
"typedef"		{ return(TYPEDEF); }
"union"			{ return(UNION); }
"unsigned"		{ return(UNSIGNED); }
"void"			{ return(VOID); }
"volatile"		{ return(VOLATILE); }
"while"			{ return(WHILE); }

{L}({L}|{D})*		{ count(); return(check_type()); }

0[xX]{H}+{IS}?		{ count(); return(CONSTANT); }
0{D}+{IS}?		{ count(); return(CONSTANT); }
{D}+{IS}?		{ count(); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ count(); return(CONSTANT); }

{D}+{E}{FS}?		{ count(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ count(); return(STRING_LITERAL); }

"..."			{ return(ELLIPSIS); }
">>="			{ return(RIGHT_ASSIGN); }
"<<="			{ return(LEFT_ASSIGN); }
"+="			{ return(ADD_ASSIGN); }
"-="			{ return(SUB_ASSIGN); }
"*="			{ return(MUL_ASSIGN); }
"/="			{ return(DIV_ASSIGN); }
"%="			{ return(MOD_ASSIGN); }
"&="			{ return(AND_ASSIGN); }
"^="			{ return(XOR_ASSIGN); }
"|="			{ return(OR_ASSIGN); }
">>"			{ return(RIGHT_OP); }
"<<"			{ return(LEFT_OP); }
"++"			{ return(INC_OP); }
"--"			{ return(DEC_OP); }
"->"			{ return(PTR_OP); }
"&&"			{ return(AND_OP); }
"||"			{ return(OR_OP); }
"<="			{ return(LE_OP); }
">="			{ return(GE_OP); }
"=="			{ return(EQ_OP); }
"!="			{ return(NE_OP); }
";"			{ return(';'); }
("{"|"<%")		{ return('{'); }
("}"|"%>")		{ return('}'); }
","			{ return(','); }
":"			{ return(':'); }
"="			{ return('='); }
"("			{ return('('); }
")"			{ return(')'); }
("["|"<:")		{ return('['); }
("]"|":>")		{ return(']'); }
"."			{ return('.'); }
"&"			{ return('&'); }
"!"			{ return('!'); }
"~"			{ return('~'); }
"-"			{ return('-'); }
"+"			{ return('+'); }
"*"			{ return('*'); }
"/"			{ return('/'); }
"%"			{ return('%'); }
"<"			{ return('<'); }
">"			{ return('>'); }
"^"			{ return('^'); }
"|"			{ return('|'); }
"?"			{ return('?'); }
[\n] {new_line();}
[ \t\v\f]		{ }
.			{ /* ignore bad characters */ }

%%

int yywrap()
{
	return(1);
}



int comment()
{
	char c, c1;
loop:
	while ((c = input()) != '*' && c != 0);
	if ((c1 = input()) == '/' && c != 0) return (0);
	else 
	{
		if(!c1) return (0);
		goto loop;
	}
}

void line_comment()
{
	char c, c1 = '\0', c2;
label:
	while ((c = input()) != '\n' && c != 0)
	{
		c2 = c1;
		c1 = c;
	}
	if (c2 =='\\') goto label;
}

int column = 0;

void count()
{
	int i;
	sscanf(yytext, "%s", yylval.identifier);
	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	//ECHO;
}


int check_type()
{
	if (is_type_name(yytext)) {
		return(TYPE_NAME); 
	}
	return(IDENTIFIER);
}

void check() {
	for (size_t q = 0; q < strlen(yytext); ++q) {
		if (*(yytext + q) == '\n') {
			new_line();	
		}
	}
}