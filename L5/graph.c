#include<stdio.h>
#include<stdlib.h>


typedef struct 
    {
        int* matrix;
        int sizemax;
    } Graph;

Graph createGraph(int sizemax)
    {
        Graph g;
        g.matrix = (int*)malloc(sizeof(int) * sizemax * sizemax);
        int i;
        for (i = 0; i < sizemax*sizemax; i++)
            g.matrix[i] = 0;
        g.sizemax = sizemax;
        return g;
    }

void addEdge(Graph g, int v1, int v2)
    {
        g.matrix[v1 * g.sizemax + v2] = 1;
        g.matrix[v1 + v2 * g.sizemax] = 1;
    }

int adjacent(Graph g, int v1, int v2)
    {
        if(g.matrix[v1 * g.sizemax + v2] == 1)
            return 1;
        return 0;
    }

int getAdjacentVertices(Graph g, int v, int* output)
    {
        int i, count = 0;
        for(i = 0; i < g.sizemax; i++)
            {
                if(g.matrix[v * g.sizemax + i] == 1)
                    output[count++] = i;
            }
        return count--;
    }
void dropGraph(Graph *g)
    {
        free(g);
    }

void print_graph(Graph g)
    {
        int i,j;
        for(i = 1; i < g.sizemax; i++)
            for(j = 0 ; j < i; j++)
                if(g.matrix[i * g.sizemax + j] == 1)
                    printf("( %d, %d )\n", j, i);
    }

int main()
    {
        Graph g = createGraph(10);
        int output[10];
        int count,i;
        addEdge(g, 1, 2);
        addEdge(g, 3, 2);
        addEdge(g, 4, 5);
        addEdge(g, 6, 1);

        print_graph(g);
        count = getAdjacentVertices(g, 1, output);
        printf("List of adjacent vertices: ");
        for(i = 0; i < count; i++)
            printf("%d  ", output[i]);

        dropGraph(&g);
    }


