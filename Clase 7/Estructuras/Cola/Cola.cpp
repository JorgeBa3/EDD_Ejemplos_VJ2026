#include <iostream>
#include "Cola.h"
#include <fstream>

// Constructor de la clase Cola
Cola::Cola()
{
    frente = nullptr;
    final = nullptr;
}

// Carga masiva csv
void Cola::cargaMasiva(std::string nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    int dato;
    std::string nombre;
    float estatura;

    // Leer la línea de encabezado
    std::string linea;
    std::getline(archivo, linea);

    while (std::getline(archivo, linea))
    {
        // Parsear la línea
        size_t pos1 = linea.find(',');
        size_t pos2 = linea.find(',', pos1 + 1);
        size_t pos3 = linea.find(',', pos2 + 1);
        size_t pos4 = linea.find(',', pos3 + 1);

        dato = std::stoi(linea.substr(0, pos1));
        nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        estatura = std::stof(linea.substr(pos2 + 1)); // Todo lo que queda después de pos2
        encolar(dato, nombre, estatura);
    }
    archivo.close();
}

// Funcion para encolar un elemento a la cola
void Cola::encolar(int dato, std::string nombre, float estatura)
{
    NodoCola* nuevoNodo = new NodoCola();
    nuevoNodo->dato = dato;
    nuevoNodo->nombre = nombre;
    nuevoNodo->estatura = estatura;
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
        std::cout << actual->dato << " " << actual->nombre << " " << actual->estatura << " -> ";
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

void Cola::graficar()
{
    
    std::ofstream archivo("cola.dot");
    archivo << "digraph Cola {" << std::endl;
    archivo << "rankdir=LR;" << std::endl; // Orientación de izquierda a derecha HORINZONTAL
    archivo << "node [shape=rectangle];" << std::endl; // Cambiar la forma de los nodos a rectángulo
    NodoCola* actual = frente;
    while (actual != nullptr)
    {
        // Cambiar circulo por rectángulo y mostrar los datos en el nodo
        
        archivo << "Nodo" << actual->dato << " [label=\"" << actual->dato << " | " << actual->nombre << " | " << actual->estatura << "\"];" << std::endl;
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
    system("dot -Tpng cola.dot -o ./Reportes/Cola/cola.png");
}