#include "Estructuras/Cola/Cola.h"

int main() {
    Cola cola;
    cola.encolar(1, "Juan", 1.75);
    cola.encolar(2, "Maria", 1.65);
    cola.graficar();
    cola.mostrarCola();
    std::cout << "ingrese el nomre del archivo de carga masiva: " << std::endl;
    std::string nombreArchivo;
    std::cin >> nombreArchivo;
    cola.cargaMasiva(nombreArchivo);
    cola.graficar();
    cola.mostrarCola();
    return 0;
}