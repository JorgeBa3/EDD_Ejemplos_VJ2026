#ifndef LISTA_H
#define LISTA_H
#include <string>
struct NodoLista
{
    std::string dato;
    NodoLista* siguiente;
};

class ListaSimple
{
    private:
        NodoLista* siguiente;
    public:
        // Constructor: inicializa la lista vacía
        ListaSimple();

        // Insertar al final
        void insertarFinal(std::string dato);

        // Eliminar por valor
        bool eliminar(std::string dato);

        // Mostrar la lista
        void mostrarLista();

};

#endif