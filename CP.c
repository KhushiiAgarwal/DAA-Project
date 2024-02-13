#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#define V 100  // Maximum number of vertices
#define INF INT_MAX

// Structure to represent an edge
struct Edge {
    int src;
    int dest;
    int cost;
    int time;
    int distance;
};

// Structure to represent a min heap node
struct MinHeapNode {
    int vertex;
    int distance;
};

struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode *newMinHeapNode(int vertex, int distance) {
    struct MinHeapNode *minHeapNode = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    minHeapNode->vertex = vertex;
    minHeapNode->distance = distance;
    return minHeapNode;
}

struct MinHeap *createMinHeap(int capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap *minHeap) {
    return minHeap->size == 0;
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode *root = minHeap->array[0];

    struct MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap *minHeap, int vertex, int distance) {
    int i = minHeap->pos[vertex];

    minHeap->array[i]->distance = distance;

    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance) {
        minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int vertex) {
    if (minHeap->pos[vertex] < minHeap->size)
        return true;
    return false;
}

void printPath(int parent[], int src, int dest, int attribute, struct Edge edges[], int E) {
    if (src == dest) {
        printf("%d ", src);
        return;
    }
    printPath(parent, src, parent[dest], attribute, edges, E);
    printf("%d ", dest);
}

void printSolution(int dist[], int n, int src, int dest, int attribute, int parent[], struct Edge edges[], int E) {
    const char *attributeName = (attribute == 1) ? "Cost" : (attribute == 2) ? "Time" : "Distance";
    printf("Shortest Path from %d to %d based on %s:\n", src, dest, attributeName);
    

    int cumulativeWeight = 0;

    printPath(parent, src, dest, attribute, edges, E);
    printf("\n");

    printf("Total %s: %d\n", attributeName, dist[dest]);
}

void dijkstra(struct Edge edges[], int E, int src, int dest, int n, int attribute) {
    int dist[n];
    int parent[n];
    struct MinHeap *minHeap = createMinHeap(n);

    for (int v = 0; v < n; ++v) {
        dist[v] = INF;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = n;

    while (!isEmpty(minHeap)) {
        struct MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex;

        for (int e = 0; e < E; ++e) {
            if (edges[e].src == u) {
                int v = edges[e].dest;
                int weight = (attribute == 1) ? edges[e].cost : (attribute == 2) ? edges[e].time : edges[e].distance;

                if (isInMinHeap(minHeap, v) && dist[u] != INF && weight + dist[u] < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    decreaseKey(minHeap, v, dist[v]);
                }
            }
        }
    }

    printSolution(dist, n, src, dest, attribute, parent, edges, E);
}

int main() {
    int n, E; // Number of vertices and edges
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &E);

    struct Edge edges[E];
    printf("Enter source, destination, cost, time, and distance of each edge:\n");
    for (int i = 0; i < E; i++) {
        int src, dest, cost, time, distance;
        scanf("%d %d %d %d %d", &src, &dest, &cost, &time, &distance);

        if (src < 0 || src >= n || dest < 0 || dest >= n) {
            printf("Invalid vertex indices. Please make sure they are in the range [0, %d].\n", n - 1);
            return 1;
        }

        edges[i].src = src;
        edges[i].dest = dest;
        edges[i].cost = cost;
        edges[i].time = time;
        edges[i].distance = distance;
    }

    int src, dest; // Source and destination vertices
    printf("Enter the source vertex: ");
    scanf("%d", &src);
    printf("Enter the destination vertex: ");
    scanf("%d", &dest);

    if (src < 0 || src >= n || dest < 0 || dest >= n) {
        printf("Invalid source or destination vertex indices. Please make sure they are in the range [0, %d].\n", n - 1);
        return 1;
    }

    int attribute;
    printf("Which factor do you want to consider?\n");
    printf("1. Cost\n");
    printf("2. Time\n");
    printf("3. Distance\n");
    printf("Enter the corresponding number: ");
    scanf("%d", &attribute);

    dijkstra(edges, E, src, dest, n, attribute);

    return 0;
}
