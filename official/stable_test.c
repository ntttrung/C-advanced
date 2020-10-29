#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INITIAL_SIZE 3
#define INCREMENTAL_SIZE 5

int found = 0;

typedef struct {
    void* key;
    void* value;
} Entry;

typedef struct {
    Entry* entries;
    int size, total;
    Entry (*makeNode)(void*, void*);
    int (*compare)(void*, void*);
    void (*output)(Entry);
} SymbolTable;

SymbolTable* createSymbolTable( Entry (*makeNode)(void*, void*),
                               int (*compare)(void*, void*),
                               void (*output)(Entry))
    {
        SymbolTable *table = (SymbolTable*)malloc(sizeof(SymbolTable));
        table->entries = (Entry*)malloc(sizeof(Entry) * INITIAL_SIZE);
        table->size = INITIAL_SIZE;
        table->total = 0;
        table->makeNode = makeNode;
        table->compare = compare;
        table->output = output;
        return table;
    }

void dropSymbolTable(SymbolTable* tab)
    {
        free(tab);
    }

int binary_search(Entry* entries, void* item, int l, int r, int (*compare)(void*, void*))
    {
        int mid, res;
        if(r < l)
            {
                found = 0;
                return r+1;
            }
        mid = (l+r) / 2;
        res = compare(item, entries[mid].key);
        if (res == 0)
            {
                found = 1;
                return mid;
            }
        else if(res < 0)
            return binary_search(entries, item, l, mid-1, compare);
        else
            return binary_search(entries, item, mid+1, r, compare);
    }

int comparekey(void* key1, void* key2)
    {
        return strcasecmp((char*)key1, (char*)key2);
    }

void addEntry(void* key, void* value, SymbolTable* table)
    {
        int pos = binary_search(table->entries, key, 0, table->total, comparekey);
        if(found == 1)
            {
                printf("%s has existed, update...\n", (char*)key);
                
            }
    }