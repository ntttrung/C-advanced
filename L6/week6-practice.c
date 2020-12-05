/* Graph using matrix to present */
// Le Thanh Hai
// Nguyen Thanh Tung
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"

typedef JRB Graph;

Graph createGraph();
void addEdge(Graph graph, int v1, int v2);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int vertex, int* output);
void dropGraph(Graph graph);
void printGraph(Graph graph);
void printVertex(int v);
void BFS(Graph graph, int start, int stop, void (*func)(int));
void DFS(Graph graph, int start, int stop, void (*func)(int));

int main(){
    Graph g = createGraph();
    
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 4);
    addEdge(g, 4, 5);
    printGraph(g);
    
    //BFS(g, 3, 5, printVertex);
    DFS(g, 3, 5, printVertex);
    printf("\n");
    
    dropGraph(g);
}

void printGraph(Graph g){
    Graph node;
    jrb_traverse(node, g){
        if (node != NULL){
            int output[10];
            int res = getAdjacentVertices(g, jval_i(node->key), output);
            printf("%d: ", jval_i(node->key));
            for (int i=0; i < res; i++){
                printf("%d ", output[i]);
            }
            printf("\n");
        }
    }
}

Graph createGraph(){
    return make_jrb();
}

void addEdge(Graph graph, int v1, int v2){
    Graph treeV1 = make_jrb();
    Graph treeV2 = make_jrb();
    Graph nodeV1 = jrb_find_int(graph,v1);
    Graph nodeV2 = jrb_find_int(graph,v2);
    
    if(nodeV1 == NULL){
        jrb_insert_int(graph,v1,new_jval_v(treeV1));
        jrb_insert_int(treeV1,v2,new_jval_i(v2));
    }else{
        Graph tree = (Graph)jval_v(nodeV1->val);
        jrb_insert_int(tree,v2,new_jval_i(v1));
    }
    if(nodeV2 == NULL){
        jrb_insert_int(graph,v2,new_jval_v(treeV2));
        jrb_insert_int(treeV2,v1,new_jval_i(v1));
    }else{
        Graph tree = (Graph)jval_v(nodeV2->val);
        jrb_insert_int(tree,v1,new_jval_i(v2));
    }
}

int adjacent(Graph graph, int v1, int v2){
    Graph node = jrb_find_int(graph,v1);
    Graph tree = (Graph)jval_v(node->val);
    Graph result = jrb_find_int(tree,v2);
    if(result != NULL) return 1;
    return 0;
}

int getAdjacentVertices(Graph graph, int vertex, int *output){
    Graph node = jrb_find_int(graph,vertex);
    Graph tree = (Graph)jval_v(node->val);
    Graph temp;
    int i = 0;
    jrb_traverse(temp,tree){
        output[i++] = jval_i(temp->key);
    }
    return i;
}

void dropGraph(Graph graph){
    jrb_free_tree(graph);
}

void printVertex(int v){
    printf("%4d", v);
}

void BFS(Graph graph, int start, int stop, void (*func)(int)){
    Dllist queue,node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(start));
    int visited[1000]={};
    int output[100],n,i,u,v;
    
    memset(visited,0,100);
    while(!dll_empty(queue)){
        node=dll_first(queue);
        u=jval_i(node->val);
        dll_delete_node(node);
        if(!visited[u]){
            func(u);
            visited[u]=1;
            if(u==stop) return;
            n=getAdjacentVertices(graph,u,output);
            for(i=0;i<n;i++){
                v=output[i];
                if(!visited[v])
                    dll_append(queue,new_jval_i(v));
}
        }
    }
}

void DFS(Graph graph, int start, int stop, void (*func)(int)){
    Dllist stack, node;
    stack = new_dllist();
    int visited[100], Adj[100], current, size;
    memset(visited, 0, 100);
    dll_append(stack, new_jval_i(start));
    while (!dll_empty(stack)) {
        node = dll_last(stack);
        current = jval_i(node->val);
        dll_delete_node(node);
        if(!visited[current]) {
            func(current);
            visited[current] = 1;
            if (current == stop) break;
            size = getAdjacentVertices(graph, current, Adj);
            for (int i = 0; i < size; i++){
                if(!visited[Adj[i]])
                    dll_append(stack, new_jval_i(Adj[i]));
            
                }
        }
    }
}
/*
typedef struct {
    int * matrix; // One-dimentional array
    int sizemax; // Number of vertices
} *Graph;

Graph createGraph(int sizemax);
void addEdge(Graph graph, int v1, int v2);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int vertex, int* output);
void dropGraph(Graph graph);
void printGraph(Graph graph);

Graph createGraph(int sizemax) {
    Graph tmp = malloc(sizeof(Graph));
    tmp->sizemax = sizemax;
    tmp->matrix = calloc(sizemax*sizemax,sizeof(int));
    return tmp;
}

void addEdge(Graph graph, int v1, int v2){
    graph->matrix[v1*graph->sizemax+v2] =1;
    graph->matrix[v2*graph->sizemax+v1] =1;
}

int adjacent(Graph graph, int v1, int v2){
    if (graph->matrix[v1*graph->sizemax+v2] == 1)
        return 1;
    else return 0;
}
int getAdjacentVertices(Graph graph, int vertex, int* output){
    int n=0;
    for (int i=0; i<graph->sizemax;i++) {
        if (graph->matrix[vertex*graph->sizemax+i] == 1)
            output[n++] = i;
    }
    return n;
}

void printGraph(Graph g) {
    for (int i =0; i<g->sizemax; i++) {
        for (int j =0; j<g->sizemax; j++) {
            printf("%d ",g->matrix[i*g->sizemax+j]);
        }
        printf("\n");
    }
}

void dropGraph(Graph graph){
    free(graph);
}

int main(){
    Graph g = createGraph(10);
    
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 4);
    addEdge(g, 2, 4);
    addEdge(g, 1, 3);
    addEdge(g, 3, 4);
    addEdge(g, 3, 5);
    printGraph(g);
    
    int output[10];
    int res = getAdjacentVertices(g, 3, output);
    if (res == 0){
        printf("There is no adjacent vertex.\n");
    }else{
        printf("There are %d adjacent vertices: ", res);
        for (int i=0; i<res; i++){
            printf("%d ", output[i]);
        }
        printf("\n");
    }
    
    dropGraph(g);
}
*/
