#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Patrones terminales del lenguaje diseñado.
// --------------- Nuestras definiciones
token FromPrefixPatternAction();
token OpenCurlyBracketPatternAction();
token CloseCurlyBracketPatternAction();
token RetrievePrefixPatternAction();
token URLPatternAction();
token ToPrefixPatternAction();
token AuthPrefixPatternAction();
token IdPatternAction();
token SemicolonPatternAction();
token UsernamePatternAction();
token PasswordPatternAction();
token VarPatternAction();
token HtmlPatternAction();
token HeadPatternAction();
token TitlePatternAction();
token BodyPatternAction();
token H1PatternAction();
token H2PatternAction();
token H3PatternAction();
token H4PatternAction();
token H5PatternAction();
token H6PatternAction();
token ImagePatternAction();
token LinkPatternAction();
token ParagraphPatternAction();
token UnorderedListPatternAction();
token OrderedListPatternAction();
token DefinitionListPatternAction();
token ListItemPatternAction();

// --------------- Nuestras de tipo
token UrlTypePatternAction(const char* lexeme, const int length);
token WordTypePatternAction(const char* lexeme, const int length);
token VariableTypePatternAction(const char* lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
