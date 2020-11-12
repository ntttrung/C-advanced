#include<stdio.h>
#include<stdlib.h>

#include "jrb.h"
#include "jval.h"


typedef JRB Graph;

Graph createGraph()
    {
        Graph g = make_jrb();
        return g;
    }

void addEdge(Graph g, int v1, int v2)
    {
        /// add v1
        Graph tree =  make_jrb();
        Graph node = jrb_find_int(g, v1);
        if( node == NULL)
            {
                // jrb_insert_int(g, v1, new_jval_v(tree));
                jrb_insert_int(tree, v2, new_jval_i(1));
                jrb_insert_int(g, v1, new_jval_v(tree));
            }
        else 
            {
                tree = (JRB) jval_v (node->val);
                jrb_insert_int(tree, v2, new_jval_i(1));
            }
        /// add v2
        tree =  make_jrb();
        node = jrb_find_int(g, v2);
        if( node == NULL)
            {
                // jrb_insert_int(g, v2, new_jval_v(tree));
                jrb_insert_int(tree, v1, new_jval_i(1));
                jrb_insert_int(g, v2, new_jval_v(tree));
            }
        else 
            {
                tree = (JRB) jval_v (node->val);
                jrb_insert_int(tree, v1, new_jval_i(1));
            }


    }

int adjacent(Graph g, int v1, int v2)
    {
        Graph node1, node2;
        node1 = jrb_find_int(g, v1);
        if(node1 != NULL)
            {
                node2 = jrb_find_int((JRB) jval_v(node1->val), v2);
                if(node2 != NULL)
                    return 1;
            }
        return 0;
    }

int getAdjacentVertices(Graph g, int v, int* output)
    {
        int i, total = 0;
        Graph node = jrb_find_int(g, v);
        Graph tree = (JRB) jval_v(node->val);
        jrb_traverse(node, tree)
            output[total++] = jval_i(node->key);
        return --total;
        
    }
void dropGraph(Graph g)
    {
        jrb_free_tree(g);
    }

void print_graph(Graph g)
    {
        Graph node;
        jrb_traverse(node, g);
            {
                if(node != NULL)
                    {
                        int output[11];
                        int res = getAdjacentVertices(g, jval_i(node->key), output);
                        printf("%d: ",jval_i(node->key));
                        int i;
                        for(i=0; i < res; i++)
                            {
                                printf("%d ", output[i]);
                            }
                        printf("\n");
                    }
            }
    }

int main()
    {
        Graph g = createGraph();
        addEdge(g, 1, 2);
        addEdge(g, 2, 3);
        addEdge(g, 5, 10);
        print_graph(g);
    }


