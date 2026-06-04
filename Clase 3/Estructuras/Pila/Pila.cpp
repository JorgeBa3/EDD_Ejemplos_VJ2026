#include <iostream>
#include "Pila.h"
#include <fstream>

// Constructor de la clase Pila
Pila::Pila()
{
    cima = nullptr;
}
    
// Funcion para encolar un elemento a la pila
void Pila::push(int dato)
{
    NodoPila* nuevoNodo = new NodoPila();
    nuevoNodo->dato = dato;
    nuevoNodo->siguiente = cima;
    // Si la pila está vacia
    if (estaVacia())
    {
        cima = nuevoNodo;
    }
    // La pila   no esta vacia
    else
    {
        // cima->siguiente = nullptr;
        cima = nuevoNodo;
    }
}


// Funcion para mostrar los elementos de la pila
void Pila::mostrarPila()
{
    NodoPila* actual = cima ;
    while (actual != nullptr)
    {
        std::cout << actual->dato << " ";
        actual = actual->siguiente;
    }
    std::cout << std::endl;
}


// Funcion desencolar un elemento de la pila
int Pila::pop()
{
    if (estaVacia())
    {
        std::cout << "La pila esta vacia" << std::endl;
        return -1;
    }
    NodoPila* nodoAEliminar = cima;
    cima = cima->siguiente;
    int datoEliminado = nodoAEliminar->dato;
    delete nodoAEliminar;
    return datoEliminado;
}

bool Pila::estaVacia()
{
    return cima == nullptr;
}

// Funcion para buscar la posicion de un elemento en la pila
int Pila::peek()
{
    if (estaVacia())
    {
        std::cout << "La pila esta vacia" << std::endl;
        return -1;
    }
    return cima->dato;
}

void Pila::graficarPila()
{
    std::ofstream archivo("pila.dot");
    archivo << "digraph Pila {" << std::endl;
    archivo << "rankdir=TB;" << std::endl;
    archivo << "node [shape=record];" << std::endl;

    NodoPila* actual = cima;
    int index = 0;
    while (actual != nullptr)
    {
        if (actual->siguiente != nullptr)
        {
            archivo << "node" << index
                    << " [label=\"{" << actual->dato
                    << " | <next> *}\"];" << std::endl;

            archivo << "node" << index << ":next -> node" << index + 1 << ";" << std::endl;
        }
        else
        {
            archivo << "node" << index
                    << " [label=\"{" << actual->dato
                    << " | <next> /}\"];" << std::endl;
        }

        actual = actual->siguiente;
        index++;
    }

    archivo << "}" << std::endl;
    archivo.close();
    system("dot -Tpng pila.dot -o pila.png");
}
