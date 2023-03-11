#include "libwc.h"

#define MAX_COMMAND_LENGTH 2048
char command[MAX_COMMAND_LENGTH] = "";


BlocksTable BlocksTable_create(size_t size) {
    if (size == 0) {
        puts("Cannot create a blocks table with size = 0!");
    }

    return (BlocksTable) {
        .blocks = calloc(size, sizeof(char*)),
        .maxSize = size,
        .currentSize = 0,
        .removed = calloc(size, sizeof(bool)),
    };
}


bool canAddBlock(BlocksTable *table) {
    return table->currentSize < table->maxSize;
}


size_t getFileSize(FILE* f) {
    fseek(f, 0, SEEK_END);
    size_t fileSize = ftell(f);
    rewind(f);
    return fileSize;
}


char* readFile(char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        puts("Failed to open the file!");
        return "";
    }

    size_t fileSize = getFileSize(f);
    char* fileContent = calloc(fileSize, sizeof(char));
    fread(fileContent, sizeof(char), fileSize, f);
    fclose(f);

    return fileContent;
}


void BlocksTable_countWords(BlocksTable* table, char* filename) {
    if(!canAddBlock(table)) {
        puts("New block cannot be added!");
        return;
    }

    char tempFilename[] = "/tmp/tmp_word_count_XXXXXX";
    int tempFile = mkstemp(tempFilename);

    if (tempFile == -1) {
        puts("Failed to create a temporary file!");
        return;
    }

    snprintf(command, MAX_COMMAND_LENGTH, "wc '%s' > '%s'", filename, tempFilename);
    if (system(command) == -1 ) {
        puts("Failed to run wc command!");
        return;
    }

    char *fileContent = readFile(tempFilename);
    if (strlen(fileContent) == 0) {
        puts("Failed to read the file!");
        return;
    }

    snprintf(command, MAX_COMMAND_LENGTH, "rm -f '%s'", tempFilename);
    if(system(command) == -1) {
        puts("Failed to delete the file!");
    }

    table->blocks[table->currentSize] = fileContent;
    table->currentSize += 1;
}


bool isIndexValid(BlocksTable *table, size_t index) {
    return index < table->currentSize && !table->removed[index];
}


char* BlocksTable_get(BlocksTable* table, size_t index) {
    if (!isIndexValid(table, index)) {
        puts("The given index is greater than the current table size or the block has been already removed!");
        return "";
    }

    return table->blocks[index];
}

void BlocksTable_remove(BlocksTable* table, size_t index) {
    if (!isIndexValid(table, index)) {
        puts("The given index is greater than the current table size or the block has been already removed!");
        return;
    }

    free(table->blocks[index]);
    table->removed[index] = true;
}

void BlocksTable_free(BlocksTable* table) {
    for (int i=0; i<table->currentSize; i++) {
        if (!table->removed[i]){
            free(table->blocks[i]);
        }
        else {
            table->removed[i] = false;
        }

    }

    table->currentSize = 0;
}
