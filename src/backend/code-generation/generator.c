#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

void generateUrlsArray(list * url_list, FILE * filePointer);
void generateTagsArray(tag_list * tag_list, FILE * filePointer);
void generatePathsArray(list * path_list, FILE * filePointer);

void Generator(int result, symbol_table *symbolTable) {
    LogInfo("Generating js file. Compilation ended with code: '%d'.", result);

    FILE *filePointer;

    filePointer = fopen("scrappy.js", "w");

    if (filePointer == NULL) {
        printf("Unable to create js file.\n");
        return;
    }

    // Generate arrays
    generateUrlsArray(symbolTable->url_list, filePointer);
    generateTagsArray(symbolTable->tag_list, filePointer);
    generatePathsArray(symbolTable->path_list, filePointer);

    fclose(filePointer);
}

void generateUrlsArray(list *url_list, FILE *filePointer) {
    fprintf(filePointer, "const urls = [\n");

    node *current = url_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  '%s',\n", current->string);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

void generateTagsArray(tag_list *tag_list, FILE *filePointer) {
    fprintf(filePointer, "const tagIdPairs = [\n");

    tag_node *current = tag_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  { tag: '%s', id: '%s' },\n", current->name, current->id);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

void generatePathsArray(list *path_list, FILE *filePointer) {
    fprintf(filePointer, "const paths = [\n");

    node *current = path_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  '%s',\n", current->string);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

