#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int n = 0;
vector<pair<int,int>> aristas;
vector<vector<int>> adj;

void agregarNodos() {
    cout << "Cuantos nodos tiene el grafo? ";
    cin >> n;
    adj.assign(n, vector<int>());
    cout << "Nodos creados: 0 al " << n - 1 << "\n";
}

void agregarArista() {
    int u, v;
    cout << "Arista (nodo nodo): ";
    cin >> u >> v;
    if (u < 0 || u >= n || v < 0 || v >= n) {
        cout << "Error: nodos fuera de rango (0 al " << n - 1 << ")\n";
        return;
    }
    aristas.push_back({u, v});
    adj[u].push_back(v);
    adj[v].push_back(u);
    cout << "Arista " << u << " -- " << v << " agregada\n";
}

void mostrarGrafo() {
    cout << "\nNodos: 0 al " << n - 1 << "\n";
    cout << "Aristas:\n";
    if (aristas.empty()) {
        cout << "  (ninguna)\n";
        return;
    }
    for (auto [u, v] : aristas)
        cout << "  " << u << " -- " << v << "\n";
}

void buscarRuta() {
    int inicio, fin;
    cout << "Nodo de inicio: ";
    cin >> inicio;
    cout << "Nodo de destino: ";
    cin >> fin;

    if (inicio < 0 || inicio >= n || fin < 0 || fin >= n) {
        cout << "Error: nodos fuera de rango\n";
        return;
    }

    // BFS
    vector<int> dist(n, -1);
    vector<int> padre(n, -1);
    queue<int> cola;
    // 0 -- 1
    // 0 -- 2
    
    // 1 
    // 2 
    dist[inicio] = 0;
    cola.push(inicio);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (actual == fin) break;

        for (int vecino : adj[actual]) {
            if (dist[vecino] == -1) {
                dist[vecino] = dist[actual] + 1;
                padre[vecino] = actual;
                cola.push(vecino);
            }
        }
    }

    if (dist[fin] == -1) {
        cout << "No existe ruta entre " << inicio << " y " << fin << "\n";
        return;
    }

    // reconstruir camino
    vector<int> camino;
    for (int v = fin; v != -1; v = padre[v])
        camino.push_back(v);
    reverse(camino.begin(), camino.end());

    cout << "\nRuta mas corta (" << dist[fin] << " saltos): ";
    for (int i = 0; i < camino.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << camino[i];
    }
    cout << "\n";
}

void generarDOT(vector<int> camino = {}) {
    // construir set de aristas del camino para resaltarlas
    vector<pair<int,int>> aristasCamino;
    for (int i = 0; i + 1 < camino.size(); i++)
        aristasCamino.push_back({camino[i], camino[i+1]});

    auto enCamino = [&](int u, int v) {
        for (auto [a, b] : aristasCamino)
            if ((a==u && b==v) || (a==v && b==u)) return true;
        return false;
    };

    string archivo = "grafo.dot";
    ofstream f(archivo);

    f << "graph MiGrafo {\n";
    f << "    graph [bgcolor=\"#ffffff\" pad=\"0.5\"]\n";
    f << "    node [shape=circle style=filled fillcolor=\"#EEEDFE\" "
         "color=\"#534AB7\" fontcolor=\"#3C3489\" fontsize=16 penwidth=2]\n";
    f << "    edge [color=\"#7F77DD\" penwidth=2]\n\n";

    // nodos (resaltar los del camino)
    for (int i = 0; i < n; i++) {
        bool enRuta = false;
        for (int c : camino) if (c == i) { enRuta = true; break; }

        if (!camino.empty() && i == camino.front())
            f << "    " << i << " [fillcolor=\"#1D9E75\" fontcolor=\"#ffffff\" color=\"#0F6E56\"]\n";
        else if (!camino.empty() && i == camino.back())
            f << "    " << i << " [fillcolor=\"#D85A30\" fontcolor=\"#ffffff\" color=\"#993C1D\"]\n";
        else if (enRuta)
            f << "    " << i << " [fillcolor=\"#FAC775\" fontcolor=\"#412402\" color=\"#BA7517\"]\n";
        else
            f << "    " << i << ";\n";
    }

    f << "\n";

    // aristas (resaltar las del camino)
    for (auto [u, v] : aristas) {
        if (enCamino(u, v))
            f << "    " << u << " -- " << v << " [color=\"#D85A30\" penwidth=3.5]\n";
        else
            f << "    " << u << " -- " << v << "\n";
    }

    f << "}\n";
    f.close();

    cout << "\nGenerando imagen...\n";
    int resultado = system("dot -Tpng grafo.dot -o grafo.png");
    if (resultado == 0)
        cout << "Imagen guardada como grafo.png\n";
    else
        cout << "Error: asegurate de tener Graphviz instalado (apt install graphviz)\n";
}

void buscarYGraficar() {
    int inicio, fin;
    cout << "Nodo de inicio: ";
    cin >> inicio;
    cout << "Nodo de destino: ";
    cin >> fin;

    if (inicio < 0 || inicio >= n || fin < 0 || fin >= n) {
        cout << "Error: nodos fuera de rango\n";
        return;
    }

    vector<int> dist(n, -1);
    vector<int> padre(n, -1);
    queue<int> cola;

    dist[inicio] = 0;
    cola.push(inicio);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        if (actual == fin) break;
        for (int vecino : adj[actual]) {
            if (dist[vecino] == -1) {
                dist[vecino] = dist[actual] + 1;
                padre[vecino] = actual;
                cola.push(vecino);
            }
        }
    }

    if (dist[fin] == -1) {
        cout << "No existe ruta entre " << inicio << " y " << fin << "\n";
        generarDOT();
        return;
    }

    vector<int> camino;
    for (int v = fin; v != -1; v = padre[v])
        camino.push_back(v);
    reverse(camino.begin(), camino.end());

    cout << "\nRuta mas corta (" << dist[fin] << " saltos): ";
    for (int i = 0; i < (int)camino.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << camino[i];
    }
    cout << "\n";

    generarDOT(camino);
}

int main() {
    int opcion;

    cout << "=== Generador de grafos ===\n";
    agregarNodos();

    do {
        cout << "\n1. Agregar arista\n";
        cout << "2. Ver grafo\n";
        cout << "3. Graficar\n";
        cout << "4. Buscar ruta mas corta (BFS)\n";
        cout << "5. Buscar ruta y graficar\n";
        cout << "6. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: agregarArista();   break;
            case 2: mostrarGrafo();    break;
            case 3: generarDOT();      break;
            case 4: buscarRuta();      break;
            case 5: buscarYGraficar(); break;
            case 6: cout << "Hasta luego!\n"; break;
            default: cout << "Opcion invalida\n";
        }
    } while (opcion != 6);

    return 0;
}