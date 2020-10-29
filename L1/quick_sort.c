#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define MAX_NUMBER 25

int* createArray(int n);
int* dumpArray(int* a, int n);
void output(int x[], int n);
void exch(int a[], int i, int j);
void swap(int *a, int *b);
void quicksort_2_way(int a[], int l, int r);
void quicksort_3_way(int a[], int l, int r);

int main()
    {
        int *a, *b;
        int i;
        a = createArray(MAX_NUMBER);
        output(a,MAX_NUMBER);
        quicksort_3_way(a, 0, MAX_NUMBER - 1);
        output(a,MAX_NUMBER);
    }

int int_compare(void const *x, void const *y)
    {
        int m,n;
        m = *((int *) x);
        n = *((int *) y);
        if (m == n) return 0;
        return m > n ? 1:-1;
    }

int* createArray(int n )
    {
        int i;
        int *p = (int*)malloc(sizeof(int) * n);
        for(i = 0; i < n; i++)
            {
                p[i] = rand() % 10;
            }
        return p;
    }

int* dumpArray(int *a, int n)
    {
        int *b = (int *)malloc(sizeof(int) * n);
        memcpy(b, a, n * sizeof(int));
        return b;
    }

void output(int * x, int n)
    {
        int i;
        for(i = 0; i<n; i++)
            printf("%d  ",*(x+i));
        printf("\n");
    }

void exch(int *a, int i, int j)
    {
        int temp = *(a+i);
        *(a+i) = *(a+j);
        *(a+j) = temp;
    }

void swap(int* a, int* b)
    {
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;
    }

void quicksort_2_way(int a[], int l, int r)
    {
        if (l > r) return;
        int i = l, j = r + 1;
        int v = a[l];

        while(1)
            {
                while(a[++i] < v) if ( i >= r) break;
                while(a[--j] > v) if ( j <= l) break;
                if (i >= j) break;
                exch(a, i, j);
            }
        exch(a, l, j);
        quicksort_2_way(a, l, j-1);
        quicksort_2_way(a, j+1, r);
    }

void quicksort_3_way(int x[],int left,int right){
    // Before partitioning
    int i,j,p,q,pivot,k;
    i=left-1; j=right;
    p=left-1; q=right;
    pivot=x[right];
    if (right<=left) return;
    
    // During swapping
    for (;;){
        while (x[++i]<pivot);
        while (x[--j]>pivot) if (j==left) break;
        if (i>=j) break;
        swap(&x[i],&x[j]);
        if (x[i]==pivot){
            p++;
            swap(&x[p],&x[i]);
        }
        if (x[j]==pivot){
            q--;
            swap(&x[q],&x[j]);
        }
    }
    swap(&x[i],&x[right]);
    j=i-1; i++;
    
    // Moving equal elements to the pivot
    for (k=left;k<p;k++,j--) swap(&x[k],&x[j]);
    for (k=right-1;k>q;k--,i++) swap(&x[i],&x[k]);
    
    // Calling QS 3 way for smaller partitions
    quicksort_3_way(x,left,j);
    quicksort_3_way(x,i,right);
}