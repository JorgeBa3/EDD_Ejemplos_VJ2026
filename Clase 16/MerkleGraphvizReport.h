#ifndef MERKLE_GRAPHVIZ_REPORT_H
#define MERKLE_GRAPHVIZ_REPORT_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
#endif

#include "MerkleTree.h"

/*
 * ============================================================================
 *  MerkleGraphvizReport — Reporte visual del Árbol de Merkle (opción 6.4)
 * ============================================================================
 *
 *  Requisitos del enunciado (pág. 10, sección 10.4):
 *    - Hojas en la parte inferior con el hash truncado de cada certificado
 *      (primeros 8 caracteres)
 *    - Nodos intermedios mostrando el hash combinado (también truncado,
 *      para que el diagrama sea legible)
 *    - Raíz en la parte superior, resaltada en azul
 *    - Indicar cuántos certificados cubre el árbol
 *
 *  Genera un archivo .dot y lo compila automáticamente a .png usando:
 *      dot -Tpng archivo.dot -o archivo.png
 *
 *  NOTA SOBRE ORIENTACIÓN: Graphviz dibuja por defecto de arriba hacia
 *  abajo (rankdir=TB), con la raíz como primer nodo declarado. Como en
 *  Merkle "la raíz arriba, las hojas abajo" es justo ese orden natural,
 *  no se necesita invertir el rankdir.
 * ============================================================================
 */

class MerkleGraphvizReport {
private:
    static void asegurarCarpeta(const std::string& carpeta) {
        struct stat info;
        if (stat(carpeta.c_str(), &info) != 0) {
#ifdef _WIN32
            _mkdir(carpeta.c_str());
#else
            mkdir(carpeta.c_str(), 0755);
#endif
        }
    }

    // Trunca un hash a los primeros N caracteres (para legibilidad visual)
    static std::string truncar(const std::string& hash, size_t n = 8) {
        if (hash.size() <= n) return hash;
        return hash.substr(0, n) + "...";
    }

    // Genera un identificador único válido para Graphviz (sin caracteres
    // especiales) a partir del nivel y la posición del nodo.
    static std::string idNodo(size_t nivel, size_t posicion) {
        return "n" + std::to_string(nivel) + "_" + std::to_string(posicion);
    }

public:
    // ---------------------------------------------------------------
    // Genera el archivo .dot a partir del árbol y lo compila a .png.
    // Devuelve true si todo el proceso fue exitoso.
    // ---------------------------------------------------------------
    static bool generarReporte(const MerkleTree& merkle,
                                const std::string& carpetaSalida = "reportes",
                                const std::string& nombreArchivo = "merkle_tree") {

        const auto& levels = merkle.getLevels();

        if (levels.empty()) {
            std::cout << "[AVISO] El Arbol de Merkle esta vacio. No hay certificados que graficar.\n";
            return false;
        }

        asegurarCarpeta(carpetaSalida);

        std::string rutaDot = carpetaSalida + "/" + nombreArchivo + ".dot";
        std::string rutaPng = carpetaSalida + "/" + nombreArchivo + ".png";

        std::ofstream dot(rutaDot);
        if (!dot.is_open()) {
            std::cerr << "[ERROR] No se pudo crear el archivo: " << rutaDot << "\n";
            return false;
        }

        size_t numCertificadosReales = merkle.getNumCertificados();
        size_t numNiveles = levels.size();
        size_t indiceNivelRaiz = numNiveles - 1;
        bool hayDuplicacion = merkle.tieneDuplicacionEnHojas();

        dot << "digraph ArbolMerkle {\n";
        dot << "    rankdir=BT;\n"; // Top-to-Bottom: raiz arriba, hojas abajo
        dot << "    node [fontname=\"Helvetica\", fontsize=10];\n";
        dot << "    edge [color=\"#555555\"];\n";
        dot << "    labelloc=\"t\";\n";
        dot << "    label=\"Arbol de Merkle - EDD CoffeeTrack\\n"
            << "Certificados cubiertos: " << numCertificadosReales
            << " | Altura: " << numNiveles << " niveles\";\n";
        dot << "    fontname=\"Helvetica\";\n";
        dot << "    fontsize=14;\n\n";

        // -----------------------------------------------------------
        // Declarar todos los nodos, nivel por nivel.
        // Usamos "rank=same" implicito al declarar cada nivel dentro
        // de su propio subgraph, para que Graphviz los alinee en fila.
        // -----------------------------------------------------------
        for (size_t nivel = 0; nivel < numNiveles; ++nivel) {
            dot << "    { rank=same;\n";

            for (size_t pos = 0; pos < levels[nivel].size(); ++pos) {
                std::string id = idNodo(nivel, pos);
                std::string hashCompleto = levels[nivel][pos];
                std::string hashCorto = truncar(hashCompleto, 8);

                if (nivel == indiceNivelRaiz) {
                    // RAÍZ: resaltada en azul, en la parte superior
                    dot << "        " << id << " [label=\"RAIZ\\n" << hashCorto
                        << "\", shape=box, style=filled, fillcolor=\"#1565C0\", "
                        << "fontcolor=white, fontsize=11];\n";
                } else if (nivel == 0) {
                    // HOJA: parte inferior, con hash truncado a 8 caracteres.
                    // Si esta es la ultima hoja Y el arbol tiene duplicacion
                    // por paridad, se marca visualmente distinto (no es un
                    // certificado real independiente, es una copia).
                    bool esHojaDuplicada = hayDuplicacion && (pos == levels[nivel].size() - 1);

                    if (esHojaDuplicada) {
                        dot << "        " << id << " [label=\"Hoja " << pos << " (copia)\\n" << hashCorto
                            << "\", shape=ellipse, style=\"filled,dashed\", fillcolor=\"#FFE0B2\"];\n";
                    } else {
                        dot << "        " << id << " [label=\"Hoja " << pos << "\\n" << hashCorto
                            << "\", shape=ellipse, style=filled, fillcolor=\"#C8E6C9\"];\n";
                    }
                } else {
                    // NODO INTERMEDIO: muestra el hash combinado
                    dot << "        " << id << " [label=\"Nivel " << nivel << "\\n" << hashCorto
                        << "\", shape=ellipse, style=filled, fillcolor=\"#E0E0E0\"];\n";
                }
            }

            dot << "    }\n";
        }

        dot << "\n";

        // -----------------------------------------------------------
        // Declarar las aristas: cada nodo del nivel N apunta hacia su
        // padre en el nivel N+1 (posicion / 2, por como se construye
        // el arbol en MerkleTree::build).
        // -----------------------------------------------------------
        for (size_t nivel = 0; nivel < numNiveles - 1; ++nivel) {
            for (size_t pos = 0; pos < levels[nivel].size(); ++pos) {
                size_t posPadre = pos / 2;
                dot << "    " << idNodo(nivel, pos) << " -> " << idNodo(nivel + 1, posPadre) << ";\n";
            }
        }

        dot << "}\n";
        dot.close();

        std::cout << "[OK] Archivo .dot generado: " << rutaDot << "\n";

        // -----------------------------------------------------------
        // Compilar el .dot a .png automaticamente, como exige el
        // enunciado (seccion 10, pag. 9): "dot -Tpng archivo.dot -o archivo.png"
        // -----------------------------------------------------------
        std::string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
        int resultado = std::system(comando.c_str());

        if (resultado == 0) {
            std::cout << "[OK] Imagen generada: " << rutaPng << "\n";
            return true;
        } else {
            std::cerr << "[ERROR] No se pudo compilar el .dot a .png. "
                      << "Verifica que Graphviz (comando 'dot') este instalado y en el PATH.\n";
            return false;
        }
    }
};

#endif