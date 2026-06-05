#include <iostream>
#include "Lista.h"

ListaSimple::ListaSimple()
{
    siguiente = nullptr;
}

// Insertar al final
void ListaSimple::insertarFinal(std::string dato)
{
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->dato = dato;
    nuevoNodo->siguiente = nullptr;

    if (siguiente == nullptr)
    {
        siguiente = nuevoNodo; // Si la lista está vacía, el nuevo nodo es el primero
    }
    else
    {
        NodoLista* actual = siguiente;
        while (actual->siguiente != nullptr)
        {
            actual = actual->siguiente; // Avanzar hasta el último nodo
        }
        actual->siguiente = nuevoNodo; // Enlazar el nuevo nodo al final de la lista
    }
};

void ListaSimple::mostrarLista()
{
    NodoLista* actual = siguiente;
    while (actual != nullptr)
    {
        std::cout << actual->dato << " -> ";
        actual = actual->siguiente; // Avanzar al siguiente nodo
    }
    std::cout << "nullptr" << std::endl; // Indicar el final de la lista
}

bool ListaSimple::eliminar(std::string dato)
{
    NodoLista* actual = siguiente;
    NodoLista* anterior = nullptr;

    while (actual != nullptr)
    {
        if (actual->dato == dato)
        {
            if (anterior == nullptr)
            {
                siguiente = actual->siguiente; // Eliminar el primer nodo
            }
            else
            {
                anterior->siguiente = actual->siguiente; // Enlazar el nodo anterior con el siguiente
            }
            delete actual; // Liberar memoria del nodo eliminado
            return true; // Salir después de eliminar el nodo
        }
        anterior = actual; // Avanzar al siguiente nodo
        actual = actual->siguiente;
    }
    return false; // Nodo no encontrado
}