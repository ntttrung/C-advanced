#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 20

void quicksort_2_way(int a[], int l, int r);
void exch(int a[], int i, int j);
void quicksort_3_way(int a[], int l, int r);
int *createArray(int size);
int *dumpArray(int *a, int size);
void output(int *a, int size);


void quicksort_2_way(int a[], int l, int r) {
    if (r <= l) return;
    int i = l, j = r + 1;
    int v = a[l];
    while (1) {
        while (a[++i] < v) if (i == r) break;
        while (v < a[--j]) if (j == l) break;
        if (i >= j) break;
        exch(a, i, j);
    }
    exch(a, l, j);
    quicksort_2_way(a, l, j - 1);
    quicksort_2_way(a, j + 1, r);
}

void exch(int a[], int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void quicksort_3_way(int a[], int l, int r) {
    if (r <= l) return;
    int p = l, i = l + 1, q = r;
    int v = a[l];
    while (i <= q) {
        if (a[i] < v) exch(a, p++, i++);
        else if (a[i] > v) exch(a, i, q--);
        else i++;
    }
    quicksort_3_way(a, l, p - 1);
    quicksort_3_way(a, q + 1, r);
}

int *createArray(int size) {
    int i;
    int *a= malloc(sizeof(size));  
    for (i= 0; i < size; i++) {
        *(a + i) = rand() % 10;
    }
    return a;
}

int *dumpArray(int *a, int size) {
    int *b= malloc(sizeof(size));
    while (--size){
        *(b++) = *(a++);
    }
    return b;
}

void output(int *a, int size) {
    int i;
    for (i=0; i < size; i++){
        printf("%d", *(a+i));
    }
    printf("\n");
}

int main()
    {
        int *a = createArray(20);
        quicksort_2_way(a,0,19);
        output(a,20);
    }