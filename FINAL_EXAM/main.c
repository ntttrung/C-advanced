#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

#define INFINITIVE_VALUE 10000000

int countCycle = 0;
int countRoute = 0;
int total_vertex = 0;
int matrix1[10][10];
int matrix2[10][10];

typedef struct {
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, int weight);
int getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int shortestPath(Graph graph, int s, int t, int *path, int *length);
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
void addEdge(Graph graph, int v1, int v2,int weight){
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

int getEdgeValue(Graph graph, int v1, int v2){
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

int shortestPath(Graph graph, int s, int t, int *path, int *length){
    // Khoi tao cac distance = 0
    int distance[1000], min;
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
        printf("3. Shortest path\n");
        printf("4. Walking path\n");
        printf("5. Find all path\n");
        printf("6. Exit\n");
    }
void ReadData(Graph g1, Graph g2)
    {
        FILE* fin;
        char str[5];
        int i, j, a, b, c, d, k, n;
        fin = fopen("dothi.txt", "r");
        fscanf(fin, "%d\t%d\n", &total_vertex, &k);
        for(j = 1; j <= total_vertex; j++)
            {
                itoa(j, str, 10);
                addVertex(g1, j, str);
                addVertex(g2, j, str);
            }
        while(fscanf(fin, "%d\t%d\t%d\t%d\n", &a, &b, &c, &d) != EOF)
            {
                addEdge(g1, a, b, c);
                addEdge(g1, b, a, c);
                addEdge(g2, a, b, d);
                addEdge(g2, b, a, d);
            }
    }
void print_adjacent_matrix(Graph g, int matrix[10][10])
    {
        int i, j;
        for(i = 1; i <= total_vertex; i++)
            for(j = 1; j <= total_vertex; j++)
                matrix[i][j] = 0;
        for(i = 1; i <= total_vertex; i++)
            for(j = 1; j <= total_vertex; j++)
                {
                    if(i == j)
                        matrix[i][j] = 0;
                    if(hasEdge(g, i, j) == 1)
                        {
                            matrix[i][j] = getEdgeValue(g, i, j);
                            matrix[j][i] = getEdgeValue(g, i, j);
                        }
                }
        for(i = 1; i <= total_vertex; i++)
            {
                for(j = 1; j <= total_vertex; j++)
                    printf("%3d  ",matrix[i][j]);
                printf("\n");
            }
        
    }

void print_adjacent_list(int matrix[10][10])
    {
        int i, j;
        for(i = 1; i <= total_vertex; i++)
            {
                printf("Castle %d: ",i);
                for(j = total_vertex; j >= 1; j--)
                    {
                        if(matrix[i][j] != 0 && i != j)
                            printf("%d\t",j);
                    }
                printf("\n");
            }
    }

int shortestPath1(Graph graph, Graph g, int s, int t, int *path, int *length){
    // Khoi tao cac distance = 0
    int distance[1000], min;
    int time[1000];
    int previous[1000], u, visit[1000], i;
    
    for (i=0; i<1000; i++){
        distance[i] = INFINITIVE_VALUE;
        time[i] = INFINITIVE_VALUE;
        visit[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    time[s] = 0;
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
            // if(getEdgeValue(graph, u, output[i]) >= 50)
                {
                     if(visit[output[i]] == 0)
                    {
                        visit[output[i]] = 1;
                        dll_append(queue, new_jval_i(output[i]));
                    }
                    if(((getEdgeValue(graph, u, output[i]) + distance[u]) < distance[output[i]]) || 
                    (((getEdgeValue(graph, u, output[i]) + distance[u]) == distance[output[i]]) && (getEdgeValue(g, u, output[i])) + time[u] < time[output[i]]))
                    {
                        distance[output[i]] = distance[u] + getEdgeValue(graph, u , output[i]);
                        time[output[i]] = time[u] + getEdgeValue(g, u, output[i]);
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
        if(ln > 100) break;
    }
    *length = ln;
    return distance[t];
}

void short_path(Graph g1, Graph g2)
    {
        int s, t, i;
        printf("Enter start: ");
        scanf("%d", &s);
        if(jrb_find_int(g1.edges, s) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
        printf("Enter terminate: ");
        scanf("%d", &t);
        if(jrb_find_int(g1.edges, t) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
        int path[101];
        int* len;
        int ln = shortestPath1(g1, g2, s, t, path, &len);
        if(ln == INFINITIVE_VALUE)
            printf("ROUTE NOT FOUND\n");
        else 
            {
                printf("\n");
                printf("Total distance: %d\n", ln);
                printf("Path:  ");
                for(i = len; i >= 0; i--)
                    printf("%d\t", path[i]);
                printf("\n");
            }

    }

void walking_path(Graph g1)
    {
        int s, t, i;
        printf("Enter start: ");
        scanf("%d", &s);
        if(jrb_find_int(g1.edges, s) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
        printf("Enter terminate: ");
        scanf("%d", &t);
        if(jrb_find_int(g1.edges, t) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
        int path[101];
        int* len;
        int ln = shortestPath2(g1, s, t, path, &len);
        if(ln == INFINITIVE_VALUE)
            printf("ROUTE NOT FOUND\n");
        else 
            {
                printf("\n");
                printf("Total time: %d\n", ln);
                printf("Path:  ");
                for(i = len; i >= 0; i--)
                    printf("%d\t", path[i]);
                printf("\n");
            }

    }

int shortestPath2(Graph graph, int s, int t, int *path, int *length){
    // Khoi tao cac distance = 0
    int distance[1000], min;
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
                    if(((getEdgeValue(graph, u, output[i]) + distance[u]) < distance[output[i]]))
                    {
                        distance[output[i]] = distance[u] + getEdgeValue(graph, u , output[i]);
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
        if(ln > 100) break;
    }
    *length = ln;
    return distance[t];
}

void printAllPaths(Graph distanceGraph, Graph timeGraph) {
    int s;
    int t;
    
    countRoute = 0;
    printf("Enter start: ");
    scanf("%d", &s);
    if(jrb_find_int(distanceGraph.edges, s) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
    printf("Enter terminate: ");
    scanf("%d", &t);
    if(jrb_find_int(distanceGraph.edges, t) == NULL)
            {
                printf("ROUTE NOT FOUND\n");
                return -1;
            }
    int visited[total_vertex + 1];
    int i;
    for (i = 0; i < total_vertex + 1; i++) {
        visited[i] = 0;
    }
    Dllist currentpath;
    currentpath = new_dllist();
    dll_append(currentpath, new_jval_i(s));
    dfs(distanceGraph, timeGraph, s, t, visited, currentpath);
    if (countRoute == 0) {
        printf("ROUTE NOT FOUND.\n");
    }
}

int dfs(Graph distanceGraph, Graph timeGraph, int s, int t, int visited[], Dllist currentpath) {
    int distance, time;
    int u;
    Dllist ptr, node;
    if (s == t) {
        distance = 0;
        time = 0;
        printf("_______________________________________\n\n");
        printf("Path %d:   ", countRoute + 1);
        dll_traverse(ptr, currentpath){
            u = jval_i(ptr->val);
            printf("%-3d", u);
        }
        printf("\n");
        dll_traverse(ptr, currentpath){
            u = jval_i(ptr->val);
            if (u != t) {
                distance += getEdgeValue(distanceGraph, u, jval_i(dll_next(ptr)->val));
                time += getEdgeValue(timeGraph, u, jval_i(dll_next(ptr)->val));
            }
        }
        printf("Total Distance: %d\n", distance);
        printf("Total Time: %d\n", time);
        countRoute++;
        return 1;
    }
    visited[s] = 1;
    int size, adj[100];
    size = outdegree(distanceGraph, s, adj);
    int i;
    for (i = 0; i < size; i++) {
        if (!visited[adj[i]]) {
            dll_append(currentpath, new_jval_i(adj[i]));
            dfs(distanceGraph, timeGraph, adj[i], t, visited, currentpath);
            dll_traverse(ptr, currentpath){
                u = jval_i(ptr->val);
                if (u == adj[i]) {
                    node = ptr;
                }
            }
            dll_delete_node(node);
        }
    }
    visited[s] = 0;
}

int main(){

    int op;
    Graph g1 = createGraph();
    Graph g2 = createGraph();
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
                        ReadData(g1, g2);
                        printf("---------------DISTANCE-----------------\n");
                        printf("\n");
                        print_adjacent_matrix(g1, matrix1);
                        printf("\n");
                        printf("-----------------TIME--------------------\n");
                        printf("\n");
                        print_adjacent_matrix(g2, matrix2);
                         printf("\n\n");
                        break;
                    case 2:
                        system("cls");
                        print_adjacent_list(matrix1);
                        break;
                    case 3:
                        system("cls");
                        short_path(g1, g2);
                        break;
                    case 4:
                        system("cls");
                        walking_path(g2);
                        break;
                    case 5:
                        system("cls");
                        printAllPaths(g1, g2);
                        break;
                }
        }
    while(op != 6);
}