#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INITIAL_SIZE 3
#define INCREMENTAL_SIZE 5

int found = 0;

typedef struct {
    char number[20];
    char name[80];
} PhoneEntry;

typedef struct {
    PhoneEntry* entries;
    int total;
    int size;
} PhoneBook;

PhoneBook* createPhoneBook() {
    PhoneBook* book = (PhoneBook*) malloc(sizeof(PhoneBook));
    book->entries = (PhoneEntry*) malloc(sizeof(PhoneEntry) * INITIAL_SIZE);
    // book = (PhoneBook*) realloc(book, sizeof(PhoneBook));
    book->total = 0;
    book->size = INITIAL_SIZE;
    return book;
}

void dropPhoneBook(PhoneBook* book) {
    free(book);
}

int binary_search(PhoneEntry* entries, char* item, int l, int r) {
    int mid, res;
    if (r < l) {
        found = 0;
        return (r+1); //position where item will be inserted properly
    }
    mid = (l + r) / 2;
    res = strcmp(item, entries[mid].name);
    if (res==0)
        {
            found = 1;
            return mid;
        }
        
    else if (res<0)
        return binary_search(entries, item, l, mid-1);
    else
        return binary_search(entries, item, mid+1, r);
}

void addPhoneNumber(char* name, char* number, PhoneBook* book) {
   
    int pos = binary_search(book->entries, name, 0, book->total - 1);
    // if name already existed
    if (found == 1) {
        printf("Phone number of \"%s\" is already exist.\n", name);
        strcpy(book->entries[pos].number, number);
        printf("Phone number of \"%s\" is updated.\n", name);
    }
    // if name did not exist yet
    else {
        if (book->total == book->size) {
        book->entries = (PhoneEntry*) realloc(book->entries,sizeof(PhoneEntry) * (book->size + INCREMENTAL_SIZE));
        // book = (PhoneBook*) realloc(book, sizeof(PhoneBook)); 
        book->size += INCREMENTAL_SIZE;
        }
        // Shift everthing "greater" than new item to the right 1 index 
        // pos: index where the item should be placed.
        for (int i = book->total; i > pos; i--){
            book->entries[i] = book->entries[i - 1];
        }
        strcpy(book->entries[pos].name,name);
        strcpy(book->entries[pos].number, number);
        book->total++;
    }
}

PhoneEntry* getPhoneNumber(char* name, PhoneBook* book) {
    int pos = binary_search(book->entries, name, 0, book->total - 1);
    // if name already existed
    if (found == 1) {
        // printf("Phone number of \"%s\" is: %d.", name,  book->entries[pos].number);
        return &book->entries[pos];
    }
    // if name did not exist yet
    else {
        // printf("Name \"%s\" is not found.", name);
        return NULL;
    }
}

void outputBook(PhoneBook* book) {
    printf("------PhoneBook-----\n");
    for(int i = 0; i < book->total; i++) {
        printf("%d- Name: %s. Phone number: %s.\n", i + 1, book->entries[i].name, book->entries[i].number);
    }
    printf("--------------------\n\n");
}

int main() {

    PhoneBook* book = createPhoneBook();
    addPhoneNumber("Tung", "0978654123", book);
    addPhoneNumber("Anh", "0348561234", book);
    addPhoneNumber("Tuan", "0865123456", book);
    addPhoneNumber("Hung", "0368895141", book);
    addPhoneNumber("Tuan", "0972145168", book);
    addPhoneNumber("Phuong", "0938654223", book);
    outputBook(book);


    PhoneEntry* x = getPhoneNumber("Hung", book);
    if (x == NULL) {
        printf("Not found.");
    } else {
        printf("Phone number of \"%s\" is : %s", x->name, x->number);
    }
}


