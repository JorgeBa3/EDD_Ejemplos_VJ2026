#include <iostream>
#include <fstream>
#include "ListaDoble.h"


ListaDoble::ListaDoble()
{
    cabeza = nullptr;
    cola = nullptr;
}

void ListaDoble::agregarAlInicio(int dato)
{
    NodoListaDoble* nuevoNodo = new NodoListaDoble();
    nuevoNodo->dato = dato;
    nuevoNodo->siguiente = cabeza;
    nuevoNodo->anterior = nullptr;

    if (cabeza != nullptr)
    {
        cabeza->anterior = nuevoNodo;
    }
    else
    {
        cola = nuevoNodo; // Si la lista estaba vacía, el nuevo nodo también es la cola
    }
    cabeza = nuevoNodo;
}

void ListaDoble::agregarAlFinal(int dato)
{
    NodoListaDoble* nuevoNodo = new NodoListaDoble();
    nuevoNodo->dato = dato;
    nuevoNodo->siguiente = nullptr;
    nuevoNodo->anterior = cola;

    if (cola != nullptr)
    {
        cola->siguiente = nuevoNodo;
    }
    else
    {
        cabeza = nuevoNodo; // Si la lista estaba vacía, el nuevo nodo también es la cabeza
    }
    cola = nuevoNodo;
}

void ListaDoble::mostrarLista()
{
    NodoListaDoble* actual = cabeza;
    while (actual != nullptr)
    {
        std::cout << actual->dato << " -> ";
        actual = actual->siguiente;
    }
    std::cout << "nullptr" << std::endl;
}

void ListaDoble::graficar()
{
    std::ofstream archivo("lista_doble.dot");
    archivo << "digraph ListaDoble {" << std::endl;
    archivo << "rankdir=LR;" << std::endl; // Orientación de izquierda a derecha
    archivo << "node [shape=record];" << std::endl;

    NodoListaDoble* actual = cabeza;
    int index = 0;
    while (actual != nullptr)
    {
        archivo << "node" << index << " [label=\"{ " << "anterior | " << actual->dato << " | " << "siguiente" << "}\"];" << std::endl;
        if (actual->anterior != nullptr)
        {
            archivo << "node" << index << " -> node" << index - 1 << ";" << std::endl;
        }
        if (actual->siguiente != nullptr)
        {
            archivo << "node" << index << " -> node" << index + 1 << ";" << std::endl;
        }
        actual = actual->siguiente;
        index++;
    }
    archivo << "}" << std::endl;
    system("dot -Tpng lista_doble.dot -o lista_doble.png");
}