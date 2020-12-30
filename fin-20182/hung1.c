#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#define INFINITE_VALUE 1000000000

typedef struct {
    JRB distance;
    JRB edge;
    JRB vertex;
} Graph;

Graph createGraph();
void dropGraph(Graph g);
void addVertex(Graph g, int vertex, int id);
void addEdge(Graph g, int v1, int v2, int weight);
int adjacent(Graph g, int id1, int id2);
int getEdgeValue(Graph g, int v1, int v2);
int getEdgeValue1(Graph g, int v1, int v2);
int getAdjacentVertex(Graph g, int vertex, int *output);
int indegree(Graph g, int id, int *output);
int outdegree(Graph g, int id, int *output);
int outdegree1(Graph g, int id, int *output);
void adjacentList(Graph g);
void matrix1(Graph g, int m);
void matrix2(Graph g, int m);
void list(Graph g);
int BFS4(Graph g, int start, int end);
int shortestPath(Graph graph, int s, int t, int path[], int *length);
void alltheway(Graph g, int s, int t);
Graph createGraph(){
    Graph g;
    g.distance = make_jrb();
    g.edge = make_jrb();
    g.vertex = make_jrb();
    return g;
}

void dropGraph(Graph g){
    jrb_free_tree(g.edge);
    jrb_free_tree(g.distance);
    jrb_free_tree(g.vertex);
}

void addVertex(Graph g, int vertex, int id){
    if(jrb_find_int(g.vertex, vertex) == NULL){
        jrb_insert_int(g.vertex, id, new_jval_i(id));
    }
}

void addEdge(Graph g, int v1, int v2, int weight){
    JRB tree = make_jrb();
    if(jrb_find_int(g.edge, v1) == NULL){
        jrb_insert_int(g.edge, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_i(weight));
    }
    else {
        JRB node = jrb_find_int(g.edge, v1);
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(weight));
    }

    tree = make_jrb();
     if(jrb_find_int(g.edge, v2) == NULL){
        jrb_insert_int(g.edge, v2, new_jval_v(tree));
        jrb_insert_int(tree, v1, new_jval_i(weight));
    }
    else {
        JRB node = jrb_find_int(g.edge, v2);
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v1, new_jval_i(weight));
    }
}

void addEdge1(Graph g, int v1, int v2, int weight){
    JRB tree = make_jrb();
    if(jrb_find_int(g.distance, v1) == NULL){
        jrb_insert_int(g.distance, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_i(weight));
    }
    else {
        JRB node = jrb_find_int(g.distance, v1);
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(weight));
    }

    tree = make_jrb();
     if(jrb_find_int(g.distance, v2) == NULL){
        jrb_insert_int(g.distance, v2, new_jval_v(tree));
        jrb_insert_int(tree, v1, new_jval_i(weight));
    }
    else {
        JRB node = jrb_find_int(g.distance, v2);
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v1, new_jval_i(weight));
    }
}
int adjacent(Graph g, int id1, int id2){
    JRB node = jrb_find_int(g.vertex, id1);
    if(node == NULL){
        return 0;
    }
    else{
        JRB tree = (JRB) jval_v(node->val);
        if(jrb_find_int(tree, id2) == NULL){
            return 0;
        }
        else {
            return 1;
        }
    }
}

int getAdjacentVertex(Graph g, int vertex, int *output){
    int total = 0;
    JRB node = jrb_find_int(g.vertex, vertex);
    JRB tree = (JRB)jval_v(node->val);
    JRB newNode;
    jrb_traverse(newNode, tree){
        output[total++] = jval_i(newNode->key);
    }
}

int getEdgeValue(Graph g, int v1, int v2){
    JRB node1 = jrb_find_int(g.edge, v1);
    if (node1 == NULL){
        return INFINITE_VALUE;
    }
    else {
        JRB tree;
        tree = (JRB) jval_v(node1->val);
        JRB node2 = jrb_find_int(tree, v2);
        if(node2 == NULL){
            return INFINITE_VALUE; 
        }
        else {
            return jval_i(node2->val);
        }
    }
}
int getEdgeValue1(Graph g, int v1, int v2){
    JRB node1 = jrb_find_int(g.distance, v1);
    if (node1 == NULL){
        return INFINITE_VALUE;
    }
    else {
        JRB tree;
        tree = (JRB) jval_v(node1->val);
        JRB node2 = jrb_find_int(tree, v2);
        if(node2 == NULL){
            return INFINITE_VALUE; 
        }
        else {
            return jval_i(node2->val);
        }
    }
}

int indegree(Graph g, int id, int *output){
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, g.edge){
        tree = (JRB)jval_v(node->val);
        if(jrb_find_int(tree, id)){
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph g, int id, int *output){
    int total = 0;
    JRB tree, node, treeNode;
    node = jrb_find_int(g.edge, id);
    if(node == NULL){
        return 0;
    } 
    else{
        tree = (JRB)jval_v(node->val);
        jrb_traverse(treeNode, tree){
            output[total++] = jval_i(treeNode->key);
        }
        return total;
    }
}
int outdegree1(Graph g, int id, int *output){
    int total = 0;
    JRB tree, node, treeNode;
    node = jrb_find_int(g.distance, id);
    if(node == NULL){
        return 0;
    } 
    else{
        tree = (JRB)jval_v(node->val);
        jrb_traverse(treeNode, tree){
            output[total++] = jval_i(treeNode->key);
        }
        return total;
    }
}

void matrix2(Graph g, int m){
    JRB node, I, J;
    JRB tree = make_jrb();

    printf("Ma tran ke cua distance: \n");
    printf("\n%3s"," ");
	jrb_traverse(I,g.distance)
	  {
	    printf("%4d",jval_i(I->key));
	  }
	printf("\n");
	jrb_traverse(I,g.distance)
	  {
	    printf("%3d",jval_i(I->key));
	    jrb_traverse(J,g.distance)
	      {
            node = jrb_find_int(g.distance,jval_i(J->key));
            tree = (JRB) jval_v(node->val);
            node = jrb_find_int(tree,jval_i(I->key));
            if (node == NULL) { 
                printf("%4s", "0");
            }
            else {
                printf("%4d",jval_i(node->val));
            }
	      }
	    printf("\n\n");
	  }
}
void matrix1(Graph g, int m){
    JRB node, I, J;
    JRB tree = make_jrb();
    printf("Ma tran ke cua time: \n");
    printf("\n%3s"," ");
	jrb_traverse(I,g.edge)
	  {
	    printf("%4d",jval_i(I->key));
	  }
	printf("\n\n");
	jrb_traverse(I,g.edge)
	  {
	    printf("%3d",jval_i(I->key));
	    jrb_traverse(J,g.edge)
	      {
            node = jrb_find_int(g.edge,jval_i(J->key));
            tree = (JRB) jval_v(node->val);
            node = jrb_find_int(tree,jval_i(I->key));
            if (node == NULL) { 
                printf("%4s", "0");
            }
            else {
                printf("%4d",jval_i(node->val));
            }
	      }
	    printf("\n\n");
	  }
}

void list(Graph g){
    JRB node, treeNode;
    JRB tree = make_jrb();
    int total = 0;
    int output[100];
    char *s = (char*)malloc(sizeof(char)*50);
    jrb_traverse(node, g.edge){
        if(node != NULL){
            printf("Castle %d : ", jval_i(node->key));
            tree = (JRB)jval_v(node->val);
            jrb_traverse(treeNode, tree){
                if(treeNode != NULL){
                    output[total++] = jval_i(treeNode->key);
                }
            }
            for(int k = total-1; k >= 0; k--){
                printf("%5d", output[k]);
            }
            total = 0;
            printf("\n");
        }
    }
}

int BFS4(Graph g, int start, int end)
{
  JRB tree = make_jrb();
  Dllist queue = new_dllist();
  if ( start == end) return -1;
  if (jrb_find_int(g.distance,start) == NULL)
    {
      printf("Don't have %d in tree\n",start);
      return 0;
    }
  else
    {
      int visit[1000];
      memset(visit,0,1000);
      dll_append(queue, new_jval_i(start));
      visit[start] = 1;
      while (!dll_empty(queue))
	{
	  Dllist node = dll_first(queue);
	  int v = jval_i(node->val);
	  if (v == end) return 1;
	  dll_delete_node(node);
       	  int arr[100];
	  int n = outdegree(g,v,arr);
	  int i;
	  for (i = 0; i < n ; i++)
	    {
	      if(visit[arr[i]] == 0)
		{
		  dll_append(queue,new_jval_i(arr[i]));
		  visit[arr[i]] = 1;
		}
	    }
	}
      return 0;
    }
  
}


// Shortest Path
int shortestPath(Graph graph, int s, int t, int path[], int *length){
    int distance[1000];
    int previous[1000], u, visit[1000];
    if (BFS4(graph,s,t) == 0) return INFINITE_VALUE;
    if( jrb_find_int(graph.distance,s)==NULL || jrb_find_int(graph.distance,t)==NULL){
        printf("ROUTE NOT FOUND!!!!");
        return INFINITE_VALUE;
    }

    for (int i=1; i<1000; i++){
        distance[i] = INFINITE_VALUE;
        visit[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visit[s] = 1;
    
    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    
    // Duyet Queue
    while (!dll_empty(queue)){
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        int output[100];
        int number = outdegree1(graph,u,output);
        for (int i =0; i<number; i++) {
            if (visit[output[i]] == 0) {
                visit[output[i]] = 1;
                dll_append(queue,new_jval_i(output[i]));
            }
            if ((getEdgeValue1(graph,u,output[i])+distance[u])<distance[output[i]]) {
                distance[output[i]]= getEdgeValue1(graph,u,output[i])+distance[u];
                previous[output[i]] = u;  
            }
        }
    }
    path[0] = t;
    int length1 = 1;
    int cur = t;
    while (cur != s){
        path[length1] = previous[cur];
        length1 = length1+1;
        cur = previous[cur];
    }
    
    *length  = length1;
    return distance[t];
}

int BFScase5(Graph g, int start, int end)
{
  JRB tree = make_jrb();
  Dllist queue = new_dllist();
  if ( start == end) return -1;
  if (jrb_find_int(g.edge,start) == NULL)
    {
      printf("Don't have %d in tree\n",start);
      return 0;
    }
  else
    {
      int visit[1000];
      memset(visit,0,1000);
      dll_append(queue, new_jval_i(start));
      visit[start] = 1;
      while (!dll_empty(queue))
	{
	  Dllist node = dll_first(queue);
	  int v = jval_i(node->val);
	  if (v == end) return 1;
	  dll_delete_node(node);
       	  int arr[100];
	  int n = outdegree(g,v,arr);
	  int i;
	  for (i = 0; i < n ; i++)
	    if(getEdgeValue(g,v,arr[i]) >= 50)
	      {
		if(visit[arr[i]] == 0)
		  {
		    dll_append(queue,new_jval_i(arr[i]));
		    visit[arr[i]] = 1;
		  }
	      }
	}
      return 0;
    }
  
}

// Shortest Path Walking
int shortestPathWalking(Graph g, int s, int t, int path[], int *length){
    int distance[1000];
    int previous[1000], u, visit[1000];
    if(BFScase5(g, s, t) == 0) return INFINITE_VALUE;
    if( jrb_find_int(g.edge,s)==NULL || jrb_find_int(g.edge,t)==NULL)
        {
            printf("ROUTE NOT FOUND!!!!");
            return INFINITE_VALUE;
        }

    for (int i=1; i<1000; i++){
        distance[i] = INFINITE_VALUE;
        visit[i] = 0;
        previous[i] = 0;
    }

    distance[s] = 0;
    previous[s] = s;
    visit[s] = 1;
    
    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    
    // Duyet Queue
    while (!dll_empty(queue)){
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        int output[100];
        int number = outdegree(g,u,output);
        for (int i =0; i<number; i++) {
            if(getEdgeValue(g, output[i], u) >= 50){
                if (visit[output[i]]==0) {
                    visit[output[i]] = 1;
                    dll_append(queue,new_jval_i(output[i]));
                }
                if ((getEdgeValue(g,u,output[i])+distance[u])<distance[output[i]]) {
                    distance[output[i]]= getEdgeValue(g,u,output[i])+distance[u];
                    previous[output[i]] = u;   
                }
            }
        }
    }
    path[0] = t;
    int length1 = 1;
    int cur = t;
    while (cur != s){
        path[length1] = previous[cur];
        length1 = length1+1;
        cur = previous[cur];
    }
    *length  = length1;
    return distance[t];
}

void alltheway(Graph g, int s, int t){
    if (BFS4(g,s,t) == 0) {
        printf("ROUTE NOT FOUND!!!\n");
    }
    // if( jrb_find_int(g.edge,s)==NULL || jrb_find_int(g.edge,t)==NULL){
    //     printf("ROUTE NOT FOUND!!!\n");
    // }
    else {
        int distance [1000];
        int previous[1000], u, visit[1000];
        int path[1000];
        for (int i=1; i<1000; i++){
        distance[i] = INFINITE_VALUE;
        visit[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visit[s] = 1;
    
    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    
    // Duyet Queue
    while (!dll_empty(queue)){
    while (!dll_empty(queue)){
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        int output[100];
        int number = outdegree1(g,u,output);
        for (int i =0; i<number; i++) {
            if (visit[output[i]] == 0) {
                visit[output[i]] = 1;
                dll_append(queue,new_jval_i(output[i]));
                previous[output[i]] = u;  
            }
        }
    } 
    path[0] = t;
    int length1 = 1;
    int cur = t;
    while (cur != s){
        path[length1] = previous[cur];
        length1 = length1+1;
        cur = previous[cur];
    }
    for(int i = length1-1; i >= 0; i--){
        printf("%d ", path[i]);
    }
    printf("\n");
    }
    }
}

void clearBuffer() {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

int choice;
void menu() {
    printf("\n---------------------------------------------------------------------------------------------------\n");
    printf("|                                              MENU                                                |\n");
    printf("---------------------------------------------------------------------------------------------------|\n");
    printf("| 1. Doc ban do va luu JRB va in ma tran ke                                                        |\n");
    printf("| 2. In ra danh sach ke theo thu tu giam dan                                                       |\n");
    printf("| 3. Tim duong di ngan nhat tu thanh tri s den thanh tri t                                         |\n");
    printf("| 4. Tim duong di bo ngan nhat tu thanh tri s den thanh tri t                                      |\n");
    printf("| 5. Tim tat ca cac duong tu thanh tri s den thanh tri t                                           |\n");
    printf("| 6. Exit                                                                                          |\n");
    printf("|__________________________________________________________________________________________________|\n");
    printf("Your choice : "); scanf("%d", &choice); getc(stdin);
}

int main(void){
    FILE *fin;
    Graph g = createGraph();
    int start, end, distance, distanceWalking;
    int path[100], length;
    int m, n, a, b, c, d;
    do {
        menu();
        switch(choice) {
            case 1: 
                fin = fopen("dothi.txt", "r+");
                fscanf(fin, "%d\t%d\n", &n, &m);
                // printf("%d\t%d\n\n", n, m);
                while(fscanf(fin, "%d\t%d\t%d\t%d", &a, &b, &c, &d) != EOF){
                    addEdge(g, a, b, d);
                    addEdge1(g, a, b, c);
                }
                fclose(fin);
                printf("\n");
                matrix1(g, n);
                matrix2(g, n);
                break;
            case 2: 
                list(g);
                break;
            case 3:
            printf("Nhap thanh tri xuat phat: ");
                scanf("%d%*c", &start);
                printf("Nhap thanh tri dich : ");
                scanf("%d%*c", &end);
                distance = shortestPath(g,start, end, path, &length);
                if(distance >= INFINITE_VALUE) printf("ROUTE NOT FOUND\n\n");
                else {
                    printf("Do dai duong di: %d\n", distance);
                    printf("Thu tu cac thanh tri phai di qua: ");  
                    for(int i = length-1; i >= 0; i--){
                        printf("%d ", path[i]);
                    }
                }
                printf("\n");
                break;
            case 4:
                printf("Nhap thanh tri xuat phat: ");
                scanf("%d%*c", &start);
                printf("Nhap thanh tri dich : ");
                scanf("%d%*c", &end);
                distanceWalking = shortestPathWalking(g,start, end, path, &length);
                if(distanceWalking >= INFINITE_VALUE) printf("ROUTE NOT FOUND\n\n");
                else {
                    printf("Thoi gian di: %d\n", distanceWalking);
                    printf("Thu tu cac thanh tri phai di qua: ");  
                    for(int i = length-1; i >= 0; i--){
                        printf("%d ", path[i]);
                    }
                    printf("\n");
                }
                break;
            case 5: 
                printf("Nhap thanh tri xuat phat: ");
                scanf("%d%*c", &start);
                printf("Nhap thanh tri dich : ");
                scanf("%d%*c", &end);
                alltheway(g, start, end);
                break;              
            case 6: 
                printf("GOOD BYE!!!\n");
                break;
            default:
                printf("Please choose in range[1..6]\n");
                break;
        }
    } while (choice != 6);
    dropGraph(g);
}
