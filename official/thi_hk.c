
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "jrb.h"
#include "dllist.h"

typedef JRB Graph;

Graph createGraph();
void readFile(Graph graph, int *a);
void cau2_nut_dinh(Graph graph, int *nut, int *dinh);
void cau3_lienthong(Graph graph);
void cau4_lienthong(Graph graph);
void cau5_nutdao(Graph graph);
void cau6_duongdi(Graph graph);
void cau7_DFS(Graph graph);
void addEdge(Graph graph, int v1, int v2);
void addVertices(Graph graph, int v);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int vertex, int* output);
void dropGraph(Graph graph);


void BFS(Graph graph, int start, int stop, int *parent);
void DFS(Graph graph, int start, int stop);

void printVertex(int v)
{
  printf("%d%d->", v/5, v%5);
}
int menu(){
  int ch;
  printf("--------------------------------\n");
  printf("1. Doc ma tran va luu vao do thi\n");
  printf("2. In tong so nut, tong so cung\n");
  printf("3. Tim cac diem lien thong\n");
  printf("4. Danh sach dinh lien thong lon nhat\n");
  printf("5. Danh sach cac nut la nut dao\n");
  printf("6. In ra duong di ngan nhat\n");
  printf("7. Tong so cach di tu 2 diem bat ki\n");
  printf("Ban chon: ");
  scanf("%d",&ch);
  return ch;
}

int main()
{
    Graph g = createGraph();
    int a[25],i,ch, dinh, canh, n, output[5];
    do{
      ch=menu();
      switch(ch){
      case 1: 
	readFile(g,a);
	break;
      case 2:
	cau2_nut_dinh(g,&dinh, &canh);
	printf("So dinh: %d\nSo canh: %d\n",dinh, canh);
	break;
      case 3:
	cau3_lienthong(g);
	break;
      case 4:
	cau4_lienthong(g);
	break;
      case 5:
	cau5_nutdao(g);
	break;
      case 6: 
	cau6_duongdi(g);
	break;
      case 7:
	cau7_DFS(g);
	break;
      default: dropGraph(g);break;
      }
    }while(1<=ch && ch<=7);
    return 0; 
}
void cau6_duongdi(Graph g){
  char s1[3], s2[3];
  int i,j,v1, v2;
  int parent[25], b[25];
  printf("Nhap diem thu nhat: ");scanf("%s",s1);
  printf("Nhap diem thu hai: ");scanf("%s",s2);
  v1=(s1[0]-'0')*5 + s1[1]-'0';
  v2=(s2[0]-'0')*5 + s2[1]-'0';
  BFS(g, v1, v2, parent);
  i=0;
  b[0]=v2;
  while(parent[v2]!=v1) {
    v2=parent[v2];
    b[++i]=v2;
  }
  b[++i]=v1;
  for(j=i;j>0;j--) printf("%d%d->",b[j]/5, b[j]%5);
  printf("%d%d\n",b[0]/5,b[0]%5);

}
void cau5_nutdao(Graph g){
   int dem;
   JRB node1, node2, tree;
   printf("Danh sach cac nut dao: ");
   jrb_traverse(node1, g){
     tree=(JRB) jval_v(node1->val);
     dem=0;
     jrb_traverse(node2, tree)
       dem++;
     if(dem==0) printf("%d%d ", jval_i(node1->key)/5, jval_i(node1->key)%5);
   }
   printf("\n");
}
void cau4_lienthong(Graph g){
  int output[4], max=0, i, n;
  for(i=0; i<25; i++){
    n=getAdjacentVertices(g, i, output);
    if(max<n) max=n;
  }
  printf("So luong lien thong lon nhat: %d\n", max);
  printf("Danh sach cac dinh: ");
  for(i=0; i<25; i++){
    n=getAdjacentVertices(g, i, output);
    if(n==max) printf("%d%d ",i/5, i%5);
  }
  printf("\n");

}
void cau3_lienthong(Graph g){
  char s[3];
  int v, i, n,  output[4];
  printf("Nhap dinh can xet: ");scanf("%s", s);
  v=(s[0]-'0')*5 + s[1]-'0';
  n = getAdjacentVertices(g, v, output);
  if(n==0) printf("Khong co dinh nao lien ke");
  else
    for(i=0; i<n; i++){
      printf("%d%d ", output[i]/5, output[i]%5);
    }
  printf("\n");

}
void cau2_nut_dinh(Graph g, int *dinh, int *canh){
  int dem1=0, dem2=0;
  JRB node1, node2, tree;
  jrb_traverse(node1, g)
    {
       dem1++;
       tree=(JRB)jval_v(node1->val);
       jrb_traverse(node2, tree){	
	 dem2++;
       }                     
    }
  *dinh=dem1;
  *canh=dem2/2;
}
Graph createGraph()
{
   return make_jrb();
}

void readFile(Graph graph, int *a){
  FILE *f;
  int v, dem=0, i, j;
  char str[30];
  // printf("Nhap ten file: ");scanf("%s",str);
  f=fopen("dulieu.txt","r");
  while(fscanf(f, "%d", &v)!=EOF){
    a[dem++] = v;   
  }
  for(i=0;i<dem;i++){
      printf("%d ",a[i]);
      if((i+1)%5==0) printf("\n");

      if(a[i]==0){
	addVertices(graph, i);
        if(i+1<dem && (i+1)%5!=0 &&  a[i+1]==0)
	  addEdge(graph, i,i+1);
	if(i+5<dem && a[i+5]==0)
	  addEdge(graph, i, i+5);
      }
   
  }
}
void addVertices(Graph graph, int v){
  JRB node, tree;
  node=jrb_find_int(graph, v);
  if(node==NULL){
    tree=make_jrb();
    jrb_insert_int(graph, v, new_jval_v(tree));
  }
}
void addEdge(Graph graph, int v1, int v2)
{
  JRB node, tree;
  if(!adjacent(graph, v1, v2)){
     node = jrb_find_int(graph, v1);
     if (node==NULL) {
         tree = make_jrb();
         jrb_insert_int(graph, v1, new_jval_v(tree));
	 if(v2==-1) return;
     }else
     {
        tree = (JRB) jval_v(node->val);   
     }
     jrb_insert_int(tree, v2, new_jval_i(1));
     
     node = jrb_find_int(graph, v2);
     if (node==NULL) {
         tree = make_jrb();
         jrb_insert_int(graph, v2, new_jval_v(tree));
     } else
     {
         tree = (JRB) jval_v(node->val);   
     }
     jrb_insert_int(tree, v1, new_jval_i(1));   
  }
    
}

int adjacent(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph, v1);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    if (jrb_find_int(tree, v2)==NULL)
       return 0;
    else
       return 1;       
}

int getAdjacentVertices (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph);
}

void BFS(Graph graph, int start, int stop, int *parent)
{
   int visited[25] = {};
   int n, output[25], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          visited[u] = 1;
          if ( u == stop){
	    return;           
	  }
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v]){
		parent[v]=u;
                 dll_append(queue, new_jval_i(v));
	      }
          }
      }
   }                            
}

void DFS_dequy(Graph graph, int start, int stop, int *dem, Dllist stack, int *visited)
{
   int visited2[25], output[25];
   int n, i, u, v;
   Dllist node;

   //Ket thuc de quy   
   if(dll_empty(stack)) {
     return;
   }
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          printVertex(u);                  
	  if(u==stop) {
	    (*dem)++;
	    printf("OK\n");            	    
	  }
          visited[u] = 1;  
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v]){
	        dll_append(stack, new_jval_i(v));
		memcpy(visited2, visited, sizeof(int)*25);
		DFS_dequy(graph,start, stop, dem, stack, visited2);
	      }
          }
      }
                              
   
}

void cau7_DFS(Graph graph){
  Dllist stack;
  int i, start, stop, dem=0, visited[25]={};
  char s1[3], s2[3];
  printf("Nhap diem thu nhat: ");scanf("%s",s1);
  printf("Nhap diem thu hai: ");scanf("%s",s2);
  start = (s1[0]-'0')*5 + s1[1]-'0';
  stop = (s2[0]-'0')*5 + s2[1]-'0';
  stack=new_dllist();
  dll_append(stack, new_jval_i(start));
  DFS_dequy(graph, start, stop, &dem, stack, visited);
  printf("So cach di: %d\n", dem);
}


