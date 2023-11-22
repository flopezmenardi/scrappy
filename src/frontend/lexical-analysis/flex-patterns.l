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
url             https?:\/\/([a-zA-Z0-9$\-_.+!*'(),%:@&=]+(?:\/[a-zA-Z0-9$\-_.+!*'(),%:@&=]*)*(?:\?[a-zA-Z0-9$\-_.+!*'(),%:@&=\/]*)?(?:#[a-zA-Z0-9$\-_.+!*'(),%:@&=\/]*)?)
word            [a-zA-Z0-9$\-_.+!*',%:@&/]+
/* saque ; y = para que no me tome como variable */
/* password        ^.*$ */
variable        [a-zA-Z0-9_]+

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"from"                                      { return FromPrefixPatternAction(yytext, yyleng);}
"{"											{ return OpenCurlyBracketPatternAction(yytext); }
"}"											{ return CloseCurlyBracketPatternAction(yytext); }
"retrieve"									{ return RetrievePrefixPatternAction(yytext, yyleng); }
"url"   									{ return URLPatternAction(yytext, yyleng); }
"to"   									    { return ToPrefixPatternAction(yytext, yyleng); }
"auth"   									{ return AuthPrefixPatternAction(yytext, yyleng); }
"id"                                        { return IdPatternAction(yytext, yyleng); }
"username"                                  { return UsernamePatternAction(yytext, yyleng); }
"password"                                  { return PasswordPatternAction(yytext, yyleng); }
"var"                                       { return VarPatternAction(yytext, yyleng); }

"html"                                      { return HtmlPatternAction(yytext, yyleng); }
"head"                                      { return HeadPatternAction(yytext, yyleng); }
"title"                                     { return TitlePatternAction(yytext, yyleng); }
"body"                                      { return BodyPatternAction(yytext, yyleng); }
"h1"                                        { return H1PatternAction(yytext, yyleng); }
"h2"                                        { return H2PatternAction(yytext, yyleng); }
"h3"                                        { return H3PatternAction(yytext, yyleng); }
"h4"                                        { return H4PatternAction(yytext, yyleng); }
"h5"                                        { return H5PatternAction(yytext, yyleng); }
"h6"                                        { return H6PatternAction(yytext, yyleng); }
"img"                                       { return ImagePatternAction(yytext, yyleng); }
"a"                                         { return LinkPatternAction(yytext, yyleng); }
"p"                                         { return ParagraphPatternAction(yytext, yyleng); }
"ul"                                        { return UnorderedListPatternAction(yytext, yyleng); }
"ol"                                        { return OrderedListPatternAction(yytext, yyleng); }
"dl"                                        { return DefinitionListPatternAction(yytext, yyleng); }
"li"                                        { return ListItemPatternAction(yytext, yyleng); }
"div"                                       { return DivPatternAction(yytext, yyleng); }

";"											{ return SemicolonPatternAction(yytext); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

{url}                                       { return UrlTypePatternAction(yytext, yyleng); }
{word}                                      { return WordTypePatternAction(yytext, yyleng); }
{variable}                                  { return VariableTypePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
