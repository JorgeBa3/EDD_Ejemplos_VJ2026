#ifndef LISTA_H
#define LISTA_H
#include "../Pila/Pila.h"
#include <string>
struct NodoLista
{
    std::string seleccion;
    Pila jugadores;
    NodoLista* siguiente;
};

class ListaSimple
{
    private:
        NodoLista* siguiente;
    public:
        // Constructor: inicializa la lista vacía
        ListaSimple();

        void graficar();
        // Insertar al final
        void insertarFinal(std::string seleccion);

        // Eliminar por valor
        bool eliminar(std::string seleccion);

        // Mostrar la lista
        void mostrarLista();
        
        void insertarJugador(std::string seleccion, int numero, std::string nombre, std::string posicion);

        void graficarLista();
};

#endif