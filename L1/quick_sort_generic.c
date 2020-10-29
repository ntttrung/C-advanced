#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void output(void *buf,size_t size, int n){
    int i;
    for (i=0; i<n; i++)
        if (size == sizeof(int)){
            printf("%d ",*((int*)buf + i));
        }
        else if (size == sizeof(long)){
            printf("%ld ",*((long*)buf + i));
        }
    printf("\n");
}

int search(
           void *buf,
           int size_of_item,
           int l, int r,
           void *item,
           int (*compare)(void const*,void const*)
           ){
    int mid,res;
    if (r<l) return -1;
    mid=(l+r)/2;
    res=compare(item,(char *)buf+size_of_item*mid);
    if (res==0)
        return mid;
    else if (res<0)
        return search(buf, size_of_item, l, mid-1, item, compare);
    else
        return search(buf, size_of_item, mid+1, r, item, compare);
}

int int_compare(void const *x,void const *y){
    int m,n;
    m=*((int *)x);
    n=*((int *)y);
    if (m==n) return 0;
    return m>n ? 1:-1;
}

void int_swap(void *buf,size_t size,int i,int j){
    int cmp=*((char*)buf+size*i);
    *((char*)buf+size*i)=*((char*)buf+size*j);
    *((char*)buf+size*j)=cmp;
}

void qsort_3way(void* buf, int l, int r, size_t size, int (*compare)(void const*,void const*), void (*swap)(void*, size_t, int, int)){
    if(l>=r) return;
    int i=l-1,j=r;
    int p=l-1,q=r;
    
    while(1)
    {
        while(compare((char*)buf+(++i)*size,(char*)buf+size*r)==-1);
        while(compare((char*)buf+(--j)*size,(char*)buf+size*r)==1)
        {
            if(j==l) break;
        }
        if(i>=j) break;
        swap(buf,size,i,j);
        if(compare((char*)buf+i*size,(char*)buf+r*size)==0) swap(buf,size,++p,i);
        if(compare((char*)buf+j*size,(char*)buf+r*size)==0) swap(buf,size,--q,j);
    }
    
    swap(buf,size,i,r);
    j=i-1;
    i=i+1;
    
    int k;
    for(k=l;k<=p;k++)
    {
        swap(buf,size,k,j--);
    }
    for(k=r-1;k>=q;k--)
    {
        swap(buf,size,k,i++);
    }
    
    qsort_3way(buf,l,j,size,compare,swap);
    qsort_3way(buf,i,r,size,compare,swap);
}

int main(){
    long a[10]={102, 5, 100, 9, 10, 34, 56, 89, 8, 2};
    output(a, sizeof(long), 10);
    qsort_3way(a, 0, 9, sizeof(long), int_compare, int_swap);
    output(a, sizeof(long), 10);
    
    long res;
    long find = 89;
    printf("Finding element: %ld \n", find);
    res = search(a, sizeof(long), 0, 9, &find, int_compare);
    printf("Position found: %ld \n", res);
}
