#include <iostream>
#include "HashTable.h"

/*
 * ============================================================================
 *  Demo en vivo para clase — Tabla Hash con Encadenamiento
 * ============================================================================
 */

int main() {
    std::cout << "=== DEMO: Tabla Hash con Encadenamiento ===\n\n";

    HashTable tabla;

    std::cout << "--- Insertando estudiantes (codigo_carnet -> nombre) ---\n";
    tabla.insertar("201700123", "Ana Lopez");
    tabla.insertar("201800456", "Carlos Mendez");
    tabla.insertar("201900789", "Maria Garcia");
    tabla.insertar("202000111", "Luis Ajucum");
    tabla.insertar("202100222", "Sofia Ramirez");

    tabla.imprimirEstructura();

    // ----------------------------------------------------------------
    // PARTE 2: Busqueda
    // ----------------------------------------------------------------
    std::cout << "--- Busquedas ---\n";
    std::string resultado;

    if (tabla.buscar("201900789", resultado)) {
        std::cout << "Encontrado: 201900789 -> " << resultado << "\n";
    }

    if (!tabla.buscar("999999999", resultado)) {
        std::cout << "No encontrado: 999999999 (correcto, no existe)\n";
    }
    std::cout << "\n";

    // ----------------------------------------------------------------
    // PARTE 3: Actualizar un valor existente (insertar con clave repetida)
    // ----------------------------------------------------------------
    std::cout << "--- Actualizando valor de una clave existente ---\n";
    std::cout << "Antes: 201700123 -> " << tabla.obtener("201700123") << "\n";
    tabla.insertar("201700123", "Ana Lopez (graduada)");
    std::cout << "Despues: 201700123 -> " << tabla.obtener("201700123") << "\n\n";

    // ----------------------------------------------------------------
    // PARTE 4: Eliminacion
    // ----------------------------------------------------------------
    std::cout << "--- Eliminando una clave ---\n";
    bool eliminado = tabla.eliminar("202000111");
    std::cout << "Eliminar 202000111: " << (eliminado ? "exitoso" : "fallido") << "\n";
    std::cout << "Tamano actual: " << tabla.size() << " elementos\n\n";

    // ----------------------------------------------------------------
    // PARTE 5: Forzar colisiones a proposito (para que se vean en clase)
    // ----------------------------------------------------------------
    std::cout << "--- Forzando colisiones con claves cortas ---\n";
    std::cout << "(usamos pocas claves simples para que caigan en el mismo bucket)\n\n";

    HashTable tablaColisiones;
    // Con capacidad inicial 8, estas claves de una letra suelen colisionar
    // entre si dependiendo del valor ASCII y el modulo. Las mostramos para
    // que la clase vea el patron real con la funcion hash usada.
    std::string clavesDemo[] = {"A", "I", "Q", "Y", "B", "J"};
    for (const auto& clave : clavesDemo) {
        tablaColisiones.insertar(clave, "valor_" + clave);
    }
    tablaColisiones.imprimirEstructura();

    // ----------------------------------------------------------------
    // PARTE 6: Disparar el rehashing en vivo
    // ----------------------------------------------------------------
    std::cout << "--- Forzando rehashing (factor de carga > 0.7) ---\n";
    HashTable tablaRehash;
    std::cout << "Capacidad inicial: " << tablaRehash.getCapacidad() << "\n\n";

    for (int i = 1; i <= 7; ++i) {
        std::string clave = "clave" + std::to_string(i);
        tablaRehash.insertar(clave, "valor" + std::to_string(i));
        std::cout << "Insertado " << clave
                  << " | elementos=" << tablaRehash.size()
                  << " | capacidad=" << tablaRehash.getCapacidad()
                  << " | factor_carga=" << tablaRehash.getFactorCarga() << "\n";
    }

    std::cout << "\n";
    tablaRehash.imprimirEstructura();

    return 0;
}