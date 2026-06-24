#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

// ── Nodo del arbol de Huffman ─────────────────────────────
struct Nodo {
    char  simbolo;      // '\0' en nodos internos
    int   frecuencia;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(char s, int f)
        : simbolo(s), frecuencia(f),
          izquierda(nullptr), derecha(nullptr) {}
};

// Comparador min-heap: menor frecuencia = mayor prioridad
struct Comparar {
    bool operator()(const Nodo* a, const Nodo* b) const {
        return a->frecuencia > b->frecuencia;
    }
};

// ─────────────────────────────────────────────────────────
// 1. Calcular frecuencias de cada caracter en el texto
// ─────────────────────────────────────────────────────────
inline std::unordered_map<char, int>
calcularFrecuencias(const std::string& texto) {
    std::unordered_map<char, int> freq;
    for (char c : texto) freq[c]++;
    return freq;
}

// ─────────────────────────────────────────────────────────
// 2. Construir el arbol de Huffman usando un min-heap
//    Se combinan los dos nodos de menor frecuencia en cada
//    iteracion hasta tener un unico nodo raiz.
// ─────────────────────────────────────────────────────────
inline Nodo*
construirArbol(const std::unordered_map<char, int>& frecuencias) {
    std::priority_queue<Nodo*, std::vector<Nodo*>, Comparar> heap;

    for (auto& [simbolo, freq] : frecuencias)
        heap.push(new Nodo(simbolo, freq));

    while (heap.size() > 1) {
        Nodo* izq = heap.top(); heap.pop();
        Nodo* der = heap.top(); heap.pop();

        Nodo* padre = new Nodo('\0', izq->frecuencia + der->frecuencia);
        padre->izquierda = izq;
        padre->derecha   = der;
        heap.push(padre);
    }
    return heap.top();   // raiz del arbol
}

// ─────────────────────────────────────────────────────────
// 3. Generar tabla de codigos recorriendo el arbol
//    Izquierda → '0'  |  Derecha → '1'
// ─────────────────────────────────────────────────────────
inline void
generarCodigos(Nodo* nodo,
               const std::string& codigoActual,
               std::unordered_map<char, std::string>& tabla) {
    if (!nodo) return;

    if (!nodo->izquierda && !nodo->derecha) {   // hoja
        tabla[nodo->simbolo] = codigoActual.empty() ? "0" : codigoActual;
        return;
    }
    generarCodigos(nodo->izquierda, codigoActual + "0", tabla);
    generarCodigos(nodo->derecha,   codigoActual + "1", tabla);
}

// ─────────────────────────────────────────────────────────
// 4. Codificar: texto → cadena de bits
// ─────────────────────────────────────────────────────────
inline std::string
codificar(const std::string& texto,
          const std::unordered_map<char, std::string>& tabla) {
    std::string bits;
    bits.reserve(texto.size() * 4);
    for (char c : texto) bits += tabla.at(c);
    return bits;
}

// ─────────────────────────────────────────────────────────
// 5. Decodificar: cadena de bits → texto original
//    Recorre el arbol bit a bit; al llegar a una hoja emite
//    el simbolo y vuelve a la raiz.
// ─────────────────────────────────────────────────────────
inline std::string
decodificar(const std::string& bits, Nodo* raiz) {
    std::string resultado;
    Nodo* actual = raiz;
    for (char bit : bits) {
        actual = (bit == '0') ? actual->izquierda : actual->derecha;
        if (!actual->izquierda && !actual->derecha) {
            resultado += actual->simbolo;
            actual = raiz;
        }
    }
    return resultado;
}

// ─────────────────────────────────────────────────────────
// Liberar memoria del arbol (post-orden)
// ─────────────────────────────────────────────────────────
inline void liberarArbol(Nodo* nodo) {
    if (!nodo) return;
    liberarArbol(nodo->izquierda);
    liberarArbol(nodo->derecha);
    delete nodo;
}

// ─────────────────────────────────────────────────────────
// Mostrar tabla de codigos ordenada por frecuencia
// ─────────────────────────────────────────────────────────
inline void
mostrarTabla(const std::unordered_map<char, std::string>& tabla,
             const std::unordered_map<char, int>& frecuencias) {

    std::vector<std::pair<char,int>> ordenado(
        frecuencias.begin(), frecuencias.end());
    std::sort(ordenado.begin(), ordenado.end(),
              [](const auto& a, const auto& b){
                  return a.second > b.second; });

    const std::string sep = "+----------+-----------+------------------+";
    std::cout << "\n" << sep << "\n";
    std::cout << "| Simbolo  | Frecuenc. | Codigo Huffman   |\n";
    std::cout << sep << "\n";

    for (auto& [simbolo, freq] : ordenado) {
        std::string s = (simbolo == ' ') ? "ESPACIO" : std::string(1, simbolo);
        std::cout << "| " << std::left  << std::setw(8) << s
                  << " | " << std::right << std::setw(9) << freq
                  << " | " << std::left  << std::setw(16) << tabla.at(simbolo)
                  << " |\n";
    }
    std::cout << sep << "\n";
}