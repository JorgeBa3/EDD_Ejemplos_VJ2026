#include <iostream>
#include <fstream>
#include "Pila.h"

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

void Pila::graficar()
{
    std::ofstream archivo("pila.dot");
    archivo << "digraph Pila {" << std::endl;
    archivo << "rankdir=TB;" << std::endl;  // Cambiar la dirección del grafo a de abajo hacia arriba
    archivo << "node [shape=record];" << std::endl; // Cambiar la forma de los nodos a rectángulo
    NodoPila* actual = cima;
    while (actual != nullptr)
    {
        // Cambiar circulo por rectángulo y mostrar los datos en el nodo
        archivo << "Nodo" << actual->dato << " [label=\"" << actual->dato << "\"];" << std::endl;
        if (actual->siguiente != nullptr)
        {
            archivo << "Nodo" << actual->dato << " -> Nodo" << actual->siguiente->dato << ";" << std::endl;
        }
        // Si es el nodo final, agregar una flecha hacia null
        else
        {
            archivo << "Nodo" << actual->dato << " -> null;" << std::endl;
        }
        actual = actual->siguiente;
    }
    archivo << "}" << std::endl;
    archivo.close();
    // System call para generar la imagen a partir del archivo .dot
    // Agragar a la carpeta reportes
    system("dot -Tpng pila.dot -o ./Reportes/Pila/pila.png");
}