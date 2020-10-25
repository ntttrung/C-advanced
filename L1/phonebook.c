#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define INITIAL_SIZE 10
#define INCREMENTAL   

typedef struct 
    {
        char name[80];
        long number;
    }
PhoneEntry;

typedef struct 
    {
        PhoneEntry * entries;
        int total;
        int size;
    } PhoneBook;

PhoneBook *createPhoneBook()
    {
        PhoneBook *p = (PhoneBook*)malloc(sizeof(PhoneBook));
        p->size = INITIAL_SIZE;
        p->total = 0;
        return p;
    }

void dropPhonebook(PhoneBook *book)
    {
        free(book);
    }

void addPhoneNumber(char* name, long number, PhoneBook *book)
    {
        if (book->size == book->total)
            book = realloc(book,book->size + INCREMENTAL * (sizeof(long) + sizeof(char) * 80));
        p->entries = (PhoneEntry*)malloc((sizeof(PhoneEntry)));
        book->total ++;
        strcpy(book->entries[book->total].name,name);
        book->entries[book->total].number = number;
    }

// PhoneEntry getPhoneNumber(char* name, PhoneBook * book)
//     {
//         int i;
//         for(i = 1; i < book->total; i++)
//             if(strcasecmp(name,book->entries[i].name) == 0)
//                 return book->entries[i];
//         return NULL;
//     }


void printPhonebook(PhoneBook *p)
    {
        int i;
        for(i=1; i<=p->total;i++)
            puts(p->entries[i].name);
            printf("TRue\n")
            printf("%ld\n",p->entries[i].number);
    }



int main()
    {
        PhoneBook *book = createPhoneBook();
        addPhoneNumber("Trung", 12345, book);
        addPhoneNumber("Thang",88888,book);
        printPhonebook(book);
    }

