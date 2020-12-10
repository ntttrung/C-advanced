#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#define INFINITY 1000;

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

void addEdge(Graph graph, int v1, int v2, double w){
    JRB g = graph.edges;
    JRB tree;
    JRB node = jrb_find_int(g, v1);
    if (node == NULL) {
        tree = make_jrb(); 
        jrb_insert_int(g, v1, new_jval_v(tree));
    } else {
        tree = (JRB) jval_v(node->val); 
    }
    jrb_insert_int(tree, v2, new_jval_d(w));
}

double getEdgeValue(Graph graph, int v1, int v2)
    {
        JRB g = graph.edges;
        JRB node2;
        JRB node1 = jrb_find_int(g, v1);
        if(node1 == NULL)
            return INFINITY;
        node2 = jrb_find_int((JRB) jval_v(node1->val), v2);
        if(node2 == NULL)
            return INFINITY;
        return jval_d(node2->val);
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
    int size,j;
    jrb_traverse(i, g){
    printf("Key %s: [", getVertex(graph, jval_i(i->key)));
    int output[10];
    size = getAdjacentVertices(graph, jval_i(i->key), output);
    for (j = 0; j < size; j++){
        printf("%s ", getVertex(graph, output[j]));
    }
    printf("]\n");
  }
}

int indegree(Graph graph, int v, int* out){
    int output[100];
    JRB g = graph.edges;
    JRB i;
    int size,j;
    int count = 0;
    // printf("In degree of %s: [", getVertex(graph, v));
    jrb_traverse(i, g){
    int output[10];
    size = getAdjacentVertices(graph, jval_i(i->key), output);
    for (j = 0; j < size; j++){
        if (output[j] == v){
            // printf("%s ", getVertex(graph, jval_i(i->key)));
            out[count++] = jval_i(i->key);
        }
    }
  }
//   printf("]\n");
  return count;
}

int outdegree(Graph graph, int v, int* out){
    JRB g = graph.edges;
    JRB i;
    int size,j;
    i = jrb_find_int(g, v);
    if (i == NULL) {
        // printf("No out degree of %s.", getVertex(graph, v));
        return 0;
    }
    else {
    // printf("Out degree of %s: ", getVertex(graph, jval_i(i->key)));
    int output[10];
    size = getAdjacentVertices(graph, v, output);
    for (j = 0; j < size; j++){
        out[j] = output[j];
    }
    // printf("\n");
    return size;
    }  
}

int shortestPath(Graph graph, int s, int t, int* path, int* length)
    {
        JRB g = graph.vertices;
        double distance[100];
        int parent[100];
        int visited[100],i;
        // memset(distance, 1000, 100);
        // memset(parent, -1, 100);
        // memset(visited, 0, 100);
        for(i = 0; i< 100; i++)
            {
                distance[i] = INFINITY;
                parent[i] = -1;
                visited[i] = 0;
            }
        distance[s] = 0;
        parent[s] = s;
        visited[s] = 1;
        
        Dllist  queue, node;
        queue = new_dllist();
        dll_append(queue, new_jval_i(s));
        while(!dll_empty(queue))
            {
                node = dll_first(queue);
                int u = jval_i(node->val);
                dll_delete_node(node);
                int output[100];
                int out_de = outdegree(graph, u, output);
                for(i = 0; i < out_de; i++)
                    {
                        if(visited[output[i]] == 0)
                            {
                                visited[output[i]] = 1;
                                dll_append(queue, new_jval_i(output[i]));
                            }
                        if((getEdgeValue(graph, u, output[i]) + distance[u]) < distance[output[i]])
                            {
                                distance[output[i]] = getEdgeValue(graph, u, output[i]) + distance[u];
                                parent[output[i]] = u;
                            }
                    }
            }
        path[0] = t;
        int length1 = 1;
        int current = t;
        while(current != s)
            {
                path[length1] = parent[current];
                length1++;
                current = parent[current];
            }
        *length = length1;
        return length1;

    }


void DFS(Graph g, int start, int stop, void (*func)(int)){
    JRB graph = g.edges;
    Dllist stack, node;
    stack = new_dllist();
    int visited[100], Adj[100], current, size, i;
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
            for (i = 0; i < size; i++){
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
    int adjacent[100], i;
    int size, v;
    size = getAdjacentVertices(g, current, adjacent);
    for(i = 0; i < size; i++) {
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
    stack = new_dllist();
    jrb_traverse(i, g.vertices) {
        v = jval_i(i->key);
        if (!visited[v]) {
            dll_append(stack, new_jval_i(v));
            visited[v] = -1;
            processDFSTree(g, stack, visited);
        }
    }
}

void topologicalSort(Graph g, int* output, int* n){
    int indegree1[100];
    JRB node;
    int out[100];
    jrb_traverse(node, g.vertices) {
        int key = jval_i(node->key);
        indegree1[key] = indegree(g, key, out);
    }
    JRB v;
    Dllist queue;
    queue = new_dllist();
    jrb_traverse(v, g.vertices) {
        if (indegree1[jval_i(v->key)] == 0)
            dll_append(queue, v->key);
    }
    int adjacent[100];
    int size,i;
    Dllist u;
    while(!dll_empty(queue)) {
        u = dll_first(queue);
        printf("%d ", jval_i(u->val));
        size = getAdjacentVertices(g, jval_i(u->val), adjacent);
        // printf("%d ", size);
        dll_delete_node(u);
        // printf("\n");
        for ( i = 0; i < size; i++){
            // printf("%d ",  adjacent[i]);
            indegree1[adjacent[i]]--;
            if (indegree1[adjacent[i]] == 0) 
                dll_append(queue, new_jval_i(adjacent[i]));
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
    int i;
    int path[100], size;
    int *ln;
    Graph g = createGraph();
    addVertex(g, 0, "A");
    addVertex(g, 1, "B");
    addVertex(g, 2, "C");
    addVertex(g, 3, "D");
    addVertex(g, 4, "E");
    addVertex(g, 5, "Y");
    addVertex(g, 5, "F");
    addVertex(g, 6, "G");

    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 2, 20);
    addEdge(g, 5, 6, 3);
    addEdge(g, 5, 1, 40);
    addEdge(g, 3, 4, 2);
    addEdge(g, 2, 4, 35);


    print_graph(g);

    size = shortestPath(g, 0, 4, path, &ln);
    for(i = 0; i < size; i++)
        printf("%d ", path[i]);
    // printf("%f", getEdgeValue(g,5,6));

}