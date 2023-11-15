#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits.h>

using namespace std;

// Función para encontrar la distancia mínima no visitada
int distanciaMinima(int distancias[], bool visitados[], int size) {
    int minimo = INT_MAX;
    int indiceMinimo = -1;
    
    for (int i = 0; i < size; i++) {
        if (!visitados[i] && distancias[i] <= minimo) {
            minimo = distancias[i];
            indiceMinimo = i;
        }
    }
    return indiceMinimo;
}

// Función para imprimir el camino desde el nodo fuente hasta el nodo actual
void imprimirCamino(int padres[], int i) {
    if (padres[i] == -1) {
        return;
    }
    imprimirCamino(padres, padres[i]);
    cout << i + 1 << " ";
}

// Implementación del algoritmo de Dijkstra
void dijkstra(vector<vector<int>>& router, int fuente) {
    int size = router.size();
    int* distancias = new int[size];
    bool* visitados = new bool[size];
    int* padres = new int[size];

    // Inicializar arreglos de distancias, visitados y padres
    for (int i = 0; i < size; i++) {
        padres[i] = -1;
        distancias[i] = INT_MAX;
        visitados[i] = false;
    }

    // La distancia desde la fuente a sí misma siempre es 0
    distancias[fuente] = 0;

    // Encontrar el camino más corto para todos los nodos
    for (int i = 0; i < size - 1; i++) {
        int u = distanciaMinima(distancias, visitados, size);
        visitados[u] = true;

        // Actualizar las distancias de los nodos adyacentes
        for (int v = 0; v < size; v++) {
            int distanciaActualizada = distancias[u] + router[u][v];
            if (!visitados[v] && router[u][v] && distanciaActualizada < distancias[v]) {
                padres[v] = u;
                distancias[v] = distanciaActualizada;
            }
        }
    }

    // Imprimir la distancia y el camino para cada nodo
    cout << "Desde el nodo " << fuente + 1 << " hacia todos los nodos:" << endl;
    for (int i = 0; i < size; i++) {
        if (i != fuente) {
            cout << fuente + 1 << "->" << i + 1 << "\tDistancia: " << distancias[i] << "\tCamino: " << fuente + 1 << " ";
            imprimirCamino(padres, i);
            cout << endl;
        }
    }

    // Liberar memoria
    delete[] distancias;
    delete[] visitados;
    delete[] padres;
}

int main() {
    // Solicitar al usuario que ingrese la ruta del archivo CSV
    cout << "Ingrese la ruta donde se encuentra la matriz de adyacencia en formato CSV: ";
    string rutaArchivo;
    getline(cin, rutaArchivo);

    // Abrir el archivo
    ifstream archivoEntrada(rutaArchivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << endl;
        return 1;  // Salir con código de error
    }

    // Leer el contenido del archivo en una cadena
    stringstream buffer;
    buffer << archivoEntrada.rdbuf();
    string contenidoArchivo = buffer.str();

    // Procesar la cadena para construir la matriz
    stringstream streamMatriz(contenidoArchivo);
    vector<vector<int>> router;

    string linea;
    while (getline(streamMatriz, linea)) {
        vector<int> fila;
        stringstream streamLinea(linea);
        int valor;
        char coma;

        while (streamLinea >> valor) {
            fila.push_back(valor);
            streamLinea >> coma;  // Leer la coma
        }

        router.push_back(fila);
    }

    // Cerrar el archivo
    archivoEntrada.close();

    // Obtener el tamaño de la matriz
    int size = router.size();

    // Verificar que la matriz sea cuadrada
    for (const auto& fila : router) {
        if (fila.size() != size) {
            cerr << "Error: La matriz no es cuadrada." << endl;
            return 1;  // Salir con código de error
        }
    }

    // Ejecutar el algoritmo de Dijkstra para cada nodo como fuente
    for (int fuente = 0; fuente < size; ++fuente) {
        dijkstra(router, fuente);
        cout << "-----------------------------------" << endl;
    }

    return 0;
}
