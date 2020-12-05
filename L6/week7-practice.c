// Name:
// Phan Huy Thang


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"

typedef struct {
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int DAG(Graph graph);
void dropGraph(Graph graph);

int main(){
    Graph g = createGraph();
    addVertex(g, 0, "Node 0");
    addVertex(g, 1, "Node 1");
    addVertex(g, 2, "Node 2");
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
    if(node == NULL)
        jrb_insert_s(graph,vertices, id, new_jval_s(name));
    else
        strcpy(jval_s(node->val), name);
}

char *getVertex(Graph graph, int id){
    JRB node = jrb_find(graph.vertices, id);
    if(node == NULL)
        return NULL;
    else
        return (jval_s(node->val));
}


/*
void addVertex(Graph graph, int id, char* name){
    JRB node = jrb_find_int(g.vertices, id);
    if (node==NULL) // only add new vertex
        jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph graph, int id){
    JRB node = jrb_find_int(g.vertices, id);
    if (node==NULL)
        return NULL;
    else
        return jval_s(node->val);
}

// Make connection between v1 and v2
void addEdge(Graph graph, int v1, int v2){
    JRB node, tree;
    if (!hasEdge(graph, v1, v2)){
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else{
            tree = (JRB) jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}

// v1 is connected to v2?
int hasEdge(Graph graph, int v1, int v2){
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
        return 0;
    tree = (JRB) jval_v(node->val);
    if (jrb_find_int(tree, v2)==NULL)
        return 0;
    else
        return 1;
}

int indegree(Graph graph, int v, int* output){
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges){
        tree = (JRB) jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
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
*/
