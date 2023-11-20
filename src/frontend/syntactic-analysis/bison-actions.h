#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/semantic-analysis/symbol-table.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
Program * ProgramGrammarAction(VarBlockNode * varBlock, FromBlockNode * fromBlock, RetrieveBlockNode * retrieveBlock, ToBlockNode * toBlock, AuthBlockNode * authBlock);

VarBlockNode * VarBlockNodeGrammarAction(VarLineNode * varLine);
VarLineNode * AppendVarLineNodeRec(VarLineNode * varLine, VarLineNode * varLineToAppend);
VarLineNode * VarLineNodeGrammarAction(char * varName, char * value);
FromBlockNode * FromBlockNodeGrammarAction(FromLineNode * fromLine);
FromLineNode * FromLineNodeGrammarAction(char * url);
FromLineNode * AppendFromLineNodeRec(FromLineNode * fromLine, FromLineNode * fromLineToAppend);
RetrieveBlockNode * RetrieveBlockNodeGrammarAction(RetrieveLineNode * retrieveLine);
RetrieveLineNode * AppendRetrieveLineNodeRec(RetrieveLineNode * retrieveLine, RetrieveLineNode * retrieveLineToAppend);
RetrieveLineNode * RetrieveLineNodeGrammarAction(Tag * tag, char * varName);
Tag * TagHtmlGrammarAction();
Tag * TagHeadGrammarAction();
Tag * TagTitleGrammarAction();
Tag * TagBodyGrammarAction();
Tag * TagH1GrammarAction();
Tag * TagH2GrammarAction();
Tag * TagH3GrammarAction();
Tag * TagH4GrammarAction();
Tag * TagH5GrammarAction();
Tag * TagH6GrammarAction();
Tag * TagImageGrammarAction();
Tag * TagLinkGrammarAction();
Tag * TagParagraphGrammarAction();
Tag * TagUnorderedListGrammarAction();
Tag * TagOrderedListGrammarAction();
Tag * TagDefinitionListGrammarAction();
Tag * TagListItemGrammarAction();
ToBlockNode * ToBlockNodeGrammarAction(ToLineNode * toLine);
ToLineNode * AppendToLineNodeRec(ToLineNode * toLine, ToLineNode * toLineToAppend);
ToLineNode * ToLineNodeGrammarAction(char * path);
AuthBlockNode * AuthBlockNodeGrammarAction(AuthLineNode * authLine);
AuthLineNode * AuthLineNodeGrammarAction(char * username, char * password);

#endif
