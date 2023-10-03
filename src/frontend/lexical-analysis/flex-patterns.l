%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit			[0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
url             https?:\/\/([a-zA-Z0-9$\-_.+!*'(),%;:@&=]+(?:\/[a-zA-Z0-9$\-_.+!*'(),%;:@&=]*)*(?:\?[a-zA-Z0-9$\-_.+!*'(),%;:@&=\/]*)?(?:#[a-zA-Z0-9$\-_.+!*'(),%;:@&=\/]*)?)
word            [a-zA-Z0-9$\-_.+!*',%:@&/]+
/* saque ; y = para que no me tome como variable */
/* password        ^.*$ */
variable        [a-zA-Z0-9_]+

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"FROM"                                      { return FromPrefixPatternAction(yytext, yyleng);}
"{"											{ return OpenCurlyBracketPatternAction(yytext); }
"}"											{ return CloseCurlyBracketPatternAction(yytext); }
"RETRIEVE"									{ return RetrievePrefixPatternAction(yytext, yyleng); }
"URL"   									{ return URLPatternAction(yytext, yyleng); }
"TO"   									    { return ToPrefixPatternAction(yytext, yyleng); }
"AUTH"   									{ return AuthPrefixPatternAction(yytext, yyleng); }
"ID"                                        { return IdPatternAction(yytext, yyleng); }
"USERNAME"                                  { return UsernamePatternAction(yytext, yyleng); }
"PASSWORD"                                  { return PasswordPatternAction(yytext, yyleng); }
"VAR"                                       { return VarPatternAction(yytext, yyleng); }

";"											{ return SemicolonPatternAction(yytext); }
"="                                         { return EqualsPatternAction(yytext); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

{url}                                       { return UrlTypePatternAction(yytext, yyleng); }
{word}                                      { return WordTypePatternAction(yytext, yyleng); }
{variable}                                  { return VariableTypePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
