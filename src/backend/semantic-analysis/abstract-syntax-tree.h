#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/

typedef struct VarLineNode VarLineNode;

struct VarLineNode {
	char * varName;
	char * value;
	VarLineNode * next;
};

typedef struct VarBlockNode VarBlockNode;

struct VarBlockNode {
	VarLineNode * varLine;
};

typedef struct FromLineNode FromLineNode;

struct FromLineNode {
	char * url;
	FromLineNode * next;
};

typedef struct FromBlockNode FromBlockNode;

struct FromBlockNode {
	FromLineNode * fromLine;
};

typedef enum {
	TAG_HTML,
	TAG_HEAD,
	TAG_TITLE,
	TAG_BODY,
	TAG_H1,
	TAG_H2,
	TAG_H3,
	TAG_H4,
	TAG_H5,
	TAG_H6,
	TAG_IMG,
	TAG_A,
	TAG_P,
	TAG_UL,
	TAG_OL,
	TAG_DL,
	TAG_LI,
	TAG_DIV
} Tag;

typedef struct RetrieveLineNode RetrieveLineNode;

struct RetrieveLineNode {
	Tag * tag;
	char * varName;
	RetrieveLineNode * next;
};

typedef struct RetrieveBlockNode RetrieveBlockNode;

struct RetrieveBlockNode {
	RetrieveLineNode * retrieveLine;
};

typedef struct ToLineNode ToLineNode;

struct ToLineNode {
	char * path;
	ToLineNode * next;
};

typedef struct ToBlockNode ToBlockNode;

struct ToBlockNode {
	ToLineNode * toLine;
};

typedef struct AuthLineNode AuthLineNode;

struct AuthLineNode {
	char * username;
	char * password;
};

typedef struct AuthBlockNode AuthBlockNode;

struct AuthBlockNode {
	AuthLineNode * authLine;
};

typedef struct {
	VarBlockNode * varBlock;
	FromBlockNode * fromBlock;
	RetrieveBlockNode * retrieveBlock;
	ToBlockNode * toBlock;
	AuthBlockNode * authBlock;
} Program;

#endif
