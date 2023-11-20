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
	Conystant constant;
	...
	*/

	// No-terminales (frontend).
	Program * program;
	// -------------------------- Nuestros no terminales
	FromBlockNode * fromblock;
	RetrieveBlockNode * retrieveblock;
	FromLineNode * fromline;
	FromLineNode * fromlinerec;
	RetrieveLineNode * retrieveline;
	RetrieveLineNode * retrievelinerec;
	ToBlockNode * toblock;
	ToLineNode * toline;
	ToLineNode * tolinerec;
	AuthBlockNode * authblock;
	AuthLineNode * authline;
	VarBlockNode * varblock;
	VarLineNode * varline;
	VarLineNode * varlinerec;
	Tag * tag;

	// -------------------------- Nuestros data types
	char * string;

	// Terminales.
	token token;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR
// ------------------------ Nuestros terminales
%token <string> FROM
%token <string> OPEN_CURLY_BRACKET
%token <string> CLOSE_CURLY_BRACKET
%token <string> RETRIEVE
%token <string> URL
%token <string> TO
%token <string> AUTH
%token <string> ID
%token <string> SEMICOLON
%token <string> USERNAME
%token <string> PASSWORD

%token <tag> HTML
%token <tag> HEAD
%token <tag> TITLE
%token <tag> BODY
%token <tag> H1
%token <tag> H2
%token <tag> H3
%token <tag> H4
%token <tag> H5
%token <tag> H6
%token <tag> VAR
%token <tag> IMG
%token <tag> A
%token <tag> P
%token <tag> UL
%token <tag> OL
%token <tag> DL
%token <tag> LI


// ------------------------ Nuestros data types
%token <string> TYPE_URL
%token <string> TYPE_WORD
%token <string> TYPE_VARIABLE

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
// ------------------------ Nuestros no-terminales
%type <fromblock> fromblock
%type <retrieveblock> retrieveblock
%type <fromline> fromline
%type <fromlinerec> fromlinerec
%type <retrieveline> retrieveline
%type <retrievelinerec> retrievelinerec
%type <toblock> toblock
%type <toline> toline
%type <tolinerec> tolinerec
%type <authblock> authblock
%type <authline> authline
%type <varblock> varblock
%type <varline> varline
%type <varlinerec> varlinerec
%type <tag> tag

// El símbolo inicial de la gramatica.
%start program

%%

program: varblock fromblock retrieveblock toblock authblock												{ $$ = ProgramGrammarAction($1, $2, $3, $4, $5); }
	;

varblock: varlinerec																					{ $$ = VarBlockNodeGrammarAction($1); }

varlinerec: varline varlinerec																			{ $$ = NULL; }
	| %empty																							{ $$ = NULL; }
varline: VAR TYPE_WORD TYPE_WORD SEMICOLON																{ $$ = VarLineNodeGrammarAction($2, $3); }

fromblock: FROM OPEN_CURLY_BRACKET fromline fromlinerec CLOSE_CURLY_BRACKET								{ $$ = FromBlockNodeGrammarAction($3); }

fromlinerec: fromline fromlinerec																		{ $$ = NULL; }
	| %empty																							{ $$ = NULL; }
fromline: URL TYPE_URL SEMICOLON																		{ $$ = FromLineNodeGrammarAction($2); }

retrieveblock: RETRIEVE OPEN_CURLY_BRACKET retrieveline retrievelinerec CLOSE_CURLY_BRACKET				{ $$ = RetrieveBlockNodeGrammarAction($3); }

retrievelinerec: retrieveline retrievelinerec															{ $$ = NULL; }
	| %empty																							{ $$ = NULL; }
retrieveline: tag ID TYPE_WORD SEMICOLON																{ $$ = RetrieveLineNodeGrammarAction($1,$3); }
	| tag SEMICOLON																						{ $$ = RetrieveLineNodeGrammarAction($1, NULL); }

tag: HTML																								{ $$ = TagHtmlGrammarAction(); }
	| HEAD																								{ $$ = TagHeadGrammarAction(); }
	| TITLE																								{ $$ = TagTitleGrammarAction(); }
	| BODY																								{ $$ = TagBodyGrammarAction(); }
	| H1																								{ $$ = TagH1GrammarAction(); }
	| H2																								{ $$ = TagH2GrammarAction(); }
	| H3																								{ $$ = TagH3GrammarAction(); }
	| H4																								{ $$ = TagH4GrammarAction(); }
	| H5																								{ $$ = TagH5GrammarAction(); }
	| H6																								{ $$ = TagH6GrammarAction(); }
	| IMG																								{ $$ = TagImageGrammarAction(); }
	| A																									{ $$ = TagLinkGrammarAction(); }
	| P																									{ $$ = TagParagraphGrammarAction(); }
	| UL																								{ $$ = TagUnorderedListGrammarAction(); }
	| OL																								{ $$ = TagOrderedListGrammarAction(); }
	| DL																								{ $$ = TagDefinitionListGrammarAction(); }
	| LI																								{ $$ = TagListItemGrammarAction(); }

toblock: TO OPEN_CURLY_BRACKET toline tolinerec CLOSE_CURLY_BRACKET										{ $$ = ToBlockNodeGrammarAction($3); }

tolinerec: toline tolinerec																				{ $$ = NULL; }
	| %empty																							{ $$ = NULL; }
toline: TYPE_WORD SEMICOLON																				{ $$ = ToLineNodeGrammarAction($1); }

authblock: AUTH OPEN_CURLY_BRACKET authline CLOSE_CURLY_BRACKET											{ $$ = AuthBlockNodeGrammarAction($3); }
	| %empty																							{ $$ = NULL;}
authline: USERNAME TYPE_WORD SEMICOLON PASSWORD TYPE_WORD SEMICOLON						    			{ $$ = AuthLineNodeGrammarAction($2, $5); }

%%
