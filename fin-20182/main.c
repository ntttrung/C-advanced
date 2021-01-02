//Nguyen Thanh Trung
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

#define INFINITIVE_VALUE 10000000

int countCycle = 0;
int total_vertex = 0;
int matrix[10][10];

typedef struct {
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void dropGraph(Graph graph);


Graph createGraph(){
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}

void dropGraph(Graph graph){
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
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

char *getVertex(Graph graph, int id){
    JRB node = jrb_find_int(graph.vertices, id);
    if(node == NULL)
        return NULL;
    else
        return (jval_s(node->val));
}

// Make connection between v1 and v2
void addEdge(Graph graph, int v1, int v2,double weight){
    JRB node, tree;
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
    {
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
}

int hasEdge(Graph graph, int v1, int v2){
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
    return has;
}

double getEdgeValue(Graph graph, int v1, int v2){
    JRB node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
        return INFINITIVE_VALUE;
    }
    JRB tree = (JRB)jval_v(node->val);
    JRB node1 = jrb_find_int(tree, v2);
    if (node1 == NULL) {
        return INFINITIVE_VALUE;
    }
    return jval_d(node1->val);
}

int indegree(Graph graph, int v, int* output){
    int total = 0;
    JRB node;
    jrb_traverse(node,graph.edges) {
        JRB tree = (JRB) jval_v(node->val);
        if (jrb_find_int(tree,v) != NULL) {
            output[total++] = jval_i(node->key);
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int* output){
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
        return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree){
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

double shortestPath(Graph graph, int s, int t, int *path, int *length){
    // Khoi tao cac distance = 0
    double distance[1000], min;
    int previous[1000], u, visit[1000], i;
    
    for (i=0; i<1000; i++){
        distance[i] = INFINITIVE_VALUE;
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
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue){
            // Lay ra min{distance}
            u = jval_i(ptr->val);
            if (min > distance[u]){
                min = distance[u];
                node = ptr;
            }
        }
        
        u = jval_i(node->val);
        // printf("%d ", u);
        dll_delete_node(node);
        int output[100], i;
        int size = outdegree(graph,u,output);
        for(i = 0; i < size; i++)
        {
            if(visit[output[i]] == 0)
            {
                visit[output[i]] = 1;
                dll_append(queue, new_jval_i(output[i]));
            }
            if((getEdgeValue(graph, u, output[i]) + distance[u])
               < distance[output[i]])
            {
                distance[output[i]] = distance[u] +
                getEdgeValue(graph, u , output[i]);
                previous[output[i]] = u;
            }
        }
    }
    int current = t;
    int ln = 0;
    path[0] = t;
    while(current!=s)
    {
        ln++;
        current = previous[current];
        path[ln] = current;
    }
    *length = ln;
    return distance[t];
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
//////////////////////////////////////////////////////////////////////////
void menu()
    {
        printf("-----------------------MENU------------------------------\n");
        printf("1. Read data\n");
        printf("2. Print adjacent list\n");
        printf("3. Output\n");
        printf("4. Shortest path\n");
        printf("5. Walking path\n");
        printf("6. Exit\n");
    }
Graph ReadData()
    {
        Graph g = createGraph();
        FILE* fin;
        char str[5];
        int i, j, a, b, c, k, n;
        fin = fopen("dothi.txt", "r");
        fscanf(fin, "%d\t%d\n", &total_vertex, &k);
        for(j = 1; j <= total_vertex; j++)
            {
                itoa(j, str, 10);
                addVertex(g, j, str);
            }
        while(fscanf(fin, "%d\t%d\t%d\n", &a, &b, &c) != EOF)
            {
                addEdge(g, a, b, c);
                addEdge(g, b, a, c);
            }
        return g;
    }
void print_adjacent_matrix(Graph g)
    {
        int i, j;
        for(i = 1; i <= total_vertex; i++)
            for(j = 1; j <= total_vertex; j++)
                matrix[i][j] = 0;
        for(i = 1; i <= total_vertex; i++)
            for(j = 1; j <= total_vertex; j++)
                {
                    if(i == j)
                        matrix[i][j] = 1;
                    if(hasEdge(g, i, j) == 1)
                        {
                            matrix[i][j] = 1;
                            matrix[j][i] = 1;
                        }
                }
        for(i = 1; i <= total_vertex; i++)
            {
                for(j = 1; j <= total_vertex; j++)
                    printf("%d  ",matrix[i][j]);
                printf("\n");
            }
        
    }

void print_adjacent_list(Graph graph)
    {
        int i, j;
        for(i = 1; i <= total_vertex; i++)
            {
                printf("Castle %d: ",i);
                for(j = 1; j <= total_vertex; j++)
                    {
                        if(matrix[i][j] == 1 && i != j)
                            printf("%d\t",j);
                    }
                printf("\n");
            }
    }

void print_walking_castle(Graph graph)
    {
        int output[10];
        int result[total_vertex + 2];
        int i, j;
        for(i = 1; i <= total_vertex; i++)
            result[i] = 1;
        for(i = 1; i <= total_vertex; i++)
            for(j = 1; j <= total_vertex; j++)
                {
                    if((hasEdge(graph, j, i) == 1) && (getEdgeValue(graph, j, i) < 50) && (i != j))
                        {
                            result[i] = 0;
                            break;
                        }
                }
        for(i = 1; i <= total_vertex; i++)
            if(result[i] == 1)
                printf("%d\t", i);
        printf("\n");
    }

void indegree_list(Graph g)
    {
        int i, result[10], output[10];
        for(i = 1; i <= total_vertex; i++)
            result[i] = indegree(g, i, output);
        int max = result[1];
        for(i = 1; i <= total_vertex; i++)
            if(result[i] > max)
                max = result[i];
        for(i = 1; i <= total_vertex; i++)
            if(result[i] == max)
                printf("%d\t", i);
        printf("\n");
    }
void short_path(Graph g)
    {
        int s, t, i;
        printf("Enter start: ");
        scanf("%d", &s);
        if(jrb_find_int(g.edges, s) == NULL)
            {
                printf("ROUTE NOT FOUND\n", s);
                return -1;
            }
        printf("Enter terminate: ");
        scanf("%d", &t);
        if(jrb_find_int(g.edges, t) == NULL)
            {
                printf("ROUTE NOT FOUND\n", t);
                return -1;
            }
        int path[10];
        int* len;
        double ln = shortestPath(g, s, t, path, &len);
        printf("%.0f\n", ln);
        for(i = len; i >= 0; i--)
            printf("%d\t", path[i]);
        printf("\n");

    }

double shortestPath2(Graph graph, int s, int t, int *path, int *length){
    // Khoi tao cac distance = 0
    double distance[1000], min;
    int previous[1000], u, visit[1000], i;
    
    for (i=0; i<1000; i++){
        distance[i] = INFINITIVE_VALUE;
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
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue){
            // Lay ra min{distance}
            u = jval_i(ptr->val);
            if (min > distance[u]){
                min = distance[u];
                node = ptr;
            }
        }
        
        u = jval_i(node->val);
        // printf("%d ", u);
        dll_delete_node(node);
        int output[100], i;
        int size = outdegree(graph,u,output);
        for(i = 0; i < size; i++)
        {
            if(getEdgeValue(graph, u, output[i]) >= 50)
                {
                     if(visit[output[i]] == 0)
                    {
                        visit[output[i]] = 1;
                        dll_append(queue, new_jval_i(output[i]));
                    }
                    if(((getEdgeValue(graph, u, output[i]) + distance[u])
                    < distance[output[i]]) && getEdgeValue(graph, u, output[i]) >= 50)
                    {
                        distance[output[i]] = distance[u] +
                        getEdgeValue(graph, u , output[i]);
                        previous[output[i]] = u;
                    }
                }
        }
    }
    int current = t;
    int ln = 0;
    path[0] = t;
    while(current!=s)
    {
        ln++;
        current = previous[current];
        path[ln] = current;
    }
    *length = ln;
    return distance[t];
}

void short_path2(Graph g)
    {
        int s, t, i;
        printf("Enter start: ");
        scanf("%d", &s);
        if(jrb_find_int(g.edges, s) == NULL)
            {
                printf("ROUTE NOT FOUND\n", s);
                return -1;
            }
        printf("Enter terminate: ");
        scanf("%d", &t);
        if(jrb_find_int(g.edges, t) == NULL)
            {
                printf("ROUTE NOT FOUND\n", t);
                return -1;
            }
        int path[10];
        int* len;
        double ln = shortestPath2(g, s, t, path, &len);
        printf("%.0f\n", ln);
        for(i = len; i >= 0; i--)
            printf("%d\t", path[i]);
        printf("\n");

    }
int main(){

    int op;
    Graph g = createGraph();
    do
        {
            menu();
            do
                {
                    printf("Enter your option: ");
                    scanf("%d",&op);
                }
            while(op < 0 || op > 6);
            switch(op)
                {
                    case 1:
                        system("cls");
                        g = ReadData();
                        print_adjacent_matrix(g);
                        break;
                    case 2:
                        system("cls");
                        print_adjacent_list(g);
                        break;
                    case 3:
                        system("cls");
                        print_walking_castle(g);
                        indegree_list(g);
                        break;
                    case 4:
                        system("cls");
                        short_path(g);
                        break;
                    case 5:
                        system("cls");
                        short_path2(g);
                        break;
                }
        }
    while(op != 6);
}