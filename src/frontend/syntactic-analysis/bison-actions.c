#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(VarBlockNode * varBlock, FromBlockNode * fromBlock, RetrieveBlockNode * retrieveBlock, ToBlockNode * toBlock, AuthBlockNode * authBlock) {
	LogDebug("[Bison] ProgramGrammarAction");
	
	Program * program = (Program *) calloc(1, sizeof(Program));

	program->varBlock = varBlock;
	program->fromBlock = fromBlock;
	program->retrieveBlock = retrieveBlock;
	program->toBlock = toBlock;
	program->authBlock = authBlock;

	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.program = program;
	return program;
}

VarBlockNode * VarBlockNodeGrammarAction(VarLineNode * varLine){
	LogDebug("[Bison] VarBlockGrammarAction");
	VarBlockNode * varBlock = (VarBlockNode *) calloc(1, sizeof(VarBlockNode));
	varBlock->varLine = varLine;
	return varBlock;
}

VarLineNode * AppendVarLineNodeRec(VarLineNode * varLine, VarLineNode * varLineToAppend){
	if(varLine == NULL){
		return varLineToAppend;
	}
	varLine->next = AppendVarLineNodeRec(varLine->next, varLineToAppend);
	return varLine;
}

VarLineNode * VarLineNodeGrammarAction(char * varName, char * value){
	LogDebug("[Bison] VarLineGrammarAction");
	VarLineNode * varLine = (VarLineNode *) calloc(1, sizeof(VarLineNode));
	varLine->varName = varName;
	varLine->value = value;

	addToVariableList(varName, value);

	return varLine;
}

FromBlockNode * FromBlockNodeGrammarAction(FromLineNode * fromLine){
	LogDebug("[Bison] FromBlockGrammarAction");
	FromBlockNode * fromBlock = (FromBlockNode *) calloc(1, sizeof(FromBlockNode));
	fromBlock->fromLine = fromLine;
	return fromBlock;
}

FromLineNode * FromLineNodeGrammarAction(char * url){
	LogDebug("[Bison] FromLineGrammarAction");
	FromLineNode * fromLine = (FromLineNode *) calloc(1, sizeof(FromLineNode));
	fromLine->url = url;

	addToUrlList(url);

	return fromLine;
}

FromLineNode * AppendFromLineNodeRec(FromLineNode * fromLine, FromLineNode * fromLineToAppend){
	if(fromLine == NULL){
		return fromLineToAppend;
	}
	fromLine->next = AppendFromLineNodeRec(fromLine->next, fromLineToAppend);
	return fromLine;
}

RetrieveBlockNode * RetrieveBlockNodeGrammarAction(RetrieveLineNode * retrieveLine){
	LogDebug("[Bison] RetrieveBlockGrammarAction");
	RetrieveBlockNode * retrieveBlock = (RetrieveBlockNode *) calloc(1, sizeof(RetrieveBlockNode));
	retrieveBlock->retrieveLine = retrieveLine;
	return retrieveBlock;
}

RetrieveLineNode * AppendRetrieveLineNodeRec(RetrieveLineNode * retrieveLine, RetrieveLineNode * retrieveLineToAppend){
	if(retrieveLine == NULL){
		return retrieveLineToAppend;
	}
	retrieveLine->next = AppendRetrieveLineNodeRec(retrieveLine->next, retrieveLineToAppend);
	return retrieveLine;
}

RetrieveLineNode * RetrieveLineNodeGrammarAction(Tag * tag, char * varName){
	LogDebug("[Bison] RetrieveLineGrammarAction");
	RetrieveLineNode * retrieveLine = (RetrieveLineNode *) calloc(1, sizeof(RetrieveLineNode));
	retrieveLine-> tag = tag;
	retrieveLine->varName = varName;

	addToTagList(getTagString(tag), varName);

	return retrieveLine;
}

Tag * TagHtmlGrammarAction(){
	LogDebug("[Bison] TagHtmlGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_HTML;
	return tag;
}

Tag * TagHeadGrammarAction(){
	LogDebug("[Bison] TagHeadGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_HEAD;
	return tag;
}

Tag * TagTitleGrammarAction(){
	LogDebug("[Bison] TagTitleGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_TITLE;
	return tag;
}

Tag * TagBodyGrammarAction(){
	LogDebug("[Bison] TagBodyGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_BODY;
	return tag;
}

Tag * TagH1GrammarAction(){
	LogDebug("[Bison] TagH1GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H1;
	return tag;
}

Tag * TagH2GrammarAction(){
	LogDebug("[Bison] TagH2GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H2;
	return tag;
}

Tag * TagH3GrammarAction(){
	LogDebug("[Bison] TagH3GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H3;
	return tag;
}

Tag * TagH4GrammarAction(){
	LogDebug("[Bison] TagH4GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H4;
	return tag;
}

Tag * TagH5GrammarAction(){
	LogDebug("[Bison] TagH5GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H5;
	return tag;
}

Tag * TagH6GrammarAction(){
	LogDebug("[Bison] TagH6GrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_H6;
	return tag;
}

Tag * TagImageGrammarAction(){
	LogDebug("[Bison] TagImageGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_IMG;
	return tag;
}

Tag * TagLinkGrammarAction(){
	LogDebug("[Bison] TagLinkGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_A;
	return tag;
}

Tag * TagParagraphGrammarAction(){
	LogDebug("[Bison] TagParagraphGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_P;
	return tag;
}

Tag * TagUnorderedListGrammarAction(){
	LogDebug("[Bison] TagUnorderedListGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_UL;
	return tag;
}

Tag * TagOrderedListGrammarAction(){
	LogDebug("[Bison] TagOrderedListGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_OL;
	return tag;
}

Tag * TagDefinitionListGrammarAction(){
	LogDebug("[Bison] TagDefinitionListGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_DL;
	return tag;
}

Tag * TagListItemGrammarAction(){
	LogDebug("[Bison] TagListItemGrammarAction");
	Tag * tag = (Tag *) calloc(1, sizeof(Tag));
	*tag = TAG_LI;
	return tag;
}

ToBlockNode * ToBlockNodeGrammarAction(ToLineNode * toLine){
	LogDebug("[Bison] ToBlockGrammarAction");
	ToBlockNode * toBlock = (ToBlockNode *) calloc(1, sizeof(ToBlockNode));
	toBlock->toLine = toLine;
	return toBlock;
}

ToLineNode * AppendToLineNodeRec(ToLineNode * toLine, ToLineNode * toLineToAppend){
	if(toLine == NULL){
		return toLineToAppend;
	}
	toLine->next = AppendToLineNodeRec(toLine->next, toLineToAppend);
	return toLine;
}

ToLineNode * ToLineNodeGrammarAction(char * path){
	LogDebug("[Bison] ToLineGrammarAction");
	ToLineNode * toLine = (ToLineNode *) calloc(1, sizeof(ToLineNode));
	toLine->path = path;

	addToPathList(path);

	return toLine;
}

AuthBlockNode * AuthBlockNodeGrammarAction(AuthLineNode * authLine){
	LogDebug("[Bison] AuthBlockGrammarAction");
	AuthBlockNode * authBlock = (AuthBlockNode *) calloc(1, sizeof(AuthBlockNode));
	authBlock->authLine = authLine;
	return authBlock;
}

AuthLineNode * AuthLineNodeGrammarAction(char * username, char * password){
	LogDebug("[Bison] AuthLineGrammarAction");
	AuthLineNode * authLine = (AuthLineNode *) calloc(1, sizeof(AuthLineNode));
	authLine->username = username;
	authLine->password = password;

	setAuth(username, password);

	return authLine;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
