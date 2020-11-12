#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "queue.h"

typedef JRB Graph;

Graph createGraph(){
    Graph g = make_jrb();
    return g;
}

void addEdge(Graph g, int v1, int v2){
    Graph tree;
    Graph node = jrb_find_int(g, v1);
    if (node == NULL) {
        tree = make_jrb(); 
        jrb_insert_int(g, v1, new_jval_v(tree));
    } else {
        tree = (JRB) jval_v(node->val); 
    }
    jrb_insert_int(tree, v2, new_jval_i(1));
/**/
    Graph node2 = jrb_find_int(g, v2);
    Graph tree2;
    if (node2 == NULL) {
        tree2 = make_jrb(); 
        jrb_insert_int(g, v2, new_jval_v(tree2));
    } else {
        tree2 = (JRB) jval_v(node2->val); 
    }
    jrb_insert_int(tree2, v1, new_jval_i(1));
}

int getAdjacentVertices(Graph g, int v, int* output){
    Graph node = jrb_find_int(g, v);
    Graph tmp;
    if (node == NULL) return -1;
    else {
        Graph tree = (JRB) jval_v(node->val);
        int total = 0;
        jrb_traverse(tmp, tree){
            output[total++] = jval_i(tmp->key);
        }
        return total; //number of adjacents of vertex
    }
}

void print_graph(Graph g){
  Graph i;
  int size;
  jrb_traverse(i, g){
    printf("Key %d: [", jval_i(i->key));
    int output[10];
    size = getAdjacentVertices(g, jval_i(i->key), output);
    int i;
    for ( i = 0; i < size; i++){
        printf("%d ", output[i]);
    }
    printf("]\n");
  }
}

// void dropGraph(Graph* g){
//     free(g);
// }
int size_of_graph(Graph g)
    {

        
    }

void BFS( Graph g, int start, int stop, void(*func)(int))
    {
        Graph node;
        int *visit;
        jrb_traverse(node, g)
            {
                count
            }
    }

int main(){
    Graph g = createGraph();

    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 7, 8);
    addEdge(g, 7, 1);
    addEdge(g, 3, 5);
    addEdge(g, 2, 5);

    print_graph(g);

    // int output[10];
    // int v = 5;
    // int res = getAdjacentVertices(g, v, output);
    // if (res == 0) printf("No adjacent.");
    // else {
    //     printf("List of adjacents of %d: ", v);
    //     for(int i = 0; i < res; i++)
    //     printf("%d ", output[i]);
    // }
    // dropGraph(&g);
}