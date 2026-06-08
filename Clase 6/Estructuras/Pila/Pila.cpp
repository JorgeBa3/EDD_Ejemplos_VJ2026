#include <iostream>
#include <fstream>
#include "Pila.h"

// Constructor de la clase Pila
Pila::Pila()
{
    cima = nullptr;
}
    
// Funcion para encolar un elemento a la pila
void Pila::push(int numero, std::string nombre, std::string posicion)
{
    NodoPila* nuevoNodo = new NodoPila();
    nuevoNodo->numero = numero;
    nuevoNodo->nombre = nombre;
    nuevoNodo->posicion = posicion;
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
        std::cout << "| Numero: " << actual->numero << ", Nombre: " << actual->nombre << ", Posicion: " << actual->posicion << std::endl;
        actual = actual->siguiente;
    }
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
    int datoEliminado = nodoAEliminar->numero;
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
    return cima->numero;
}

void Pila::graficarPila(std::ofstream& archivo, const std::string& prefijo)
{
    NodoPila* actual = cima;
    while (actual != nullptr)
    {
        std::string id = prefijo + std::to_string(actual->numero);
        archivo << id << " [label=\"#" << actual->numero << "\\n" << actual->nombre << "\\n" << actual->posicion << "\"];" << std::endl;
        if (actual->siguiente != nullptr)
        {
            std::string idSig = prefijo + std::to_string(actual->siguiente->numero);
            archivo << id << " -> " << idSig << ";" << std::endl;
        }
        else
        {
            archivo << id << " -> null;" << std::endl;
        }
        actual = actual->siguiente;
    }
}

void Pila::graficar()
{
    std::ofstream archivo("pila.dot");

    archivo << "digraph Pila {\n";
    archivo << "rankdir=TB;\n";
    archivo << "node [shape=record];\n";

    archivo << "top [label=\"TOP\", shape=plaintext];\n";
    archivo << "null_node [label=\"NULL\", shape=plaintext];\n";

    NodoPila* actual = cima;

    int i = 0;

    if(actual != nullptr)
    {
        archivo << "top -> nodo0;\n";
    }

    while(actual != nullptr)
    {
        archivo << "nodo" << i
                << " [label=\"{#"
                << actual->numero
                << "|"
                << actual->nombre
                << "|"
                << actual->posicion
                << "}\"];\n";

        if(actual->siguiente != nullptr)
        {
            archivo << "nodo"
                    << i
                    << " -> nodo"
                    << i + 1
                    << ";\n";
        }
        else
        {
            archivo << "nodo"
                    << i
                    << " -> null_node;\n";
        }

        actual = actual->siguiente;
        i++;
    }

    archivo << "}\n";

    archivo.close();

    system("dot -Tpng pila.dot -o ./Reportes/Pila/pila.png");
}