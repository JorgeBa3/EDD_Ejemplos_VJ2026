#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <iostream>
#include <stdexcept>


class HashTable {
private:
    // Nodo de la lista enlazada de cada bucket (encadenamiento manual)
    struct Nodo {
        std::string clave;
        std::string valor;
        Nodo* siguiente;

        Nodo(const std::string& k, const std::string& v)
            : clave(k), valor(v), siguiente(nullptr) {}
    };

    Nodo** buckets;          // arreglo dinamico de punteros (cabezas de lista)
    size_t capacidad;        // tamaño actual del arreglo de buckets
    size_t numElementos;     // cantidad total de pares clave-valor almacenados

    static const size_t CAPACIDAD_INICIAL = 8;
    static constexpr double FACTOR_CARGA_MAXIMO = 0.7;

    // ---------------------------------------------------------------
    // Función hash polinomial (ver documentación arriba)
    // ---------------------------------------------------------------
    size_t calcularHash(const std::string& clave) const {
        unsigned long long h = 0;
        const unsigned long long PRIMO = 31;

        for (char c : clave) {
            h = h * PRIMO + static_cast<unsigned char>(c);
        }

        return static_cast<size_t>(h % capacidad);
    }

    double factorCarga() const {
        return static_cast<double>(numElementos) / static_cast<double>(capacidad);
    }

    // ---------------------------------------------------------------
    // Rehashing: crea un arreglo de buckets del doble de tamaño y
    // reinserta todos los elementos existentes (sus indices cambian
    // porque dependen de 'capacidad', que tambien cambia).
    // ---------------------------------------------------------------
    void rehash() {
        size_t capacidadAnterior = capacidad;
        Nodo** bucketsAnteriores = buckets;

        capacidad *= 2;
        buckets = new Nodo*[capacidad];
        for (size_t i = 0; i < capacidad; ++i) {
            buckets[i] = nullptr;
        }

        size_t elementosAntesDeReinsertar = numElementos;
        numElementos = 0; // se vuelve a contar al reinsertar con insertar()

        for (size_t i = 0; i < capacidadAnterior; ++i) {
            Nodo* actual = bucketsAnteriores[i];
            while (actual != nullptr) {
                Nodo* siguiente = actual->siguiente; // guardar antes de mover
                insertarSinRehash(actual->clave, actual->valor);
                delete actual; // se crea un nodo nuevo en insertarSinRehash
                actual = siguiente;
            }
        }

        delete[] bucketsAnteriores;

        std::cout << "[REHASH] Tabla crecio de " << capacidadAnterior
                  << " a " << capacidad << " buckets ("
                  << elementosAntesDeReinsertar << " elementos reinsertados)\n";
    }

    // Inserta sin verificar el factor de carga (usado internamente por rehash
    // para evitar una recursion infinita de rehash-dentro-de-rehash)
    void insertarSinRehash(const std::string& clave, const std::string& valor) {
        size_t idx = calcularHash(clave);

        Nodo* actual = buckets[idx];
        while (actual != nullptr) {
            if (actual->clave == clave) {
                actual->valor = valor; // clave ya existe: se actualiza el valor
                return;
            }
            actual = actual->siguiente;
        }

        Nodo* nuevo = new Nodo(clave, valor);
        nuevo->siguiente = buckets[idx];
        buckets[idx] = nuevo;
        numElementos++;
    }

public:
    HashTable() {
        capacidad = CAPACIDAD_INICIAL;
        numElementos = 0;
        buckets = new Nodo*[capacidad];
        for (size_t i = 0; i < capacidad; ++i) {
            buckets[i] = nullptr;
        }
    }

    // Destructor: libera todas las listas enlazadas y el arreglo de buckets
    ~HashTable() {
        for (size_t i = 0; i < capacidad; ++i) {
            Nodo* actual = buckets[i];
            while (actual != nullptr) {
                Nodo* siguiente = actual->siguiente;
                delete actual;
                actual = siguiente;
            }
        }
        delete[] buckets;
    }

    // Prohibir copia simple (evita doble-free); se podria implementar
    // un copy constructor profundo si el curso lo requiere
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // ---------------------------------------------------------------
    // Inserta un par clave-valor. Si la clave ya existe, actualiza
    // el valor (comportamiento estandar de un diccionario/mapa).
    // ---------------------------------------------------------------
    void insertar(const std::string& clave, const std::string& valor) {
        insertarSinRehash(clave, valor);

        if (factorCarga() > FACTOR_CARGA_MAXIMO) {
            rehash();
        }
    }

    // ---------------------------------------------------------------
    // Busca una clave. Devuelve true si existe y copia el valor en
    // 'valorEncontrado'. Devuelve false si la clave no existe.
    // ---------------------------------------------------------------
    bool buscar(const std::string& clave, std::string& valorEncontrado) const {
        size_t idx = calcularHash(clave);

        Nodo* actual = buckets[idx];
        while (actual != nullptr) {
            if (actual->clave == clave) {
                valorEncontrado = actual->valor;
                return true;
            }
            actual = actual->siguiente;
        }

        return false;
    }

    // Variante conveniente que lanza excepcion si no se encuentra
    // (similar a std::map::at)
    std::string obtener(const std::string& clave) const {
        std::string valor;
        if (!buscar(clave, valor)) {
            throw std::out_of_range("Clave no encontrada: " + clave);
        }
        return valor;
    }

    // ---------------------------------------------------------------
    // Elimina una clave de la tabla. Devuelve true si se elimino,
    // false si la clave no existia.
    // ---------------------------------------------------------------
    bool eliminar(const std::string& clave) {
        size_t idx = calcularHash(clave);

        Nodo* actual = buckets[idx];
        Nodo* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->clave == clave) {
                if (anterior == nullptr) {
                    buckets[idx] = actual->siguiente; // era la cabeza de la lista
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                numElementos--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }

        return false; // no se encontro la clave
    }

    bool contiene(const std::string& clave) const {
        std::string valorTemporal;
        return buscar(clave, valorTemporal);
    }

    size_t size() const { return numElementos; }
    size_t getCapacidad() const { return capacidad; }
    double getFactorCarga() const { return factorCarga(); }

    // ---------------------------------------------------------------
    // Util para clase: imprime visualmente cada bucket y su lista
    // enlazada, para que los estudiantes vean las colisiones reales.
    // ---------------------------------------------------------------
    void imprimirEstructura() const {
        std::cout << "\n=== Estructura interna de la Tabla Hash ===\n";
        std::cout << "Capacidad: " << capacidad
                  << " | Elementos: " << numElementos
                  << " | Factor de carga: " << factorCarga() << "\n\n";

        for (size_t i = 0; i < capacidad; ++i) {
            std::cout << "[" << i << "] -> ";
            Nodo* actual = buckets[i];
            if (actual == nullptr) {
                std::cout << "(vacio)";
            }
            while (actual != nullptr) {
                std::cout << "(" << actual->clave << ": " << actual->valor << ")";
                if (actual->siguiente != nullptr) std::cout << " -> ";
                actual = actual->siguiente;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

#endif