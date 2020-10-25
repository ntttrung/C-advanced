//Pham Thanh Dat
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jval.h"

void outputJal(Jval buf[],size_t size, int n){
    int i;
    for (i=0; i<n; i++)
        if (size == sizeof(int)){
            printf("%d ",*((int*)buf + i));
        }
        else if (size == sizeof(long)){
            printf("%ld ",buf[i]);
        }
    printf("\n");
}

int search(
           Jval *buf,
           int size_of_item,
           int l, int r,
           Jval *item,
           int (*compare)(Jval*,Jval*)
           ){
    int mid,res;
    if (r<l) return -1;
    mid=(l+r)/2;
    res=compare(item,&buf[mid]);
    if (res==0)
        return mid;
    else if (res<0)
        return search(buf, size_of_item, l, mid-1, item, compare);
    else
        return search(buf, size_of_item, mid+1, r, item, compare);
}

int int_compare(Jval *x,Jval *y){
    Jval *m,*n;
    m = x;
    n = y;
    if (m->i==n->i) return 0;
    return m->i>n->i ? 1:-1;
}

int compareJval(Jval *a, Jval *b)
    {
        int m = jval_i(*a);
        int n = jval_i(*b);
        if(m == n) return 0;
        if(m < n) return -1;
        return 1;
    }




void swap(Jval buf[],int i,int j){
    Jval tmp = buf[i];
    buf[i] = buf[j];
    buf[j] = tmp;
}

void qsort_3way(Jval buf[], int l, int r, size_t size, int (*compare)(Jval*,Jval*), void (*swap)(Jval*, int, int)){
    if(l>=r) return;
    int i=l-1,j=r;
    int p=l-1,q=r;
    
    while(1)
    {
        while(compare(&buf[++i],&buf[r])==-1);
        while(compare(&buf[--j],&buf[r])==1)
        {
            if(j==l) break;
        }
        if(i>=j) break;
        swap(buf,i,j);
        if(compare(&buf[i],&buf[r])==0) swap(buf,++p,i);
        if(compare(&buf[j],&buf[r])==0) swap(buf,--q,j);
    }
    
    swap(buf,i,r);
    j=i-1;
    i=i+1;
    
    int k;
    for(k=l;k<=p;k++)
    {
        swap(buf,k,j--);
    }
    for(k=r-1;k>=q;k--)
    {
        swap(buf,k,i++);
    }
    
    qsort_3way(buf,l,j,size,compare,swap);
    qsort_3way(buf,i,r,size,compare,swap);
}

int main(){
    Jval a[10]={102, 5, 100, 9, 10, 34, 56, 89, 8, 2};
    outputJal(a, sizeof(long), 10);
    qsort_3way(a, 0, 9, sizeof(long), int_compare, swap);
    outputJal(a, sizeof(long), 10);
    
    long res;
    long find = 89;
    printf("Finding element: %ld \n", find);
    res = search(a, sizeof(long), 0, 9, &find, int_compare);
    printf("Position found: %ld \n", res);
}
