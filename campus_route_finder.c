/*
 * PROJECT 1: Campus Route Finder Using Graphs
 * Language: C
 * Concepts: adjacency list, BFS, DFS, queue, path reconstruction, reachability.
 *
 * Compile: gcc campus_route_finder.c -o campus_route_finder
 * Run:     ./campus_route_finder
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V 8
#define NAME_LEN 30

const char *locations[V] = {
    "Main Gate", "Library", "Block 1", "Block 2",
    "Cafeteria", "Auditorium", "Parking", "Lab"
};

typedef struct Node {
    int vertex;
    struct Node *next;
} Node;

Node *adj[V];

void addEdge(int u, int v) {
    Node *a = (Node *)malloc(sizeof(Node));
    Node *b = (Node *)malloc(sizeof(Node));
    if (!a || !b) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    a->vertex = v; a->next = adj[u]; adj[u] = a;
    b->vertex = u; b->next = adj[v]; adj[v] = b;
}

void createGraph(void) {
    int i;
    for (i = 0; i < V; i++) adj[i] = NULL;

    /* Undirected campus paths. Lab is intentionally disconnected for testing. */
    addEdge(0, 1); /* Main Gate - Library */
    addEdge(0, 2); /* Main Gate - Block 1 */
    addEdge(1, 3); /* Library - Block 2 */
    addEdge(1, 4); /* Library - Cafeteria */
    addEdge(2, 3); /* Block 1 - Block 2 */
    addEdge(2, 6); /* Block 1 - Parking */
    addEdge(3, 5); /* Block 2 - Auditorium */
    addEdge(4, 5); /* Cafeteria - Auditorium */
}

void displayGraph(void) {
    int i;
    Node *p;
    printf("\nCAMPUS GRAPH (Adjacency List)\n");
    for (i = 0; i < V; i++) {
        printf("%d. %-12s -> ", i, locations[i]);
        for (p = adj[i]; p; p = p->next)
            printf("%s%s", locations[p->vertex], p->next ? " -> " : "");
        printf("\n");
    }
}

void bfsRoute(int source, int destination) {
    int visited[V] = {0}, parent[V], queue[V];
    int front = 0, rear = 0, i, path[V], count = 0;
    Node *p;

    for (i = 0; i < V; i++) parent[i] = -1;
    visited[source] = 1;
    queue[rear++] = source;

    while (front < rear) {
        int u = queue[front++];
        if (u == destination) break;
        for (p = adj[u]; p; p = p->next) {
            int w = p->vertex;
            if (!visited[w]) {
                visited[w] = 1;
                parent[w] = u;
                queue[rear++] = w;
            }
        }
    }

    if (!visited[destination]) {
        printf("No route exists from %s to %s.\n",
               locations[source], locations[destination]);
        return;
    }

    for (i = destination; i != -1; i = parent[i])
        path[count++] = i;

    printf("Shortest route (minimum number of edges): ");
    for (i = count - 1; i >= 0; i--)
        printf("%s%s", locations[path[i]], i ? " -> " : "");
    printf("\nNumber of edges: %d\n", count - 1);
}

void dfsVisit(int u, int visited[]) {
    Node *p;
    visited[u] = 1;
    printf("%s", locations[u]);
    for (p = adj[u]; p; p = p->next) {
        if (!visited[p->vertex]) {
            printf(" -> ");
            dfsVisit(p->vertex, visited);
        }
    }
}

void showReachable(int source) {
    int visited[V] = {0}, i;
    printf("DFS traversal from %s: ", locations[source]);
    dfsVisit(source, visited);
    printf("\nReachable locations: ");
    for (i = 0; i < V; i++)
        if (visited[i]) printf("%s; ", locations[i]);
    printf("\nDisconnected from this starting point: ");
    for (i = 0; i < V; i++)
        if (!visited[i]) printf("%s; ", locations[i]);
    printf("\n");
}

int readLocation(const char *prompt) {
    int x, i;
    printf("%s\n", prompt);
    for (i = 0; i < V; i++) printf("%d. %s\n", i, locations[i]);
    printf("Enter location number: ");
    if (scanf("%d", &x) != 1 || x < 0 || x >= V) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Invalid location.\n");
        return -1;
    }
    return x;
}

void freeGraph(void) {
    int i;
    for (i = 0; i < V; i++) {
        Node *p = adj[i];
        while (p) {
            Node *temp = p;
            p = p->next;
            free(temp);
        }
    }
}

int main(void) {
    int choice, source, destination;
    createGraph();
    do {
        printf("\n===== CAMPUS ROUTE FINDER =====\n");
        printf("1. Display campus graph\n");
        printf("2. Find shortest route using BFS\n");
        printf("3. Display reachable locations using DFS\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            choice = 0;
        }

        switch (choice) {
            case 1:
                displayGraph();
                break;
            case 2:
                source = readLocation("Select source:");
                if (source == -1) break;
                destination = readLocation("Select destination:");
                if (destination != -1) bfsRoute(source, destination);
                break;
            case 3:
                source = readLocation("Select starting location:");
                if (source != -1) showReachable(source);
                break;
            case 4:
                printf("Thank you for using Campus Route Finder.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    freeGraph();
    return 0;
}
