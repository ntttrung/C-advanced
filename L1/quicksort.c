#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_NUMBER 1000

void exch(int a[], int i , int j);
int* createArray(int n);
int* dumpArray(int* a, int n);
void output(int x[], int n);
void quicksort_2_way(int a[], int l, int r) ;
void quicksort_3_way(int a[], int l, int r) ;

int main()
{
	time_t start, end;
	int *a1,*a2;
	srand(time NULL);
	a1=createArray(MAX_NUMBER);
	a2= dumpArray(a1, MAX_NUMBER);
	
	printf("Number of integer:%d\n", MAX_NUMBER);
	start= clock();
    quicksort_2_way(a1, 0 , MAX_NUMBER -1);
	end = clock();
	//output(a1,MAX_NUMBER);
	printf("Run in %.8f second(s)\n", difftime(end, start)/CLOCKS_PER_SEC);

	start= clock();
    quicksort_3_way(a1, 0 , MAX_NUMBER -1);
	end = clock();
	//output(a2,MAX_NUMBER);
	printf("Run in %.8f second(s)", difftime(end, start)/CLOCKS_PER_SEC);

	



}

int* createArray(int n)
{
	
	int *p= (int*)malloc(sizeof(int)*n);
	int i;
	for(i=0; i<n ; i++)
	
		*(p+i) = rand() % 10;
	
	return p;
}

int * dumpArray(int *a, int size) 
{
	int i;
   int *b= (int*)malloc(sizeof(int)*size);
    for(i=0; i<size; i++)
    {
        *(b+i)=*(a+i);
    }
    return b;
    
}
void output(int x[], int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		printf("%d ",x[i]);	
	}
	printf("\n");
}

void quicksort_2_way(int a[], int l, int r) 
{
	if(r<=l) return;
	int i=l, j=r+1;
	int v=a[l];
	while(1)
	{
		while(a[++i]<v) if(i==r) break;
		while(a[--j]>v) if(j==l) break;
		if(i>=j) break;
		exch(a,i,j);
	}
	exch(a,l,j);
	quicksort_2_way(a,l,j-1);
	quicksort_2_way(a,j+1,r);
}

void exch(int a[], int i , int j)
{
	int temp=a[i];
	a[i]= a[j];
	a[j]= temp;
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