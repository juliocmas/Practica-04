#include <bits/stdc++.h>
using namespace std;

void minimumDistance(int distance[], bool visited[], int size, int& min, int& min_index) {
  min = INT_MAX;
  min_index = -1;
  for (int i = 0; i < size; i++) {
    if (!visited[i] && distance[i] < min) {
      min = distance[i];
      min_index = i;
    }
  }
}

void printParentPath(int parent[], int i) {
  if (parent[i] == -1) {
    cout << i + 1;
    return;
  }
  printParentPath(parent, parent[i]);
  cout << " -> " << i + 1;
}

void grafo(int** graph, int size, int source, int destination) {
  int distance[size];
  bool visited[size];
  int parent[size];

  for (int i = 0; i < size; i++) {
    parent[i] = -1;
    distance[i] = INT_MAX;
    visited[i] = false;
  }

  distance[source] = 0;

  for (int i = 0; i < size - 1; i++) {
    int U, min, min_index;
    minimumDistance(distance, visited, size, min, min_index);
    U = min_index;
    visited[U] = true;

    for (int j = 0; j < size; j++) {
      if (!visited[j] && graph[U][j] && distance[U] != INT_MAX &&
          (distance[U] + graph[U][j] < distance[j])) {
        parent[j] = U;
        distance[j] = distance[U] + graph[U][j];
      }
    }
  }

  cout << "Vertex\t\tDistance\tPath" << endl;
  cout << source + 1 << " -> " << destination << "\t\t" << distance[destination - 1] << "\t\t" << source + 1 << " -> ";
  printParentPath(parent, destination - 1);
  cout << endl;
}

int main() {
  int size;
  cout << "Enter the number of nodes: ";
  cin >> size;

  int** graph = new int*[size];
  for (int i = 0; i < size; i++) {
    graph[i] = new int[size];
  }

  cout << "Enter the path to the adjacency matrix file: ";
  string filePath;
  cin >> filePath;

  ifstream inputFile(filePath);

  if (!inputFile) {
    cerr << "Failed to open the file." << endl;
    return 1;
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      inputFile >> graph[i][j];
    }
  }

  int source, destination;
  cout << "Enter the source node (1 to " << size << "): ";
  cin >> source;
  source--; // Convert to 0-based indexing

  cout << "Enter the destination node (1 to " << size << "): ";
  cin >> destination;
  destination--; // Convert to 0-based indexing

  grafo(graph, size, source, destination);

  for (int i = 0; i < size; i++) {
    delete[] graph[i];
  }
  delete[] graph;

  return 0;
}
