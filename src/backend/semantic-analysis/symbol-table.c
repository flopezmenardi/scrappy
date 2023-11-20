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
    if(hasErrors()){
        printf("Errors:\n");
        printList(errorList);
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
    newNode->string = strdup(string); // Assumes you want to copy the string
    newNode->next = NULL;

    if ((*param_list)->tail == NULL) {
        // The list is empty
        (*param_list)->head = newNode;
        (*param_list)->tail = newNode;
    } else {
        // Append to the end of the list
        (*param_list)->tail->next = newNode;
        (*param_list)->tail = newNode;
    }
}

void addToUrlList(char *string){
    addToList(string, &urlList);
    printf("added url: %s\n", string);
}

void addToPathList(char *string){
    addToList(string, &pathList);
    printf("added path: %s\n", string);
}

void addToErrorList(char *string){
    addToList(string, &errorList);
    printf("added error: %s\n", string);
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
    printList(urlList);
}

void printPathList(){
    printList(pathList);
}

void printErrorList(){
    printList(errorList);
}

int hasUrls(){
    return urlList != NULL;
}

int hasPaths(){
    return pathList != NULL;
}

int hasAuth(){
    return username != NULL && password != NULL;
}

int hasErrors(){
    return errorList != NULL;
}

void setAuth(char *username, char *password) {
    // Allocate memory for the username and copy its content
    symbolTable->username = strdup(username);
    if (symbolTable->username == NULL) {
        printf("Error: Couldn't allocate memory for the username.\n");
        exit(1);
    }

    // Allocate memory for the password and copy its content
    symbolTable->password = strdup(password);
    if (symbolTable->password == NULL) {
        // Clean up previously allocated memory
        free(symbolTable->username);
        printf("Error: Couldn't allocate memory for the password.\n");
        exit(1);
    }

    printf("Added username: %s\n", symbolTable->username);
    printf("Added password: %s\n", symbolTable->password);
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
    // Free the variable list and its nodes
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
    // Check if the name is repeated
    variable_node *current = variableList->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // Name is repeated, add an error and return
            addToErrorList("Duplicate variable name");
            return;
        }
        current = current->next;
    }

    // Name is not repeated, add to the variable list
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
        // If the list is empty, set the new node as the head
        variableList->head = newNode;
    } else {
        // Traverse the list to find the last node
        current = variableList->head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Append the new node after the last node
        current->next = newNode;
    }
}


variable_list * getVariableList(){
    return variableList;
}

void printVariableList() {
    // Print the variable list
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
    // Free the tag list and its nodes
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
    // Add to the tag list
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
    // Print the tag list
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
        default:
            return "undefined";
    }
}

