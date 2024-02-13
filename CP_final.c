#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define V 100  // Maximum number of vertices
#define INF INT_MAX

// Structure to represent an edge with multiple attributes
struct Edge {
    int src, dest;
    int cost;    // Cost attribute
    int time;    // Time attribute
    int distance; // Distance attribute
};

// Function to print the solution (shortest paths from source)
void printPath(int parent[], int src, int dest) {
    if (src == dest) {
        printf("%d ", src);
        return;
    }
    printPath(parent, src, parent[dest]);
    printf("%d ", dest);
}

void printSolution(int dist[], int n, int src, int dest, int attribute, int parent[], struct Edge edges[], int E) {
    const char *attributeName = (attribute == 1) ? "Cost" : (attribute == 2) ? "Time" : "Distance";
    printf("Shortest Path from %d to %d based on %s:\n", src, dest, attributeName);
    printf("Vertex \t Weight\n");
    int currentVertex = dest;
    int totalWeight = 0; // Initialize totalWeight
    while (currentVertex != src) {
        int prevVertex = parent[currentVertex];
        int edgeWeight = 0;

        // Find the edgeWeight for the corresponding edge
        for (int e = 0; e < E; e++) {
            if (edges[e].src == prevVertex && edges[e].dest == currentVertex) {
                edgeWeight = (attribute == 1) ? edges[e].cost : (attribute == 2) ? edges[e].time : edges[e].distance;
                break;
            }
        }

        totalWeight += edgeWeight; // Add edgeWeight to totalWeight
        printf("%d \t %d\n", currentVertex, edgeWeight);
        currentVertex = prevVertex;
    }
    printf("%d \t %d\n", src, 0); // Display the source vertex

    // Display total weight
    printf("Total %s: %d\n", attributeName, totalWeight);
}

// Function to find the vertex with the minimum distance value
int minDistance(int dist[], bool sptSet[], int n) {
    int min = INF, min_index;

    for (int v = 0; v < n; v++) {
        if (!sptSet[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Dijkstra's algorithm using a binary heap-based priority queue
void dijkstra(struct Edge edges[], int E, int src, int dest, int n, int attribute) {
    int dist[V]; // The output array. dist[i] will hold the shortest distance from src to i.
    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in the shortest path tree

    // Initialize all distances as INFINITE and sptSet[] as false
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        sptSet[i] = false;
    }

    int parent[n];
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    // Distance of the source vertex from itself is always 0
    dist[src] = 0;

    // Find the shortest path for all vertices
    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, sptSet, n);
        sptSet[u] = true;

        // Update dist value of adjacent vertices
        for (int e = 0; e < E; e++) {
            if (!sptSet[edges[e].dest] && edges[e].src == u) {
                int edgeWeight = (attribute == 1) ? edges[e].cost : (attribute == 2) ? edges[e].time : edges[e].distance;

                if (dist[u] != INF && edgeWeight < dist[edges[e].dest]) {
                    dist[edges[e].dest] = edgeWeight;
                    parent[edges[e].dest] = u;
                }
            }
        }
    }

    // Print the constructed distance array
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
