#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
    {
        char id[10];
        float profit;
    }
sale_struct;

sale_struct data[10005];
int count = -1;

void xoa(char s[90],int vitrixoa)
{
    int i;
	int n=strlen(s);
    for(i=vitrixoa;i<n;i++)
     s[i]=s[i+1];
	s[n-1]='\0';
}
void xoakt(char*s)
{
    int i;
    for(i=0;i<strlen(s);i++)
		if(s[i]=='\n'&& s[i+1]=='\n')
		{
			xoa(s,i);
			i--;
		}
    if(s[0]=='\n')
		xoa(s,0);
    if(s[strlen(s)-1]=='\n')
		xoa(s,strlen(s)-1);
}

void read_csv()
    {
        int features = 0;
        char *tmp;
        FILE *file = fopen("sale_records.csv", "r");
        if (file == NULL)
            {
                perror("Unable to open the file.");
                exit(1);
            }
        char line[200];

        while(fgets(line, sizeof(line), file) !=NULL)
            {
                char * token;
                token = strtok(line, ",");
                count ++;
                if(count != 0)
                    {
                        while(token != NULL && token != "\n")
                            {
                                features ++;
                                tmp = token;
                                xoakt(tmp);
                                if(features == 7)
                                    strcpy(data[count].id, tmp);
                                else if(features == 14)
                                    data[count].profit = atof(tmp);
                                token = strtok(NULL, ",");
                            }
                    }
                features = 0;
                if (count == 10000) break;
            }
    }

void output(sale_struct x[]){
    int i;
    for (i=1; i<=count; i++)
        printf("%d\t%s\t%.1f\n ",i,x[i].id,x[i].profit);
}

void exch(sale_struct a[], int i, int j) {
    sale_struct temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}


void quicksort_2_way(sale_struct a[], int l, int r) {
    if (l > r) return;
    int i = l, j = r + 1;
    float v = a[l].profit;
    
    while(1) {
        while (a[++i].profit < v) if (i >= r) break;
        while (a[--j].profit > v) if (j <= l) break;
        if (i >= j) break;
        exch(a, i, j);
    }
    exch(a, l, j);
    quicksort_2_way(a, l, j-1);
    quicksort_2_way(a, j+1, r);
}


 
void write_to_file(char const *fileName) 
{ 
  FILE *f = fopen(fileName, "w"); 
  if (f == NULL) 
    {
        perror("Unable to open the file.");
        exit(1);
    }
  int i;
  for(i = 1; i<= count; i++)
    // you might want to check for out-of-disk-space here, too 
    fprintf(f, "%d,%s,%f\n", i, data[i].id, data[i].profit);  
  fclose(f); 
} 

int main()
    {
        read_csv();  
        // output(data,10);  
        printf("\n\n");
        quicksort_2_way(data,1,10000);
        // output(data);
        write_to_file("output_data.csv");
    }