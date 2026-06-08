#include <iostream>
#include <fstream>
#include "Lista.h"
#include "../Pila/Pila.h"


ListaSimple::ListaSimple()
{
    siguiente = nullptr;
}

void ListaSimple::graficar()
{
    std::ofstream archivo("lista.dot");

    archivo << "digraph ListaConPilas {\n";

    archivo << "graph [ranksep=1.2, nodesep=0.6, splines=ortho, bgcolor=\"white\"];\n";
    archivo << "node [shape=record, fontname=\"Helvetica\", fontsize=11];\n";
    archivo << "edge [arrowsize=0.8];\n\n";

    archivo << "null_right [label=\"NULL\", shape=plaintext];\n\n";

    // ==========================
    // NODOS DE LA LISTA
    // ==========================
    NodoLista* actual = siguiente;

    while(actual != nullptr)
    {
        archivo << "\""
                << actual->seleccion
                << "\" [label=\"{ "
                << actual->seleccion
                << " | <next> * }\"];\n";

        actual = actual->siguiente;
    }

    archivo << "\n";

    // ==========================
    // MISMO RANK
    // ==========================
    archivo << "{ rank=same; ";

    actual = siguiente;

    while(actual != nullptr)
    {
        archivo << "\"" << actual->seleccion << "\"; ";
        actual = actual->siguiente;
    }

    archivo << "null_right; }\n\n";

    // ==========================
    // ENLACES DE LA LISTA
    // ==========================
    actual = siguiente;

    while(actual != nullptr)
    {
        if(actual->siguiente != nullptr)
        {
            archivo << "\""
                    << actual->seleccion
                    << "\":next -> \""
                    << actual->siguiente->seleccion
                    << "\";\n";
        }
        else
        {
            archivo << "\""
                    << actual->seleccion
                    << "\":next -> null_right;\n";
        }

        actual = actual->siguiente;
    }

    archivo << "\n";

    // ==========================
    // PILAS
    // ==========================
    actual = siguiente;

    while(actual != nullptr)
    {
        std::string top =
            "top_" + actual->seleccion;

        std::string nullPila =
            "null_" + actual->seleccion;

        archivo << top
                << " [label=\"TOP\", shape=plaintext];\n";

        archivo << nullPila
                << " [label=\"NULL\", shape=plaintext];\n";

        archivo << "\""
                << actual->seleccion
                << "\" -> "
                << top
                << " [style=dashed];\n";

        NodoPila* jugador =
            actual->jugadores.getCima();

        int i = 0;

        std::string primerNodo = "";

        while(jugador != nullptr)
        {
            std::string idNodo =
                actual->seleccion +
                "_jugador_" +
                std::to_string(i);

            if(i == 0)
            {
                primerNodo = idNodo;

                archivo << top
                        << " -> "
                        << idNodo
                        << ";\n";
            }

            archivo << idNodo
                    << " [label=\"{#"
                    << jugador->numero
                    << "|"
                    << jugador->nombre
                    << "|"
                    << jugador->posicion
                    << "| <next> *}\"];\n";

            if(jugador->siguiente != nullptr)
            {
                std::string siguienteNodo =
                    actual->seleccion +
                    "_jugador_" +
                    std::to_string(i + 1);

                archivo << idNodo
                        << ":next -> "
                        << siguienteNodo
                        << ";\n";
            }
            else
            {
                archivo << idNodo
                        << ":next -> "
                        << nullPila
                        << ";\n";
            }

            jugador = jugador->siguiente;
            i++;
        }

        archivo << "\n";

        actual = actual->siguiente;
    }

    archivo << "}\n";

    archivo.close();

    system("dot -Tpng lista.dot -o ./Reportes/Lista/lista.png");
}
void ListaSimple::insertarJugador(std::string seleccion, int numero, std::string nombre, std::string posicion)
{
    NodoLista* actual = siguiente;
    while (actual != nullptr)
    {
        if (actual->seleccion == seleccion)
        {
            actual->jugadores.push(numero, nombre, posicion); // Insertar el número del jugador en la pila de la selección
            return; // Salir después de insertar el jugador
        }
        actual = actual->siguiente; // Avanzar al siguiente nodo
    }
    std::cout << "Selección no encontrada: " << seleccion << std::endl; // Mensaje si la selección no existe
}

// Insertar al final
void ListaSimple::insertarFinal(std::string seleccion)
{
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->seleccion = seleccion;
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
        std::cout << actual->seleccion ;
        std::cout << " Jugadores: ";
        actual->jugadores.mostrarPila(); // Mostrar los jugadores de la selección
        std::cout << " -> ";
        actual = actual->siguiente; // Avanzar al siguiente nodo
    }
    std::cout << "nullptr" << std::endl; // Indicar el final de la lista
}

bool ListaSimple::eliminar(std::string seleccion)
{
    NodoLista* actual = siguiente;
    NodoLista* anterior = nullptr;

    while (actual != nullptr)
    {
        if (actual->seleccion == seleccion)
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