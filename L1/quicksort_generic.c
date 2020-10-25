#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 20

int int_compare(void const* x, void const *y);
void quicksort_2_way(int a[], int l, int r);
void exch(int a[], int i, int j);
void quicksort_3_way(int a[], int l, int r);
int *createArray(int size);
int *dumpArray(int *a, int size);
void output(int *a, int size);
void exch_generic(void* buf, int i, int j, int size);
void quicksort_2_way_generic(void* buf, size_t size, int l, int r, int (*compare) (void const*, void const*));
void quicksort_3_way_generic(void* buf, size_t size, int l, int r, int (*compare) (void const*, void const*));

int main() {

    int *a1, *a2, *a3, *a4, *a5;
    srand(time NULL);
    a1 = createArray(MAX_SIZE);
    a2 = dumpArray(a1, MAX_SIZE);
    a3 = dumpArray(a1, MAX_SIZE);
    a4 = dumpArray(a1, MAX_SIZE);
    a5 = dumpArray(a1, MAX_SIZE);
    output(a1, MAX_SIZE);
    printf("Number of elements: %d\n\n", MAX_SIZE);

    // /* Using qsort() function in C library */
    // clock_t start0, end0;
    // start0 = clock();
    // qsort(a3, MAX_SIZE, sizeof(int), int_compare);
    // // output(a3, MAX_SIZE);
    // end0 = clock();
    // printf("Quick sort using library: %.8f seconds.\n", (double) (end0 - start0) / CLOCKS_PER_SEC);

    // /* Using quicksort 2 way */
    // clock_t start1, end1;
    // start1 = clock();
    // quicksort_3_way(a1, 0, MAX_SIZE-1);
    // // output(a1, MAX_SIZE);
    // end1 = clock();
    // printf("Quick sort 2 way: %.8f seconds.\n", (double) (end1 - start1) / CLOCKS_PER_SEC);

    /* Using quicksort 3 way */
    clock_t start2, end2;
    start2 = clock();
    // quicksort_3_way_generic(a2, sizeof(int), 0, MAX_SIZE - 1, int_compare);
    quicksort_3_way(a2, 0, MAX_SIZE-1);
    output(a2, MAX_SIZE);
    end2 = clock();
    printf("Quick sort 3 way: %.8f seconds.\n\n", (double) (end2 - start2) / CLOCKS_PER_SEC);

    /* Using quicksort 2 way generic */
    clock_t start3, end3;
    start3 = clock();
    quicksort_2_way_generic(a4, sizeof(int), 0, MAX_SIZE - 1, int_compare);
    output(a4, MAX_SIZE);
    end3 = clock();
    printf("Quick sort 2 way generic: %.8f seconds.\n\n", (double) (end3 - start3) / CLOCKS_PER_SEC);

    // Using quicksort 3 way generic 
    clock_t start4, end4;
    start4 = clock();
    quicksort_3_way_generic(a5, sizeof(int), 0, MAX_SIZE - 1, int_compare);
    output(a5, MAX_SIZE);
    end4 = clock();
    printf("Quick sort 3 way generic: %.8f seconds.\n", (double) (end4 - start4) / CLOCKS_PER_SEC);

    free(a1);
    free(a2);
    free(a3);
    free(a4);
    free(a5);

}

int int_compare(void const* x, void const *y) {
    int m, n;
    m = *((int*)x);
    n = *((int*)y);
    if (m == n) return 0;
    return m > n ? 1: -1;
}


void exch(int a[], int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void exch_generic(void* buf, int i, int j, int size) {
    void *tmp = malloc(sizeof(char)*size);
    memcpy(tmp, buf+size*i, size);
    memcpy(buf+size*i, buf+size*j, size);
    memcpy(buf+size*j, tmp, size);

    // char buffer[size];
    // memcpy(buffer, (char*)(buf + i*size), size);
    // memcpy((char*)(buf + i*size), (char*)(buf + j*size), size);
    // memcpy((char*)(buf + j*size), buffer, size);
}

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

void quicksort_2_way_generic(void* buf, size_t size, int l, int r, int (*compare) (void const*, void const*)) {
    if (r <= l) return;
    int i = l, j = r + 1;
    void* v = (char*)(buf + (size*l));
    while (1) {
        while (compare(v, (char*)(buf + (size*(++i)))) > 0) if (i == r) break;
        while (compare(v, (char*)(buf + (size*(--j)))) < 0) if (j == l) break;
        if (i >= j) break;
        exch_generic(buf, i, j, size);
    }
    exch_generic(buf, l, j, size);
    quicksort_2_way_generic(buf, size, l, j - 1, compare);
    quicksort_2_way_generic(buf, size, j + 1, r, compare);
}
void quicksort_3_way(int a[], int l, int r) {
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	while(1)	{
		while (a[++i] < a[r]);
		while (a[r] < a[--j]) if (j == l) break;
		if (i >= j) break;
		exch(a, i, j);
		if (a[i]==a[r]) exch(a, ++p, i);
		if (a[j]==a[r]) exch(a, --q, j);
	}
	exch(a, i, r);
	j = i - 1;
	i = i + 1;
	for (int k = l ; k <= p; k++) exch(a, k, j--);
	for (int k = r-1; k >= q; k--) exch(a, k, i++);
	quicksort_3_way(a, l, j);
	quicksort_3_way(a, i, r);
}

void quicksort_3_way_generic(void* buf, size_t size, int l, int r, int (*compare) (void const*, void const*)) {
    if (r<=l) return;
    int i=l-1, j=r;
    int p=l-1, q=r;
  while(1)
  {
      while(compare((char *)(buf+size*(++i)),(char *)(buf+size*r)) <0);
      while(compare((char*)(buf+ size*r),(char*)(buf+size*(--j)))<0) if(j==l) break;
      if(i>=j) break;
      exch_generic(buf, i,j, size);
      if(compare((char*)(buf+size*i),(char*)(buf+size*r))==0)  exch_generic(buf,++p,i,size);
      if(compare((char*)(buf+size*j),(char*)(buf+size*r))==0)  exch_generic(buf, --q,j, size);
  }
  exch_generic(buf,i,r,size);
  j=i-1;
  i=i+1;
  for(int k=l;k<=p; k++) exch_generic(buf, k, j--,size);
  for(int k=r-1; k>=q; k--) exch_generic(buf,k,i++,size);
  quicksort_3_way_generic(buf,size,l,j,compare);
  quicksort_3_way_generic(buf,size,i,r,compare);
}
/*
void quicksort_3_way_generic(void* buf, size_t size, int l, int r, int (*compare) (void const*, void const*)) {
    if (r <= l) return;
    
    void* v = (char*)(buf + (size*l));
    int p = l, i = l + 1, q = r;
    
    while (i <= q) {
        if ((*compare) (v, (char*)(buf + (size*i))) > 0) exch_generic(buf, p++, i, size);
        else if ((*compare) (v, (char*)(buf + (size*i))) < 0) exch_generic(buf, i, q--, size);
        else i++;
    }
    quicksort_3_way_generic(buf, size, l, p - 1, compare);
    quicksort_3_way_generic(buf, size, q + 1, r, compare);
}
*/
/*
void quicksort_3_way(int a[], int l, int r) {
    if (r <= l) return;
    int p = l, i = l + 1, q = r;
    int v = a[l];
    while (i <= q) {
        if (a[i] < v) exch(a, p++, i);
        else if (a[i] > v) exch(a, i, q--);
        else i++;
    }
    quicksort_3_way(a, l, p - 1);
    quicksort_3_way(a, q + 1, r);
}
*/
int *createArray(int size) {
    int *a= malloc(size*sizeof(int));  
    for (int i= 0; i < size; i++) {
        *(a + i) = rand() % 10;
    }
    return a;
}

int *dumpArray(int *a, int size) {
    int *b= malloc(size*sizeof(int));
   for (int i=0; i < size; i++){
        *(b+i) = *(a+i);
    }
    return b;
}

void output(int *a, int size) {
    for (int i=0; i < size; i++){
        printf("%d ", *(a+i));
    }
    printf("\n");
}