#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_NUMBER 100000000

void exch(int a[], int i , int j);
int* createArray(int n);
int* dumpArray(int* a, int n);
void output(int x[], int n);
void quicksort_2_way(int a[], int l, int r) ;
void quicksort_3_way(int a[], int l, int r) ;


int int_compare(void const* x, void const* y)
	{
		int m,n;
		m = *((int*)x);
		n = *((int*)y);
		if(m==n) return 0;
		return m > n ? 1: -1;
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

void exch_gen(void * buf, int size, int i, int j)
	{
		char buffer[size];
		memcpy(buffer,(char *) buf + i*size, size);
		memcpy((char *)buf + i*size, buf + j*size, size);
		memcpy((char *)buf + j*size, buffer, size);
	}


void quicksort_3_way( void* a, int size,int l, int r, int (*compare)(void*, void*)) {
    if (r <= l) return;
    int p = l, i = l + 1, q = r;
	void * v = (char*)(a + (l*size));
    while (i <= q) {
		void* a_i = (char*)(a + (i*size));
        if ((*compare)(a_i, v) < 0) exch_gen(a, size, p++, i++);
        else if ((*compare)(a_i, v)) exch_gen(a, size, i, q--);
        else i++;
    }
    quicksort_3_way(a, size, l, p - 1);
    quicksort_3_way(a, size, q + 1, r);
}


int main()
{
	time_t start, end;
	int *a1,*a2,*a3;
	srand(time NULL);
	a1=createArray(MAX_NUMBER);
	a2= dumpArray(a1, MAX_NUMBER);
	a3 = dumpArray(a1,MAX_NUMBER);
	printf("Number of integer:%d\n", MAX_NUMBER);
	start= clock();
    quicksort_2_way(a1, 0 , MAX_NUMBER -1);
	end = clock();
	//output(a1,MAX_NUMBER);
	printf("Run in %.8f second(s)\n", difftime(end, start)/CLOCKS_PER_SEC);

	start= clock();
    quicksort_3_way(a1,sizeof(int), 0 , MAX_NUMBER -1);
	end = clock();
	//output(a2,MAX_NUMBER);
	printf("Run in %.8f second(s)\n", difftime(end, start)/CLOCKS_PER_SEC);

	start= clock();
	qsort(a3,MAX_NUMBER, sizeof(int),int_compare);
	end = clock();
	//output(a1,MAX_NUMBER);
	printf("Run in %.8f second(s)\n", difftime(end, start)/CLOCKS_PER_SEC);
	free(a1);
	free(a2);
	free(a3);
}