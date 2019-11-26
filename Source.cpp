/* Bobitan Denisa - Ioana

   Task:

   Implement correctly and efficiently the Breadth-First Search. For graph representation, you should use adjacency lists.
 
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#pragma warning(disable : 4996)

Profiler profiler("demo");

struct adjListNode
{
	int dest;
	adjListNode *next;
};

adjListNode *createAdjListNode(int dest)
{
	adjListNode *node = (adjListNode*)malloc(sizeof(adjListNode));
	node->dest = dest;
	node->next = NULL;
	return node;
}

struct adjList
{
	adjListNode *head;
	int color; //0 means not visited, 1 means in process and 2 means already visited
	int d;
	adjListNode *p;
};

struct graph
{
	int v;
	adjList *list;
};

struct queue
{
	int count;
	adjListNode *first;
	adjListNode *last;
};

queue *createQ()
{
	queue *q = (queue*)malloc(sizeof(queue));
	q->count = 0;
	q->first = q->last = NULL;
	return q;
}

void enqueue(queue *q, int v) //insert at rear
{
	adjListNode *node = createAdjListNode(v);
	if (q->first == NULL) //if q empty, insert only node
	{
		q->first = q->last = node;
	}
	else //if q not empty, insert at rear
	{
		q->last->next = node;
		q->last = node;
	}
	q->count++;
}

int dequeue(queue *q) //delete first
{
	if (!q) //if q empty, show message
	{
		printf("Empty queue!");
		return NULL;
	}
	else //if q not empty, delete first
	{
		int aux = q->first->dest;
		q->first = q->first->next;
		q->count--;
		return aux;
	}
}

graph *createGraph(int v)
{
	graph *g = (graph*)malloc(sizeof(graph));
	g->v = v;
	g->list = (adjList*)malloc(v * sizeof(adjList));
	for (int i = 0; i < v; i++)
	{
		g->list[i].head = (adjListNode*)malloc(sizeof(adjListNode));
		g->list[i].color = 0; //white
		g->list[i].d = INT_MAX;
		g->list[i].p = NULL;
	}
	return g;
}

void insOneEdge(graph *g, int src, int dest) //insert the dest node in the adjList of the src node
{
	//directed graph
	adjListNode *node = createAdjListNode(dest);
	adjListNode *aux = g->list[src - 1].head;
	while (aux->next)
	{
		aux = aux->next;
	}
	aux->next = node;
}

void insertEdge(graph *g, int src, int dest) //for undirected graphs
{
	insOneEdge(g, src, dest);
	insOneEdge(g, dest, src);
}

graph *generateGraph()
{
	int v = 7;
	graph *g = createGraph(v);

	for (int i = 0; i < v; i++)
	{
		g->list[i].head = createAdjListNode(i + 1); //5 nodes with ids from 1 to 5
	}
	
	insertEdge(g, 1, 2);
	insertEdge(g, 1, 3);
	insertEdge(g, 2, 3);
	insertEdge(g, 2, 5);
	insertEdge(g, 4, 5);
	insertEdge(g, 6, 7);

	return g;
}

void displayAdjList(graph *g)
{
	for (int i = 0; i < g->v; i++)
	{
		printf("%d: ", g->list[i].head->dest);
		if (g->list[i].head->next)
		{
			adjListNode *node = g->list[i].head->next;
			while (node)
			{
				printf("%d ", node->dest);
				node = node->next;
			}
		}
		printf("\n");
	}
	printf("\n");
}

int op;

int *parent; //parent vector to keep all parens of all nodes in the bfs traversed tree

void bfsUtil(graph *g, int s) //used in the demo
{
	printf("BFS traversal: ");

	queue *q = createQ();

	g->list[s - 1].color = 1; //gray
	g->list[s - 1].d = 0;
	//parent is already set to NULL in generateGraph() 

	enqueue(q, s); //enqueue source node
	while (q->first != NULL) //queue not empty
	{
		adjListNode *node = createAdjListNode(dequeue(q)); //dequeue one node

		printf("%d ", node->dest);

		adjListNode *aux = g->list[node->dest - 1].head->next;
		while (aux) //for each aux adjacent to node
		{
			if (g->list[aux->dest - 1].color == 0) //if not visited
			{
				g->list[aux->dest - 1].color = 1; //gray
				g->list[aux->dest - 1].d = g->list[node->dest - 1].d + 1;
				g->list[aux->dest - 1].p = g->list[node->dest - 1].head;
				parent[aux->dest - 1] = g->list[aux->dest - 1].p->dest;
				enqueue(q, aux->dest); //enqueue node
			}
			aux = aux->next; //move to the next node
		}
		g->list[node->dest - 1].color = 2; //black
	}
	printf("\n");
}

void bfs(graph *g, int s) //this makes sure that the alg works in case of more than one connected graph
{
	parent = (int*)calloc(g->v, sizeof(int));
	bfsUtil(g, s);
	for (int i = 0; i < g->v; i++) //for each node, we check the color
	{
		if (g->list[i].color == 0)
		{
			bfsUtil(g, i + 1);
		}
	}
}

void bfsPerformanceUtil(graph *g, int start) //the same function, but without printing the traversal and it counts the operations
{
	adjListNode *s = createAdjListNode(g->list[start - 1].head->dest); //start from given node
	op = 0; //init number of operations to 0
	queue *q = createQ(); //create an empty queue
	
	g->list[s->dest - 1].color = 1; //gray
	g->list[s->dest - 1].d = 0;
	g->list[s->dest - 1].p = NULL;
	op += 3;

	enqueue(q, s->dest); //enqueue source node
	while (q->first != NULL) //queue not empty
	{
		adjListNode *node = createAdjListNode(dequeue(q)); //dequeue one node
		adjListNode *aux = g->list[node->dest - 1].head->next;
		while (aux) //for each aux adjacent to node
		{
			op++;
			if (g->list[aux->dest - 1].color == 0) //if not visited
			{
				g->list[aux->dest - 1].color = 1; //gray
				g->list[aux->dest - 1].d = g->list[node->dest - 1].d + 1;
				g->list[aux->dest - 1].p = g->list[node->dest - 1].head;
				op += 3;
				enqueue(q, aux->dest); //enqueue node
			}
			aux = aux->next; //move to the next node
		}
		g->list[node->dest - 1].color = 2; //black
		op++;
	}
}

void bfsPerformance(graph *g, int s)
{
	bfsPerformanceUtil(g, s);
	for (int i = 1; i <= g->v; i++) //for each node, we check the color
	{
		if (g->list[i].color == 0)
		{
			bfsPerformanceUtil(g, i);
		}
	}
}

void findElements(int n, int p)
{
	if (p == 0)
	{
		printf("root ");
	}
	else
	{
		printf("\nnode %d has children: ", p);
	}
	for (int i = 0; i < n; i++)
	{
		if (p == 0)
		{
			break;
		}
		if (parent[i] == p)
		{
			printf("%d ", i + 1);
		}
	}
}

void printTree(int n)
{
	printf("\nTree:\n");
	for (int i = 0; i <= n; i++) //for each element possible in parent vector
	{
		findElements(n, i);
	}
	printf("\n");
}

void demo()
{
	graph *g = generateGraph();
	displayAdjList(g);
	bfs(g, g->list[0].head->dest);
	printTree(g->v);
	free(g);
}

boolean isUnique(int *ed, graph *g)
{
	adjListNode *aux = g->list[ed[0] - 1].head->next;
	while (aux != NULL)
	{
		if ((ed[0] == g->list[ed[0] - 1].head->dest && ed[1] == aux->dest) || (ed[1] == g->list[ed[0] - 1].head->dest && ed[0] == aux->dest))
		{
			return false;
		}
		aux = aux->next;
	}
	return true;
}

graph *generateRandGraph(int v, int e)
{
	graph *g = createGraph(v);
	for (int i = 0; i < v; i++)
	{
		g->list[i].head = createAdjListNode(i + 1);
	}
	int *ed = (int*)malloc(2 * sizeof(int));
	int k = 0;
	while (k < e)
	{
		FillRandomArray(ed, 2, 1, v, true, 0); //generate pairs of nodes which represent an edge
		if (isUnique(ed, g)) //if edge is unique, insert it in the graph
		{
			insertEdge(g, ed[0], ed[1]);
			k++;
		}
	}
	return g;
}

void demo1()
{
	graph *g = generateRandGraph(5, 7);
	displayAdjList(g);
	bfs(g, g->list[0].head->dest);
	free(g);
}

void report()
{
	int v;
	int e;
	graph *g;

	v = 100;
	for (e = 1000; e <= 4500; e += 100)
	{
		g = generateRandGraph(v, e);
		op = 0;
		bfsPerformance(g, g->list[0].head->dest);
		profiler.countOperation("V_fixed_E_variable", e, op);
	}

	e = 4500;
	for (v = 100; v <= 200; v += 10)
	{
		g = generateRandGraph(v, e);
		op = 0;
		bfsPerformance(g, g->list[0].head->dest);
		profiler.countOperation("V_variable_E_fixed", v, op);
	}

	profiler.showReport();
}

int main()
{
	int x = 1;
	while (x != 0)
	{
		printf("Enter option: ");
		scanf("%d", &x);
		if (x == 1)
		{
			demo(); //hard-coded graph
		}
		else if (x == 2)
		{
			demo1(); //random generated small graph
		}
		else if (x == 3)
		{
			report();
		}
		printf("\n");
	}
	return 0;
}