/*
 * ============================================================
 *  main.cpp  —  Demo de Codificacion de Huffman
 *  Tema: Historia y evolucion de la compresion de datos
 *
 *  Compilar:
 *    g++ -std=c++17 -o huffman main.cpp
 *  Ejecutar:
 *    ./huffman
 * ============================================================
 */

#include "huffman.h"

// Imprime una linea divisora de ancho fijo
static void divisor(int ancho = 48) {
    std::cout << std::string(ancho, '-') << "\n";
}

int main() {

    // ── Pedir texto al usuario ────────────────────────────
    std::string texto;
    divisor();
    std::cout << "  Codificacion de Huffman -- C++ Demo\n";
    divisor();
    std::cout << "Ingresa el texto a comprimir: ";
    std::getline(std::cin, texto);

    if (texto.empty()) {
        std::cerr << "[ERROR] El texto no puede estar vacio.\n";
        return 1;
    }

    std::cout << "Longitud       : " << texto.size() << " caracteres\n";

    // ── Paso 1: Frecuencias ───────────────────────────────
    auto frecuencias = calcularFrecuencias(texto);
    std::cout << "Simbolos unicos: " << frecuencias.size() << "\n";

    // ── Paso 2: Arbol de Huffman ──────────────────────────
    Nodo* raiz = construirArbol(frecuencias);

    // ── Paso 3: Tabla de codigos ──────────────────────────
    std::unordered_map<char, std::string> tabla;
    generarCodigos(raiz, "", tabla);

    mostrarTabla(tabla, frecuencias);

    // ── Paso 4: Codificacion ──────────────────────────────
    std::string bits = codificar(texto, tabla);
    std::cout << "\nTexto codificado (" << bits.size() << " bits):\n";
    std::cout << bits.substr(0, 64)
              << (bits.size() > 64 ? "..." : "") << "\n";

    // ── Paso 5: Decodificacion ────────────────────────────
    std::string recuperado = decodificar(bits, raiz);
    std::cout << "\nTexto recuperado: \"" << recuperado << "\"\n";
    std::cout << (texto == recuperado
        ? "[OK] Decodificacion correcta (sin perdida)\n"
        : "[ERROR] La decodificacion fallo\n");

    // ── Metricas de compresion ────────────────────────────
    int    bitsOriginales  = static_cast<int>(texto.size()) * 8;
    int    bitsComprimidos = static_cast<int>(bits.size());
    double tasa   = static_cast<double>(bitsOriginales) / bitsComprimidos;
    double ahorro = 100.0 * (1.0 - static_cast<double>(bitsComprimidos)
                                   / bitsOriginales);

    std::cout << "\n";
    divisor();
    std::cout << "  Metricas de Compresion\n";
    divisor();
    std::cout << "  Tamano original   : "
              << bitsOriginales  << " bits  ("
              << texto.size()    << " bytes x 8)\n";
    std::cout << "  Tamano comprimido : "
              << bitsComprimidos << " bits\n";
    std::cout << "  Tasa de compres.  : "
              << std::fixed << std::setprecision(2) << tasa << ":1\n";
    std::cout << "  Ahorro de espacio : "
              << ahorro << "%\n";
    divisor();

    // ── Limpieza ──────────────────────────────────────────
    liberarArbol(raiz);
    return 0;
}