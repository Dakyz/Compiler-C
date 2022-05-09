%{
#include <stdio.h>
#include "list.h"

List<struct idElem> elements = 
	List<struct idElem>();
bool isFuncPushed = false;
int gType = 0;
bool isType = false;
bool gGlobal = true;
int line = 1;
void new_line() {++line;}
extern FILE* yyin;
extern char yytext[];
extern int column;
int main(int argc, char** argv);
void yyerror(const char *s);
extern "C" {
    int yylex(void);
}
bool is_find(char * id);
bool is_type_name(char * id);
%}
%union{
	char identifier[256];
	int number;
}
%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit

%%

primary_expression
	: IDENTIFIER {
	if (!elements.is_find(yylval.identifier, gGlobal)) {
		char tmp[400];
		snprintf(tmp, 400, "implicit declaration %s", yylval.identifier);
		yyerror(tmp);
	}
	if (!elements.is_init(yylval.identifier)) {
		fprintf(stderr, "\033[0;35m");
		fprintf(stderr, "warning %d: %s not initialized\n", line, yylval.identifier);
		fprintf(stderr, "\033[0m");
	}
	}
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier 
	| type_qualifier declaration_specifiers
	;

init_declarator_list
	: init_declarator {
		if (isType) {
			elements.push_type();
		}
		else if (!isFuncPushed) {
			elements.push_back(gGlobal);
		}
	}
	| init_declarator_list ',' init_declarator {
		elements.push_back(gGlobal);
	}
	;

init_declarator
	: declarator
	| declarator '=' {
		elements.localTmp->initialized = true;
	} initializer
	;

storage_class_specifier
	: TYPEDEF {
		isType = true;
	}
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier 
	: VOID {
		gType = void_;
	}
	| CHAR {
		gType = char_;
	}
	| SHORT {
		gType = int_;
	}
	| INT {
		gType = int_;
	}
	| LONG {
		gType = int_;
	}
	| FLOAT {
		gType = float_;
	}
	| DOUBLE {
		gType = float_;
	}
	| SIGNED 
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME 
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT { 
		gType = struct_;
	}
	| UNION {
		gType = union_;
	}
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER {
		strncpy(elements.localTmp->name, yylval.identifier, 256);
		elements.localTmp->type = int_;
		elements.localTmp->initialized = true;
		elements.push_back(true);
	}
	| IDENTIFIER '=' {
		strncpy(elements.localTmp->name, yylval.identifier, 256);
		elements.localTmp->type = int_;
		elements.localTmp->initialized = true;
		elements.push_back(true);
	} constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	| direct_declarator 
	;

direct_declarator
	: IDENTIFIER   {
		if (isType) {
			strncpy(elements.type,  yylval.identifier, 256);
		}
		else {
			if (elements.is_find(yylval.identifier, gType)) {
				char tmp[400];
				snprintf(tmp, 400, "multiple definition '%s'", yylval.identifier);
			}
			strncpy(elements.localTmp->name, yylval.identifier, 256);
			elements.localTmp->type = gType;
		}
	}
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '('  {
		elements.localTmp->callable = true;
		elements.push_back(gGlobal);
		gGlobal = false;
		isFuncPushed = true;
	}
	parameter_type_list ')'
	| direct_declarator '('  {
		elements.localTmp->callable = true;
		elements.push_back(gGlobal);
		gGlobal = false;
		isFuncPushed = true;
	}
	identifier_list ')'
	| direct_declarator '('  {
		elements.localTmp->callable = true;
		elements.push_back(gGlobal);
		gGlobal = false;
		isFuncPushed = true;
	} ')'
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration  {
		elements.localTmp->initialized = true;
		elements.push_back(gGlobal);
	}
	| parameter_list ',' parameter_declaration  {
		elements.localTmp->initialized = true;
		elements.push_back(gGlobal);
	}
	;

parameter_declaration
	: declaration_specifiers declarator 
	| declaration_specifiers abstract_declarator 
	| declaration_specifiers 
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' {isFuncPushed = false;} func_definition {
		elements.init_last();
	}
	;

func_definition
	: '}'
	| statement func_definition
	| declaration func_definition
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' {
		elements.up_level();
	} if_body {
		elements.nested_clear();
	}
	| SWITCH '(' expression ')'  {
		elements.up_level();
	} statement {
		elements.nested_clear();
	}
	;

if_body
	: statement 
	| statement ELSE {
		elements.nested_clear();
		elements.up_level();
	} statement

iteration_statement
	: WHILE '(' expression ')' {
		elements.up_level();
	} statement {
		elements.nested_clear();
	}
	| DO {
		elements.up_level();
	} statement WHILE '(' expression ')' ';' {
		elements.nested_clear();
	}
	| FOR '(' {
		elements.up_level();
	} for_body {
		elements.nested_clear();
	}
	;

for_body
	: expression_statement expression_statement ')' statement
	| expression_statement expression_statement expression ')' statement
	| declaration_list expression_statement ')' statement 
	| declaration_list expression_statement expression ')' statement 
	;
jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition {
		gGlobal = true;
		isType = isFuncPushed = false;
		elements.clear_members();
		elements.is_duplicate();
	}
	| declaration  {
		gGlobal = true;
		isType = isFuncPushed = false;
		elements.clear_members();
		elements.is_duplicate();
	}
	;

function_definition
	: declaration_specifiers declarator {
		gGlobal = false;
	} declaration_list compound_statement
	| declaration_specifiers declarator {
		gGlobal = false;
	} compound_statement
	| declarator {
		gGlobal = false;
	} declaration_list compound_statement
	| declarator  {
		gGlobal = false;
	} compound_statement
	;

%%
int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s filename\n", *argv);
		return -1;
	}
	char * cmd;
	asprintf(&cmd, "echo \"typedef float _Float128;\n\" > tmp.i &"
			"echo \"typedef char* __builtin_va_list;\n\" >> tmp.i &"
			"gcc -E %s >> tmp.i", *(argv + 1));
	system(cmd);
	free(cmd);
	yyin = fopen("tmp.i","r");
	if (!yyin){
		fprintf(stderr, "file tmp.i doesn't exist\n");
		return -1;
	}
	
	if(!feof (yyin)) {
		yyparse();
	}
	return 0;
}

void yyerror(const char *s)
{
	fprintf(stderr, "\033[1;31m");
	fprintf(stderr, "error %d: %s\n", line, s);
	fprintf(stderr, "\033[0m");
	exit(0);
}

bool is_type_name(char * id) {
	return elements.is_type(id);
}

bool is_find(char * id) {
	return elements.is_find(id, gGlobal);
}