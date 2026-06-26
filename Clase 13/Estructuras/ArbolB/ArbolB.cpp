#include <iostream>
#include <fstream>
#include "ArbolB.h"

NodoB::NodoB(bool hoja)
{
    esHoja = hoja;
}

ArbolB::ArbolB(int grado)
{
    t = grado;
    raiz = nullptr;
}

// Divide al hijo "i" del nodo "padre" (que tiene 2t-1 claves, esta lleno)
// La clave del medio sube al padre, y el hijo se reparte en dos nodos.
void ArbolB::dividirHijo(NodoB* padre, int i)
{
    NodoB* hijo = padre->hijos[i];
    NodoB* nuevoHijo = new NodoB(hijo->esHoja);

    // El nuevo nodo se queda con las ultimas (t-1) claves de "hijo"
    for (int j = 0; j < t - 1; j++)
        nuevoHijo->claves.push_back(hijo->claves[j + t]);

    // Si no es hoja, tambien se reparten los hijos
    if (!hijo->esHoja)
    {
        for (int j = 0; j < t; j++)
            nuevoHijo->hijos.push_back(hijo->hijos[j + t]);
        hijo->hijos.resize(t);
    }

    int claveMedia = hijo->claves[t - 1];
    hijo->claves.resize(t - 1);

    // Insertar el nuevo hijo y la clave que sube dentro de "padre"
    padre->hijos.insert(padre->hijos.begin() + i + 1, nuevoHijo);
    padre->claves.insert(padre->claves.begin() + i, claveMedia);
}

// Inserta "valor" en "nodo", asumiendo que "nodo" NO esta lleno
void ArbolB::insertarNoLleno(NodoB* nodo, int valor)
{
    int i = (int)nodo->claves.size() - 1;

    if (nodo->esHoja)
    {
        // Desplazar claves mayores y ubicar el nuevo valor en su lugar
        nodo->claves.push_back(0);
        while (i >= 0 && nodo->claves[i] > valor)
        {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = valor;
    }
    else
    {
        // Buscar el hijo donde deberia insertarse
        while (i >= 0 && nodo->claves[i] > valor)
            i--;
        i++;

        if ((int)nodo->hijos[i]->claves.size() == 2 * t - 1)
        {
            dividirHijo(nodo, i);
            if (valor > nodo->claves[i])
                i++;
        }
        insertarNoLleno(nodo->hijos[i], valor);
    }
}

// insertar publica
void ArbolB::insertar(int valor)
{
    if (raiz == nullptr)
    {
        raiz = new NodoB(true);
        raiz->claves.push_back(valor);
        return;
    }

    if ((int)raiz->claves.size() == 2 * t - 1)
    {
        // La raiz esta llena: crece el arbol en altura
        NodoB* nuevaRaiz = new NodoB(false);
        nuevaRaiz->hijos.push_back(raiz);
        raiz = nuevaRaiz;
        dividirHijo(raiz, 0);
        insertarNoLleno(raiz, valor);
    }
    else
    {
        insertarNoLleno(raiz, valor);
    }
}

// busqueda privada
NodoB* ArbolB::buscar(NodoB* nodo, int valor)
{
    if (nodo == nullptr) return nullptr;

    int i = 0;
    while (i < (int)nodo->claves.size() && valor > nodo->claves[i])
        i++;

    if (i < (int)nodo->claves.size() && nodo->claves[i] == valor)
        return nodo;

    if (nodo->esHoja)
        return nullptr;

    return buscar(nodo->hijos[i], valor);
}

// busqueda publica
bool ArbolB::buscar(int valor)
{
    return buscar(raiz, valor) != nullptr;
}

// inorden privada (recorre todas las claves en orden ascendente)
void ArbolB::inorden(NodoB* nodo)
{
    if (nodo == nullptr) return;

    int i;
    for (i = 0; i < (int)nodo->claves.size(); i++)
    {
        if (!nodo->esHoja)
            inorden(nodo->hijos[i]);
        std::cout << nodo->claves[i] << " ";
    }
    if (!nodo->esHoja)
        inorden(nodo->hijos[i]);
}

// inorden publica
void ArbolB::inorden()
{
    inorden(raiz);
    std::cout << std::endl;
}

void ArbolB::graficarNodo(NodoB* nodo, std::ofstream& archivo, int& contador, int idPadre, bool tienePadre)
{
    if (nodo == nullptr) return;

    int idActual = contador++;

    // Etiqueta con las claves separadas, ej: "5 | 10 | 15"
    archivo << "    nodo" << idActual << " [label=\"";
    for (size_t i = 0; i < nodo->claves.size(); i++)
    {
        archivo << nodo->claves[i];
        if (i != nodo->claves.size() - 1) archivo << " | ";
    }
    archivo << "\"];\n";

    if (tienePadre)
        archivo << "    nodo" << idPadre << " -> nodo" << idActual << ";\n";

    if (!nodo->esHoja)
    {
        for (NodoB* hijo : nodo->hijos)
            graficarNodo(hijo, archivo, contador, idActual, true);
    }
}

void ArbolB::graficar()
{
    std::ofstream archivo("arbolb.dot");
    archivo << "digraph ArbolB {" << std::endl;
    archivo << "    node [shape=record, style=filled, fillcolor=white, fontname=\"Helvetica\"];" << std::endl;
    archivo << "    edge [arrowsize=0.7];" << std::endl;
    archivo << "    graph [nodesep=0.6, ranksep=0.7];" << std::endl;

    int contador = 0;
    graficarNodo(raiz, archivo, contador, -1, false);

    archivo << "}" << std::endl;
    archivo.close();

    system("dot -Tpng arbolb.dot -o ./Reportes/ArbolB/arbolb.png");
}