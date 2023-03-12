#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
    char **blocks;
    size_t maxSize;
    size_t currentSize;
    bool *removed;
} BlocksTable;

#ifdef USE_DLL

BlocksTable (*BlocksTable_create) (size_t size);
void (*BlocksTable_countWords) (BlocksTable* table, char* filename);
char* (*BlocksTable_get) (BlocksTable* table, size_t index);
void (*BlocksTable_remove) (BlocksTable* table, size_t index);
void (*BlocksTable_free) (BlocksTable *table);

#else

BlocksTable BlocksTable_create(size_t size);
void BlocksTable_countWords(BlocksTable* table, char* filename);
char* BlocksTable_get(BlocksTable* table, size_t index);
void BlocksTable_remove(BlocksTable* table, size_t index);
void BlocksTable_free(BlocksTable *table);

#endif