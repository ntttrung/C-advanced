#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

int countCycle = 0;

typedef struct {
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph(){
    JRB edges = make_jrb();
    JRB vertices = make_jrb();
    Graph g;
    g.edges = edges;
    g.vertices = vertices;
    return g;
}

void addEdge(Graph graph, int v1, int v2){
    JRB g = graph.edges;
    JRB tree;
    JRB node = jrb_find_int(g, v1);
    if (node == NULL) {
        tree = make_jrb(); 
        jrb_insert_int(g, v1, new_jval_v(tree));
    } else {
        tree = (JRB) jval_v(node->val); 
    }
    jrb_insert_int(tree, v2, new_jval_i(1));
}

void hasEdge(Graph graph, int v1, int v2){
    JRB g = graph.edges;
    int has = 0;
    JRB node2;
    JRB node1 = jrb_find_int(g, v1);
    if (node1 == NULL) {
        has = 0;
    } else {
        node2 = jrb_find_int((JRB) jval_v(node1->val), v2); 
        if (node2 == NULL) has = 0;
        else has = 1;
    }
    if (has = 0) printf("Edge (%d, %d) doesn't exist.\n", v1, v2);
    else printf("Edge (%d, %d) exist.\n", v1, v2);
}

void addVertex(Graph graph, int id, char* name){
    JRB g = graph.vertices;
    JRB node = jrb_find_int(g, id);
    if (node == NULL) {
        jrb_insert_int(g, id, new_jval_s(name));
    } else {
        printf("ID %d already exists. Updated.\n", id);
        node->val = new_jval_s(name);
    }
}

char* getVertex(Graph graph, int id){
    JRB g = graph.vertices;
    JRB node = jrb_find_int(g, id);
    if (node == NULL) {
        return NULL;
    } else {
        return jval_s(node->val);
    }
}

int getAdjacentVertices(Graph graph, int v, int* output){
    JRB g = graph.edges;
    JRB node = jrb_find_int(g, v);
    JRB tmp;
    if (node == NULL) {
        return -1;
    }
    else {
        JRB tree = (JRB) jval_v(node->val);
        int total = 0;
        jrb_traverse(tmp, tree){
            output[total++] = jval_i(tmp->key);
        }
        return total; //number of adjacents of vertex
    }
}

void print_graph(Graph graph){
    JRB g = graph.edges;
    JRB i;
    int size;
    jrb_traverse(i, g){
    printf("Key %s: [", getVertex(graph, jval_i(i->key)));
    int output[10];
    size = getAdjacentVertices(graph, jval_i(i->key), output);
    for (int j = 0; j < size; j++){
        printf("%s ", getVertex(graph, output[j]));
    }
    printf("]\n");
  }
}

int indegree(Graph graph, int v, int* output){
    JRB g = graph.edges;
    JRB i;
    int size;
    printf("In degree of %s: [", getVertex(graph, v));
    jrb_traverse(i, g){
    int output[10];
    size = getAdjacentVertices(graph, jval_i(i->key), output);
    for (int j = 0; j < size; j++){
        if (output[j] == v) printf("%s ", getVertex(graph, jval_i(i->key)));
    }
  }
  printf("]\n");
}

int outdegree(Graph graph, int v, int* output){
    JRB g = graph.edges;
    JRB i;
    int size;
    i = jrb_find_int(g, v);
    if (i == NULL) {
        printf("No out degree of %s.", getVertex(graph, v));
        return -1;
    }
    else {
    printf("Out degree of %s: ", getVertex(graph, jval_i(i->key)));
    int output[10];
    size = getAdjacentVertices(graph, v, output);
    for (int j = 0; j < size; j++){
        printf("%s ", getVertex(graph, output[j]));
    }
    printf("\n");
    return size;
    }  
}


void DFS(Graph g, int start, int stop, void (*func)(int)){
    JRB graph = g.edges;
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
            size = getAdjacentVertices(g, current, Adj);
            for (int i = 0; i < size; i++){
                if(!visited[Adj[i]])
                    dll_append(stack, new_jval_i(Adj[i]));
                }
        }
    }
}

void printCycle(Dllist stack, int v) {
    Dllist stack2;
    stack2 = new_dllist();
    Dllist node;
    int current;
    node = dll_last(stack);
    current = jval_i(node->val);
    dll_append(stack2, new_jval_i(current));
    dll_delete_node(node);
    while (jval_i(dll_last(stack2)->val) != v) {
        dll_append(stack2, dll_last(stack)->val);
        dll_delete_node(dll_last(stack));
    }
    while (!dll_empty(stack2)) {
        printf("%d ", jval_i(dll_last(stack2)->val));
        dll_append(stack, dll_last(stack2)->val);
        dll_delete_node(dll_last(stack2));
    }
    printf("\n");
}

void processDFSTree(Graph g, Dllist stack, int* visited) {
    Dllist node;
    int current;
    node = dll_last(stack);
    current = jval_i(node->val);
    int adjacent[100];
    int size, v;
    size = getAdjacentVertices(g, current, adjacent);
    for(int i = 0; i < size; i++) {
        v = adjacent[i];
        if (visited[v] == -1) {
            printCycle(stack, v);
            countCycle++;
        }
        if (visited[v] == 0) {
            dll_append(stack, new_jval_i(v));
            visited[v] = -1;
            processDFSTree(g, stack, visited);
        } 
    }
    visited[current] = 2;
    dll_delete_node(node);
}

void findCycles(Graph g) {
    int visited[100];
    memset(visited, 0, 100);
    JRB i;
    Dllist stack;
    int v;
    jrb_traverse(i, g.vertices) {
        v = jval_i(i->key);
        if (!visited[v]) {
            stack = new_dllist();
            dll_append(stack, new_jval_i(v));
            visited[v] = -1;
            processDFSTree(g, stack, visited);
        }
    }
}





// void BFS(Graph g, int start, int stop, void (*report)(int)){
//     int visited[100];
//     int Adj[100];
//     int size_adj, u;
//     Dllist node, queue;

//     memset(visited, 0, 100);
//     queue = new_dllist();
//     dll_append(queue, new_jval_i(start));

//     while(!dll_empty(queue)){

//         node = dll_first(queue);
//         u = jval_i(node->val);
//         dll_delete_node(node);

//         if (!visited[u]) {
//             report(u);
//             visited[u] = 1;
//             if (u == stop) break;
//             size_adj = getAdjacentVertices(g, u, Adj);
//             for(int i = 0; i < size_adj; i++){
//                 if(!visited[Adj[i]]) dll_append(queue, new_jval_i(Adj[i]));
//             }
//         }
//     }
// }


// void report(int u){
//     printf("Visited %d.\n", u);
// }

// void dropGraph(Graph* g){
//     free(g);
// }

int main(){
    Graph g = createGraph();
    addVertex(g, 0, "A");
    addVertex(g, 1, "B");
    addVertex(g, 2, "C");
    addVertex(g, 3, "D");
    addVertex(g, 4, "E");
    addVertex(g, 5, "Y");
    addVertex(g, 5, "F");
    addVertex(g, 6, "G");
    
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 5, 6);
    addEdge(g, 5, 1);
    addEdge(g, 6, 5);
    addEdge(g, 3, 4);
    addEdge(g, 2, 4);
    addEdge(g, 4, 0);

    print_graph(g);
    int output[10];
    indegree(g, 1, output);
    if (countCycle == 0) 
        printf("It is DAG.\n");
    else
        printf("It is not DAG.\n");
    printf("\n");
    findCycles(g);
}