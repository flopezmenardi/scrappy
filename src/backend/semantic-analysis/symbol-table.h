#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-syntax-tree.h"

typedef struct node {
    char * string;
    struct node * next;
} node;

typedef struct list {
    struct node * head;
    struct node * tail;
} list;

typedef struct variable_node {
    char * name;
    char * value;
    struct variable_node * next;
} variable_node;

typedef struct variable_list {
    struct variable_node * head;
} variable_list;

typedef struct tag_node {
    char * name;
    char * id;
    struct tag_node * next;
} tag_node;

typedef struct tag_list {
    struct tag_node * head;
    struct tag_node * tail;
} tag_list;

typedef struct symbol_table {
    variable_list * variable_list;
    list * url_list;
    tag_list * tag_list;
    list * path_list;
    char * username;
    char * password;
    list * error_list;
} symbol_table;

//--------------------------------------------------------------------------------------

extern symbol_table * symbolTable;

extern list * errorList;

symbol_table * initSymbolTable();

void freeSymbolTable();

symbol_table * getSymbolTable();

void printSymbolTable();

//--------------------------------------------------------------------------------------

void initList(list ** param_list);

void freeList(list ** param_list);

void addToList(char * string, list ** param_list);

void addToUrlList(char *string);

void addToPathList(char *string);

void addToErrorList(char *string);

list * getUrlList();

list * getPathList();

void printList(list * param_list);

void printUrlList();

void printPathList();

void printErrorList();

int hasUrls();

int hasPaths();

int hasAuth();

int hasErrors();

void setAuth(char * username, char * password);

//--------------------------------------------------------------------------------------

void initVariableList();

void freeVariableList();

void addToVariableList(char *name, char *value);

variable_list * getVariableList();

void printVariableList();

int hasVariables();

//--------------------------------------------------------------------------------------

void initTagList();

void freeTagList();

void addToTagList(char *name, char *id);

tag_list * getTagList();

void printTagList();

int hasTags();

char * getTagString(Tag * tag);

#endif
