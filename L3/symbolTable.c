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


SymbolTable* createSymbolTable(Entry (*makeNode)(void*, void*), int (*compare)(void*, void*), void (*output)(Entry)){
    SymbolTable* table = (SymbolTable*) malloc(sizeof(SymbolTable));
    table->entries = (Entry*) malloc(sizeof(Entry) * INITIAL_SIZE);
    // table = (SymbolTable*) realloc(table, sizeof(SymbolTable));
    table->total = 0;
    table->size = INITIAL_SIZE;
    table->makeNode = makeNode;
    table->compare = compare;
    table->output = output;
    return table;
}

void dropSymbolTable(SymbolTable* table) {
    free(table);
}

void addEntry(void* key, void* value, SymbolTable* table) {
    int pos = binary_search(table->entries, key, 0, table->total - 1, table->compare);
    // if key already existed
    if (found == 1) {
        printf("Found an existing field: ");
        table->output(table->entries[pos]);
        table->entries[pos] = table->makeNode(key, value);
        printf("Updated this field.\n");
    }
    // if key did not exist yet
    else {
        if (table->total == table->size) {
        table->entries = (Entry*) realloc(table->entries,sizeof(Entry) * (table->size + INCREMENTAL_SIZE));
        // table = (SymbolTable*) realloc(table, sizeof(SymbolTable));
        table->size += INCREMENTAL_SIZE;
        }
        // Shift everthing "greater" than new item to the right 1 index 
        // pos: index where the item should be placed.
        int i;
        for (i = table->total; i > pos; i--){
            table->entries[i] = table->entries[i - 1];
        }
        table->entries[pos] = table->makeNode(key, value);
        table->total++;
    }
}

int binary_search(Entry* entries, void* item, int l, int r, int (*compare)(void*, void*)) {
    int mid, res;
    if (r < l) {
        found = 0;
        return (r+1); //position where item will be inserted properly
    }
    mid = (l + r) / 2;
    res = compare(item, entries[mid].key);
    if (res==0){
            found = 1;
            return mid;
    } else if (res<0)
        return binary_search(entries, item, l, mid-1, compare);
    else
        return binary_search(entries, item, mid+1, r, compare);
}


Entry* getEntry(void* key, SymbolTable* table) {
    int pos = binary_search(table->entries, key, 0, table->total - 1, table->compare);
    if (found == 1)
        return &table->entries[pos];
    else 
        return NULL;
}

void outputTable(SymbolTable* table) {
    printf("\n--------Table-------\n");
    int i;
    for(i = 0; i < table->total; i++) {
        printf("%d- ", i+1);
        table->output(table->entries[i]);
    }
    printf("--------------------\n\n");
}

Entry makePhone(void* name, void* phone){
    Entry res;
    res.key = strdup((char*) name);
    res.value = strdup((char*) phone);
    return res;
}

int comparePhone(void* key1, void* key2) {
    return strcmp((char*) key1, (char*) key2);
}

void outputPhone(Entry entry){
    printf("Name: %s. Phone number: %s.\n", entry.key, entry.value);
}

int main() {
    SymbolTable* book = createSymbolTable(makePhone, comparePhone, outputPhone);
    addEntry("Tung", "0978654123", book);
    addEntry("Anh", "0348561234", book);
    addEntry("Tuan", "0865123456", book);
    addEntry("Hung", "0368895141", book);
    outputTable(book);
    addEntry("Tuan", "0972145168", book); //test case: key collision.
    outputTable(book);
    addEntry("Phuong", "0938654223", book);
    outputTable(book);

    // query from table
    Entry* x = getEntry("Hung", book);
    if (x == NULL) {
        printf("Not found.");
    } else {
        outputPhone(*x);
    } 

}


