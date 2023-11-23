#include <stdio.h>
#include <stdlib.h>
#include "symbol-table.h"

symbol_table * symbolTable = NULL;

variable_list * variableList = NULL;

list * urlList = NULL;

tag_list * tagList = NULL;

list * pathList = NULL;

char * username = NULL;

char * password = NULL;

list * errorList = NULL;

symbol_table * initSymbolTable(){
    symbol_table * aux = (symbol_table*)calloc(1, sizeof(symbol_table));
    if(aux == NULL){
        printf("Error: No se pudo reservar memoria para la tabla de símbolos.\n");
        exit(1);
    }
    symbolTable = aux;

    initVariableList();
    initList(&urlList);
    initTagList();
    initList(&pathList);
    initList(&errorList);

    symbolTable->variable_list = variableList;
    symbolTable->url_list = urlList;
    symbolTable->tag_list = tagList;
    symbolTable->path_list = pathList;
    symbolTable->error_list = errorList;

    return symbolTable;
}

void freeSymbolTable(){
    if(symbolTable == NULL){
        return;
    }
    freeVariableList();
    freeList(&urlList);
    freeTagList();
    freeList(&pathList);
    free(symbolTable);
}

symbol_table * getSymbolTable(){
    return symbolTable;
}

void printSymbolTable(){
    if(symbolTable == NULL){
        return;
    }
    if(hasVariables){
        printf("Variables:\n");
        printVariableList();
    }
    if(hasUrls){
        printf("URLs:\n");
        printUrlList();
    }
    if(hasTags){
        printf("Tags:\n");
        printTagList();
    }
    if(hasPaths){
        printf("Paths:\n");
        printPathList();
    }
    if(hasAuth){
        printf("Auth:\n");
        printf("Username: %s\n", symbolTable->username);
        printf("Password: %s\n", symbolTable->password);
    }
}

// --------------------------------------------------------------------------------------   

void initList(list **param_list) {
    *param_list = (list *)malloc(sizeof(list));
    if (*param_list == NULL) {
        printf("Error: Couldn't allocate memory for the list.\n");
        exit(1);
    }
    (*param_list)->head = NULL;
    (*param_list)->tail = NULL;
}

void freeList(list **param_list) {
    if (*param_list == NULL) {
        return;
    }

    node *current = (*param_list)->head;
    while (current != NULL) {
        node *next = current->next;
        free(current->string);
        free(current);
        current = next;
    }

    free(*param_list);
    *param_list = NULL;
}

void addToList(char *string, list **param_list) {
    if (*param_list == NULL) {
        printf("Error: The list was not initialized.\n");
        exit(1);
    }

    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Error: Couldn't allocate memory for the node.\n");
        exit(1);
    }
    newNode->string = strdup(string);
    newNode->next = NULL;

    if ((*param_list)->tail == NULL) {
        // Lista vacía
        (*param_list)->head = newNode;
        (*param_list)->tail = newNode;
    } else {
        // Inserta al final de la lista
        (*param_list)->tail->next = newNode;
        (*param_list)->tail = newNode;
    }
}

void addToUrlList(char *string){
    addToList(string, &urlList);
}

void addToPathList(char *string){
    addToList(string, &pathList);
}

void addToErrorList(char *string){
    addToList(string, &errorList);
}

list * getUrlList(){
    return urlList;
}

list * getPathList(){
    return pathList;
}

void printList(list *param_list) {
    if (param_list == NULL) {
        printf("Error: The list is NULL.\n");
        return;
    }

    node *current = param_list->head;
    while (current != NULL) {
        printf("%s\n", current->string);
        current = current->next;
    }
}

void printUrlList(){
    printf("URLs:\n");
    printList(urlList);
}

void printPathList(){
    printf("Paths:\n");
    printList(pathList);
}

void printErrorList(){
    printf("Errores:\n");
    printList(errorList);
}

int hasUrls(){
    return urlList->head != NULL;
}

int hasPaths(){
    return pathList->head != NULL;
}

int hasAuth(){
    return username != NULL && password != NULL;
}

int hasErrors(){
    return errorList->head != NULL;
}

void setAuth(char *username, char *password) {
    // Reserva memoria para el username y copia su contenido
    symbolTable->username = strdup(username);
    if (symbolTable->username == NULL) {
        printf("Error: Couldn't allocate memory for the username.\n");
        exit(1);
    }

    // Reserva memoria para la contraseña y copia su contenido
    symbolTable->password = strdup(password);
    if (symbolTable->password == NULL) {
        // Limpiar la memoria previamente reservada
        free(symbolTable->username);
        printf("Error: Couldn't allocate memory for the password.\n");
        exit(1);
    }

}

char *substituteVariables(char *original) {
    char *result = strdup(original); // Duplicar el string original
    char *cursor = result;

    // Iterar sobre el string resultante
    while (*cursor != '\0') {
        // Buscar el siguiente caracter '$'
        if (*cursor == '$') {
            // Encontrar el final del nombre de la variable
            char *varStart = cursor + 1;
            char *varEnd = strpbrk(varStart, " \t\n\r/");
            if (varEnd == NULL) {
                varEnd = varStart + strlen(varStart);
            }

            // Extraer el final del nombre de la variable
            size_t varLength = varEnd - varStart;
            char *varName = malloc(varLength + 1);
            strncpy(varName, varStart, varLength);
            varName[varLength] = '\0';

            // Buscar la variable en la tabla de símbolos
            variable_node *currentVar = symbolTable->variable_list->head;
            char *varValue = NULL;
            while (currentVar != NULL) {
                if (strcmp(currentVar->name, varName) == 0) {
                    varValue = currentVar->value;
                    break;
                }
                currentVar = currentVar->next;
            }

            // Si la variable no existe, agregar un error a la lista
            if (varValue == NULL && strcmp(varName, "HOME") != 0) {
                addToErrorList("Variable not found: ");
                addToErrorList(varName);
            }

            // Susituir la variable en el string resultante
            if (varValue != NULL) {
                size_t prefixLength = varStart - result - 1;
                size_t suffixLength = strlen(varEnd);
                size_t resultLength = prefixLength + strlen(varValue) + suffixLength;

                char *newResult = malloc(resultLength + 1);
                strncpy(newResult, result, prefixLength);
                strcpy(newResult + prefixLength, varValue);
                strcpy(newResult + prefixLength + strlen(varValue), varEnd);

                free(result);
                result = newResult;

                // Mover el cursor al siguiente caracter después de la variable
                cursor = newResult + prefixLength + strlen(varValue);
            } else {
                // Mover el cursor al siguiente caracter después de '$'
                cursor = varEnd;
            }

            // Liberar la memoria reservada para el nombre de la variable
            free(varName);
        } else {
            // Mover el cursor al siguiente caracter
            cursor++;
        }
    }

    return result;
}


void interpolate() {
    // Interpolar en url_list
    node *urlNode = symbolTable->url_list->head;
    while (urlNode != NULL) {
        char *interpolatedString = substituteVariables(urlNode->string);
        free(urlNode->string);
        urlNode->string = interpolatedString;

        // Pasa al siguiente nodo
        urlNode = urlNode->next;
    }

    // Interpolar en tag_list
    tag_node *tag = symbolTable->tag_list->head;
    while (tag != NULL) {
        char *interpolatedName = substituteVariables(tag->name);
        free(tag->name);
        tag->name = interpolatedName;

        char *interpolatedId = substituteVariables(tag->id);
        free(tag->id);
        tag->id = interpolatedId;

        // Pasa al siguiente nodo
        tag = tag->next;
    }

    // Interpolar en path_list
    node *pathNode = symbolTable->path_list->head;
    while (pathNode != NULL) {
        char *interpolatedString = substituteVariables(pathNode->string);
        free(pathNode->string);
        pathNode->string = interpolatedString;

        // Pasa al siguiente nodo
        pathNode = pathNode->next;
    }

    // Interpolar en username
    if (symbolTable->username != NULL) {
        char *interpolatedString = substituteVariables(symbolTable->username);
        free(symbolTable->username);
        symbolTable->username = interpolatedString;
    }

    // Interpolar en password
    if (symbolTable->password != NULL) {
        char *interpolatedString = substituteVariables(symbolTable->password);
        free(symbolTable->password);
        symbolTable->password = interpolatedString;
    }
}


// --------------------------------------------------------------------------------------

void initVariableList() {
    variableList = (variable_list *)malloc(sizeof(variable_list));
    if (variableList == NULL) {
        printf("Error: Could not allocate memory for the variable list.\n");
        exit(1);
    }
    variableList->head = NULL;
}

void freeVariableList() {
    // Libera la lista de variables y sus nodos
    variable_node *current = variableList->head;
    while (current != NULL) {
        variable_node *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
    free(variableList);
}

void addToVariableList(char *name, char *value) {
    // Verifica que el nombre de la variable no esté repetido
    variable_node *current = variableList->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // El nombre está repetido, agrega un error a la lista y retorna
            addToErrorList("Duplicate variable name");
            return;
        }
        current = current->next;
    }

    // El nombre no está repetido, agrega la variable a la lista
    variable_node *newNode = (variable_node *)malloc(sizeof(variable_node));
    if (newNode == NULL) {
        printf("Error: Could not allocate memory for the variable node.\n");
        exit(1);
    }

    newNode->name = strdup(name);
    if (newNode->name == NULL) {
        printf("Error: Could not allocate memory for the variable name.\n");
        exit(1);
    }

    newNode->value = strdup(value);
    if (newNode->value == NULL) {
        printf("Error: Could not allocate memory for the variable value.\n");
        exit(1);
    }

    newNode->next = NULL;

    if (variableList->head == NULL) {
        // Si la lista está vacía, el nuevo nodo es la cabeza
        variableList->head = newNode;
    } else {
        // Recorre la lista hasta llegar al último nodo
        current = variableList->head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Inserta el nuevo nodo al final de la lista
        current->next = newNode;
    }
}


variable_list * getVariableList(){
    return variableList;
}

void printVariableList() {
    // Imprime la lista de variables
    printf("Variables:\n");
    variable_node *current = variableList->head;
    while (current != NULL) {
        printf("Variable Name: %s, Value: %s\n", current->name, current->value);
        current = current->next;
    }
}

// --------------------------------------------------------------------------------------

void initTagList() {
    tagList = (tag_list *)malloc(sizeof(tag_list));
    if (tagList == NULL) {
        printf("Error: Could not allocate memory for the tag list.\n");
        exit(1);
    }
    tagList->head = NULL;
    tagList->tail = NULL;
}

void freeTagList() {
    // Libera la lista de tags y sus nodos
    tag_node *current = tagList->head;
    while (current != NULL) {
        tag_node *next = current->next;
        free(current->name);
        free(current->id);
        free(current);
        current = next;
    }
    free(tagList);
}

void addToTagList(char *name, char *id) {
    // Agrega el tag a la lista
    tag_node *newNode = (tag_node *)malloc(sizeof(tag_node));
    if (newNode == NULL) {
        printf("Error: Could not allocate memory for the tag node.\n");
        exit(1);
    }

    newNode->name = strdup(name);
    if (newNode->name == NULL) {
        printf("Error: Could not allocate memory for the tag name.\n");
        exit(1);
    }

    newNode->id = strdup(id);
    if (newNode->id == NULL) {
        printf("Error: Could not allocate memory for the tag ID.\n");
        exit(1);
    }

    newNode->next = NULL;

    if (tagList->head == NULL) {
        tagList->head = newNode;
        tagList->tail = newNode;
    } else {
        tagList->tail->next = newNode;
        tagList->tail = newNode;
    }
}

tag_list * getTagList(){
    return tagList;
}

void printTagList() {
    // Imprime la lista de tags
    printf("Tags:\n");
    tag_node *current = tagList->head;
    while (current != NULL) {
        printf("Tag Name: %s, ID: %s\n", current->name, current->id);
        current = current->next;
    }
}

int hasTags(){
    return tagList != NULL;
}

char * getTagString(Tag * tag){
    switch(*tag){
        case TAG_HTML:
            return "html";
        case TAG_HEAD:
            return "head";
        case TAG_TITLE:
            return "title";
        case TAG_BODY:
            return "body";
        case TAG_H1:
            return "h1";
        case TAG_H2:
            return "h2";
        case TAG_H3:
            return "h3";
        case TAG_H4:
            return "h4";
        case TAG_H5:
            return "h5";
        case TAG_H6:
            return "h6";
        case TAG_P:
            return "p";
        case TAG_A:
            return "a";
        case TAG_IMG:
            return "img";
        case TAG_UL:
            return "ul";
        case TAG_OL:
            return "ol";
        case TAG_DL:
            return "dl";
        case TAG_LI:
            return "li";
        case TAG_DIV:
            return "div";
        default:
            return "undefined";
    }
}

