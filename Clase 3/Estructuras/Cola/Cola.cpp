#include <iostream>
#include "Cola.h"

// Constructor de la clase Cola
Cola::Cola()
{
    frente = nullptr;
    final = nullptr;
}

// Funcion para encolar un elemento a la cola
void Cola::encolar(int dato)
{
    NodoCola* nuevoNodo = new NodoCola();
    nuevoNodo->dato = dato;
    nuevoNodo->siguiente = nullptr;
    // Si la cola está vacia
    if (estaVacia())
    {
        frente = nuevoNodo;
        final = nuevoNodo;
    }
    // La cola no esta vacia
    else
    {
        final->siguiente = nuevoNodo;
        final = nuevoNodo;
    }
}


// Funcion para mostrar los elementos de la cola
void Cola::mostrarCola()
{
    NodoCola* actual = frente;
    while (actual != nullptr)
    {
        std::cout << actual->dato << " ";
        actual = actual->siguiente;
    }
    std::cout << std::endl;
}


// Funcion desencolar un elemento de la cola
void Cola::desencolar()
{
    if (estaVacia())
    {
        std::cout << "La cola esta vacia" << std::endl;
        return;
    }
    NodoCola* nodoAEliminar = frente;
    frente = frente->siguiente;
    delete nodoAEliminar;
}

bool Cola::estaVacia()
{
    return frente == nullptr;
}

// Funcion para buscar la posicion de un elemento en la cola
int Cola::buscar(int dato)
{
    NodoCola* actual = frente;
    int posicion = 0;
    while (actual != nullptr)
    {
        if (actual->dato == dato)
        {
            return posicion;
        }
        actual = actual->siguiente;
        posicion++;
    }
    return -1; // No se encontro el elemento
}