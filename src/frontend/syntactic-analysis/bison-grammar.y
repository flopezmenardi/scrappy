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
	int innerfromblock;
	int toblock;
	int authblock;

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


%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant
// ------------------------ Nuestros no-terminales
%type <fromblock> fromblock
%type <retrieveblock> retrieveblock
%type <innerfromblock> innerfromblock
%type <toblock> toblock
%type <authblock> authblock

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%
/*
program: expression													{ $$ = ProgramGrammarAction($1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	; */

program: fromblock retrieveblock toblock authblock									{ $$ = ProgramGrammarAction($1); }
	;

fromblock : FROM OPEN_CURLY_BRACKET innerfromblock CLOSE_CURLY_BRACKET

innerfromblock: URL

retrieveblock: RETRIEVE OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET

toblock: TO OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET

authblock: AUTH OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET
%%
