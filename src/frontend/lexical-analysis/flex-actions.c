#include "../../backend/support/logger.h"
#include "flex-actions.h"
// #include "../../utils/wrapper-functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------


token FromPrefixPatternAction() {
	LogDebug("[Flex] FromPrefixPatternAction: 'FROM'.");
	return FROM;
}

token OpenCurlyBracketPatternAction() {
	LogDebug("[Flex] OpenCurlyBracketPatternAction: '{'.");
	return OPEN_CURLY_BRACKET;
}

token CloseCurlyBracketPatternAction() {
	LogDebug("[Flex] CloseCurlyBracketPatternAction: '}'.");
	return CLOSE_CURLY_BRACKET;
}

token RetrievePrefixPatternAction() {
	LogDebug("[Flex] RetrievePrefixPatternAction: 'RETRIEVE'.");
	return RETRIEVE;
}

token URLPatternAction() {
	LogDebug("[Flex] URLPatternAction: 'URL'.");
	return URL;
}

token ToPrefixPatternAction() {
	LogDebug("[Flex] ToPrefixPatternAction: 'TO'.");
	return TO;
}

token AuthPrefixPatternAction() {
	LogDebug("[Flex] AuthPrefixPatternAction: 'AUTH'.");
	return AUTH;
}

token IdPatternAction() {
	LogDebug("[Flex] IdPrefixPatternAction: 'ID'.");
	return ID;
}

token SemicolonPatternAction() {
	LogDebug("[Flex] SemicolonPatternAction: ';'.");
	return SEMICOLON;
}

token UsernamePatternAction() {
	LogDebug("[Flex] UsernamePatternAction: 'USERNAME'.");
	return USERNAME;
}

token PasswordPatternAction() {
	LogDebug("[Flex] PasswordPatternAction: 'PASSWORD'.");
	return PASSWORD;
}

token VarPatternAction() {
	LogDebug("[Flex] VarPatternAction: 'VAR'.");
	return VAR;
}

token HtmlPatternAction() {
	LogDebug("[Flex] HtmlPatternAction: 'HTML'.");
	return HTML;
}

token HeadPatternAction() {
	LogDebug("[Flex] HeadPatternAction: 'HEAD'.");
	return HEAD;
}

token TitlePatternAction() {
	LogDebug("[Flex] TitlePatternAction: 'TITLE'.");
	return TITLE;
}

token BodyPatternAction() {
	LogDebug("[Flex] BodyPatternAction: 'BODY'.");
	return BODY;
}

token H1PatternAction() {
	LogDebug("[Flex] H1PatternAction: 'H1'.");
	return H1;
}

token H2PatternAction() {
	LogDebug("[Flex] H2PatternAction: 'H2'.");
	return H2;
}

token H3PatternAction() {
	LogDebug("[Flex] H3PatternAction: 'H3'.");
	return H3;
}

token H4PatternAction() {
	LogDebug("[Flex] H4PatternAction: 'H4'.");
	return H4;
}

token H5PatternAction() {
	LogDebug("[Flex] H5PatternAction: 'H5'.");
	return H5;
}

token H6PatternAction() {
	LogDebug("[Flex] H6PatternAction: 'H6'.");
	return H6;
}

token ImagePatternAction() {
	LogDebug("[Flex] ImagePatternAction: 'IMG'.");
	return IMG;
}

token LinkPatternAction() {
	LogDebug("[Flex] LinkPatternAction: 'A'.");
	return A;
}

token ParagraphPatternAction() {
	LogDebug("[Flex] ParagraphPatternAction: 'PARAGRAPH'.");
	return P;
}

token UnorderedListPatternAction() {
	LogDebug("[Flex] UnorderedListPatternAction: 'UNORDERED_LIST'.");
	return UL;
}

token OrderedListPatternAction() {
	LogDebug("[Flex] OrderedListPatternAction: 'ORDERED_LIST'.");
	return OL;
}

token DefinitionListPatternAction() {
	LogDebug("[Flex] DefinitionListPatternAction: 'DEFINITION_LIST'.");
	return DL;
}

token ListItemPatternAction() {
	LogDebug("[Flex] ListItemPatternAction: 'LIST_ITEM'.");
	return LI;
}

// --------------- Nuestras de tipo

token UrlTypePatternAction(const char *lexeme, const int length) {
    LogDebug("UrlTypePatternAction: '%s' (length = %d).", lexeme, length);
    char *url = (char *)calloc(length + 1, sizeof(char));
    strncpy(url, lexeme, length);
    yylval.string = url;

    return TYPE_URL;
}

token WordTypePatternAction(const char *lexeme, const int length) {
	LogDebug("WordTypePatternAction: '%s' (length = %d).", lexeme, length);
	char *word = (char *)calloc(length + 1, sizeof(char));
	strncpy(word, lexeme, length);
	yylval.string = word;

	return TYPE_WORD;
}

token VariableTypePatternAction(const char *lexeme, const int length) {
	LogDebug("VariableTypePatternAction: '%s' (length = %d).", lexeme, length);
	char *variable = (char *)calloc(length + 1, sizeof(char));
	strncpy(variable, lexeme, length);
	yylval.string = variable;

	return TYPE_VARIABLE;
}