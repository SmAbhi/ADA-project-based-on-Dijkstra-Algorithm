#include <stdio.h>
#include <limits.h>

#define MAX 100 // Maximum number of vertices
#define INF INT_MAX

typedef struct {
    int id;
    int capacity;
} Vehicle;

typedef struct {
    int id;
    int demand;
    int ready_time;
    int due_time;
    int service_time;
} Customer;

// A utility function to find the vertex with the minimum distance value
int minDistance(int dist[], int sptSet[], int V) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance array
void printSolution(int dist[], int V) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t %d\n", i, dist[i]);
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using an adjacency matrix representation
void dijkstra(int graph[MAX][MAX], int src, int dist[], int V) {
    int sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest path tree

    // Initialize all distances as INFINITE and sptSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INF, sptSet[i] = 0;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, sptSet, V);

        // Mark the picked vertex as processed
        sptSet[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++)
            // Update dist[v] only if it is not in sptSet, there is an edge from u to v,
            // and total weight of path from src to v through u is smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

// Function to check if a vehicle can deliver to a customer considering capacity and time window constraints
int canDeliver(Vehicle vehicle, Customer customer, int current_time) {
    return (vehicle.capacity >= customer.demand && 
            current_time + customer.service_time <= customer.due_time);
}

int main() {
    int V; // Number of vertices
    printf("Enter the number of vertices: ");
    scanf("%d", &V);

    int graph[MAX][MAX];
    printf("Enter the adjacency matrix (enter 0 if there is no edge):\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            scanf("%d", &graph[i][j]);
            if (graph[i][j] == 0 && i != j) graph[i][j] = INF;
        }
    }

    int num_vehicles;
    printf("Enter the number of vehicles: ");
    scanf("%d", &num_vehicles);

    Vehicle vehicles[num_vehicles];
    for (int i = 0; i < num_vehicles; i++) {
        vehicles[i].id = i;
        printf("Enter the capacity of vehicle %d: ", i);
        scanf("%d", &vehicles[i].capacity);
    }

    int num_customers;
    printf("Enter the number of customers: ");
    scanf("%d", &num_customers);

    Customer customers[num_customers];
    for (int i = 0; i < num_customers; i++) {
        customers[i].id = i;
        printf("Enter demand, ready time, due time, and service time for customer %d: ", i);
        scanf("%d %d %d %d", &customers[i].demand, &customers[i].ready_time, &customers[i].due_time, &customers[i].service_time);
    }

    int src;
    printf("Enter the source vertex (depot): ");
    scanf("%d", &src);

    for (int v = 0; v < num_vehicles; v++) {
        int dist[MAX];
        dijkstra(graph, src, dist, V);

        int current_time = 0;
        printf("Route for vehicle %d:\n", vehicles[v].id);
        for (int c = 0; c < num_customers; c++) {
            if (canDeliver(vehicles[v], customers[c], current_time)) {
                printf("Customer %d: Distance %d, Demand %d\n", customers[c].id, dist[customers[c].id], customers[c].demand);
                current_time += customers[c].service_time;
                vehicles[v].capacity -= customers[c].demand;
            }
        }
    }

    return 0;
}