#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <vector>
#include <string>
#include <stdexcept>
#include "HashFunction.h"

/*
 * ============================================================================
 *  MerkleTree — Árbol de Merkle para EDD CoffeeTrack Fase 2
 * ============================================================================
 *
 *  Cumple con los requisitos del enunciado:
 *   1. Hojas: hash_contenido de cada certificado (ya viene hasheado).
 *   2. Nodos internos: hash(hijo_izquierdo + hijo_derecho).
 *   3. Si el número de hojas es impar, la última hoja se duplica.
 *   4. Verificación: recalcular el camino de una hoja hasta la raíz.
 *   5. Soporta actualización incremental (agregar una hoja y reconstruir)
 *      y reconstrucción completa desde una lista de hashes (opción 5.1).
 *
 *  IMPORTANTE: No se usa std::map ni std::set. Solo std::vector como
 *  contenedor dinámico de bajo nivel (equivalente a un arreglo dinámico),
 *  permitido porque no reemplaza una estructura no lineal requerida.
 * ============================================================================
 */

class MerkleTree {
private:
    // levels[0] = hojas (hash_contenido de cada certificado)
    // levels[ultimo] = { raíz }
    std::vector<std::vector<std::string>> levels;

    // Número real de certificados (hojas ANTES de la posible duplicación
    // por paridad). Se guarda aparte porque levels[0] puede contener una
    // copia extra si el número de hojas era impar.
    size_t numCertificadosReales = 0;

    // Combina dos hashes hijos en el hash del padre
    static std::string combinar(const std::string& izquierdo, const std::string& derecho) {
        return HashFunction::hash(izquierdo + derecho);
    }

    // Construye todos los niveles superiores a partir de levels[0]
    void construirDesdeHojas() {
        levels.clear();
        // Si no hay hojas, no hay árbol
        // (se maneja en build() antes de llamar a esta función)
        while (levels.empty() || levels.back().size() > 1) {
            if (levels.empty()) break; // se llena afuera
        }
    }

public:
    MerkleTree() {}

    // -------------------------------------------------------------------
    // Construye el árbol completo desde una lista de hashes de hojas.
    // Se usa en:
    //   - Carga inicial / reconstrucción (opción 5.1)
    //   - Generación masiva de certificados (opción 4.2), una sola vez
    //     al final, con todos los hashes nuevos incluidos.
    // -------------------------------------------------------------------
    void build(const std::vector<std::string>& hashesHojas) {
        levels.clear();
        numCertificadosReales = hashesHojas.size();

        if (hashesHojas.empty()) {
            return; // árbol vacío: no hay certificados aún
        }

        // Nivel 0 = las hojas tal cual (ya vienen hasheadas: hash_contenido)
        std::vector<std::string> nivelActual = hashesHojas;
        levels.push_back(nivelActual);

        // Si el número de hojas es impar, se duplica la última (regla del enunciado)
        if (nivelActual.size() % 2 != 0 && nivelActual.size() > 1) {
            nivelActual.push_back(nivelActual.back());
            levels[0] = nivelActual; // guardamos la versión "duplicada" como nivel 0 real
        }

        // Construir niveles hacia arriba hasta llegar a la raíz (1 solo nodo)
        while (levels.back().size() > 1) {
            const std::vector<std::string>& actual = levels.back();
            std::vector<std::string> siguiente;

            for (size_t i = 0; i < actual.size(); i += 2) {
                if (i + 1 < actual.size()) {
                    siguiente.push_back(combinar(actual[i], actual[i + 1]));
                } else {
                    // Nodo impar sobrante en un nivel intermedio: se duplica también
                    siguiente.push_back(combinar(actual[i], actual[i]));
                }
            }

            // Si el nuevo nivel quedó impar y no es la raíz aún, se duplica el último
            if (siguiente.size() % 2 != 0 && siguiente.size() > 1) {
                siguiente.push_back(siguiente.back());
            }

            levels.push_back(siguiente);
        }
    }

    // -------------------------------------------------------------------
    // Actualización incremental: agrega un nuevo hash de certificado y
    // reconstruye el árbol. Se usa en la opción 4.1 (un solo certificado).
    //
    // NOTA DE DISEÑO: aunque se llama "incremental", la reconstrucción del
    // árbol completo es O(n). Para este proyecto es aceptable porque el
    // número de certificados es pequeño; se documenta esta decisión en el
    // README como trade-off simplicidad vs. eficiencia.
    // -------------------------------------------------------------------
    void agregarHoja(const std::string& nuevoHash) {
        std::vector<std::string> hojasActuales = getHojas();
        hojasActuales.push_back(nuevoHash);
        build(hojasActuales);
    }

    // Devuelve las hojas REALES (sin la duplicación de paridad, si la hubo)
    std::vector<std::string> getHojas() const {
        if (levels.empty()) return {};
        std::vector<std::string> hojas = levels[0];
        hojas.resize(numCertificadosReales); // recorta la copia duplicada si existe
        return hojas;
    }

    // Devuelve el nivel 0 TAL CUAL se usó para construir el árbol (puede
    // incluir la hoja duplicada). Se usa para reportes visuales, donde sí
    // queremos mostrar el nodo duplicado para que el diagrama sea fiel
    // a la estructura interna real del árbol.
    std::vector<std::string> getHojasConDuplicacion() const {
        if (levels.empty()) return {};
        return levels[0];
    }

    std::string getRoot() const {
        if (levels.empty() || levels.back().empty()) return "";
        return levels.back()[0];
    }

    // Número REAL de certificados cubiertos (sin contar la hoja duplicada)
    size_t getNumCertificados() const {
        return numCertificadosReales;
    }

    // true si la última hoja fue duplicada por ser un número impar de hojas
    bool tieneDuplicacionEnHojas() const {
        return !levels.empty() && levels[0].size() != numCertificadosReales;
    }

    size_t getAltura() const {
        return levels.size();
    }

    // -------------------------------------------------------------------
    // Expone todos los niveles internos del árbol (de hojas a raíz).
    // Se usa exclusivamente para generar reportes visuales (Graphviz,
    // opción 6.4); no se usa en la lógica de negocio del árbol.
    // -------------------------------------------------------------------
    const std::vector<std::vector<std::string>>& getLevels() const {
        return levels;
    }

    // -------------------------------------------------------------------
    // Genera la prueba de Merkle (camino de hashes hermanos) para la hoja
    // en la posición 'index'. Se usa para verificar integridad sin tener
    // que recorrer todo el árbol desde la opción 4.3 / 5.2.
    // -------------------------------------------------------------------
    struct PasoProof {
        std::string hashHermano;
        bool hermanoEsIzquierdo; // true si el hermano va a la izquierda al combinar
    };

    std::vector<PasoProof> getProof(size_t index) const {
        std::vector<PasoProof> proof;

        if (levels.empty() || index >= levels[0].size()) {
            throw std::out_of_range("Indice de hoja invalido en el Arbol de Merkle");
        }

        size_t idx = index;
        for (size_t lvl = 0; lvl < levels.size() - 1; ++lvl) {
            const std::vector<std::string>& nivel = levels[lvl];
            bool esNodoDerecho = (idx % 2 == 1);
            size_t idxHermano = esNodoDerecho ? idx - 1 : idx + 1;

            // Por construcción siempre existe el hermano (se duplicó si era impar)
            std::string hashHermano = (idxHermano < nivel.size()) ? nivel[idxHermano] : nivel[idx];

            // Si yo (la hoja actual) soy el nodo DERECHO, mi hermano va a la IZQUIERDA.
            // Si yo soy el nodo IZQUIERDO, mi hermano va a la DERECHA.
            PasoProof paso;
            paso.hashHermano = hashHermano;
            paso.hermanoEsIzquierdo = esNodoDerecho;
            proof.push_back(paso);

            idx /= 2;
        }

        return proof;
    }

    // -------------------------------------------------------------------
    // Verifica que 'hashCertificado' (ya calculado con HashFunction::hash
    // sobre el contenido original del certificado) pertenece al árbol y
    // no ha sido alterado, usando su proof y comparando contra la raíz.
    // -------------------------------------------------------------------
    bool verify(const std::string& hashCertificado,
                const std::vector<PasoProof>& proof,
                const std::string& raizEsperada) const {
        std::string actual = hashCertificado;

        for (const auto& paso : proof) {
            if (paso.hermanoEsIzquierdo) {
                actual = combinar(paso.hashHermano, actual);
            } else {
                actual = combinar(actual, paso.hashHermano);
            }
        }

        return actual == raizEsperada;
    }

    // Conveniencia: busca el índice de una hoja por su hash y verifica
    // directamente contra la raíz actual del árbol.
    bool verificarCertificado(const std::string& hashCertificado) const {
        std::vector<std::string> hojas = getHojas();
        for (size_t i = 0; i < hojas.size(); ++i) {
            if (hojas[i] == hashCertificado) {
                auto proof = getProof(i);
                return verify(hashCertificado, proof, getRoot());
            }
        }
        return false; // el hash no existe como hoja => no se puede verificar
    }
};

#endif