//Nguyen Thanh Trung
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"

#define INFINITIVE_VALUE 10000000

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

int main(){
    Graph g = createGraph();
    addVertex(g, 1, "V1");
    addVertex(g, 2, "V2");
    addVertex(g, 3, "V3");
    addVertex(g, 4, "V4");
    addVertex(g, 5, "V5");
    addVertex(g, 6, "V6");
    addVertex(g, 7, "V7");
    addVertex(g, 8, "V8");
    addEdge(g, 1, 2, 9);
    addEdge(g, 1, 6, 14);
    addEdge(g, 1, 7, 15);
    addEdge(g, 2, 3, 24);
    addEdge(g, 3, 5, 2);
    addEdge(g, 3, 8, 19);
    addEdge(g, 4, 3, 6);
    addEdge(g, 4, 8, 6);
    addEdge(g, 5, 4, 11);
    addEdge(g, 5, 8, 16);
    addEdge(g, 6, 3, 18);
    addEdge(g, 6, 5, 30);
    addEdge(g, 6, 7, 5);
    addEdge(g, 7, 5, 20);
    addEdge(g, 7, 8, 44);
    
    int s, t, path[100], length;
    s = 1;
    t = 8;
    double distance_s_t = shortestPath(g, s, t, path, &length);
    if (distance_s_t != INFINITIVE_VALUE){
        printf("Shortest distance from s --> t: %lf\n", distance_s_t);
        printf("Path: ");
        for (int i=length-1; i>=0; i--){
            printf("%d ", path[i]);
        }
        printf("\n");
    }else{
        printf("No Path from s --> t\n");
    }
}

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
    JRB node = jrb_find_int(graph.vertices, id);
    if (node==NULL) // only add new vertex
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
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
    int previous[1000], u, visit[1000];
    
    for (int i=0; i<1000; i++){
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
        printf("%d ", u);
        dll_delete_node(node);
        int output[100];
        int size = outdegree(graph,u,output);
        for(int i = 0; i < size; i++)
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
