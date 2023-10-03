 %{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int expression;
	int factor;
	int constant;
	// -------------------------- Nuestros no terminales
	int fromblock;
	int retrieveblock;
	int fromline;
	int retrieveline;
	int retrievelinerec;
	int toblock;
	int toline;
	int tolinerec;
	int authblock;
	int authline;
	int varblock;
	int varlinerec;
	int varline;

	// -------------------------- Nuestros data types
	char * string;

	// Terminales.
	token token;
	int integer;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
// ------------------------ Nuestros terminales
%token <token> FROM
%token <token> OPEN_CURLY_BRACKET
%token <token> CLOSE_CURLY_BRACKET
%token <token> RETRIEVE
%token <token> URL
%token <token> TO
%token <token> AUTH
%token <token> ID
%token <token> SEMICOLON
%token <token> USERNAME
%token <token> PASSWORD
%token <token> VAR
%token <token> EQUALS

// ------------------------ Nuestros data types
%token <string> TYPE_URL
%token <string> TYPE_WORD
%token <string> TYPE_VARIABLE

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
// ------------------------ Nuestros no-terminales
%type <fromblock> fromblock
%type <retrieveblock> retrieveblock
%type <fromline> fromline
%type <retrieveline> retrieveline
%type <retrievelinerec> retrievelinerec
%type <toblock> toblock
%type <toline> toline
%type <tolinerec> tolinerec
%type <authblock> authblock
%type <authline> authline
%type <varblock> varblock
%type <varlinerec> varlinerec
%type <varline> varline

// El símbolo inicial de la gramatica.
%start program

%%

program: varblock fromblock retrieveblock toblock authblock												{ $$ = ProgramGrammarAction($1); }
	;

varblock: varlinerec																					{ $$ = 0; }
	| %empty
varlinerec: varline varlinerec																			{ $$ = 0; }
	| %empty																								
varline: VAR TYPE_WORD EQUALS TYPE_WORD SEMICOLON														{ $$ = 0; }

fromblock: FROM OPEN_CURLY_BRACKET fromline CLOSE_CURLY_BRACKET											{ $$ = 0; }

fromline: URL TYPE_URL SEMICOLON																		{ $$ = 0; }

retrieveblock: RETRIEVE OPEN_CURLY_BRACKET retrieveline retrievelinerec CLOSE_CURLY_BRACKET				{ $$ = 0; }

retrievelinerec: retrieveline retrievelinerec															{ $$ = 0; }
	| %empty																								
retrieveline: TYPE_WORD ID TYPE_WORD SEMICOLON															{ $$ = 0; }

toblock: TO OPEN_CURLY_BRACKET toline tolinerec CLOSE_CURLY_BRACKET										{ $$ = 0; }

tolinerec: toline tolinerec																				{ $$ = 0; }
	| %empty
toline: TYPE_WORD SEMICOLON																				{ $$ = 0; }

authblock: AUTH OPEN_CURLY_BRACKET authline CLOSE_CURLY_BRACKET											{ $$ = 0; }
	| %empty																						
authline: USERNAME TYPE_WORD SEMICOLON PASSWORD TYPE_WORD SEMICOLON						    			{ $$ = 0; }

%%
