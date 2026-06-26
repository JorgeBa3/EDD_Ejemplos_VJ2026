#include <iostream>
#include "BST.h"
#include <fstream>
#include <functional>

// Constructor
BST::BST()
{
    raiz = nullptr;
}

// Insercion recursiva (privada)
NodoBST* BST::insertar(NodoBST* nodo, int valor)
{
    if (nodo == nullptr)
    {
        NodoBST* nuevoNodo = new NodoBST();
        nuevoNodo->valor = valor;
        nuevoNodo->izq = nullptr;
        nuevoNodo->der = nullptr;
        return nuevoNodo;
    }
    if (valor < nodo->valor)
        nodo->izq = insertar(nodo->izq, valor);
    else if (valor > nodo->valor)
        nodo->der = insertar(nodo->der, valor);

    return nodo;
}

// Insercion publica
void BST::insertar(int valor)
{
    raiz = insertar(raiz, valor);
}

// Recorrido inorden (privado)
void BST::inorden(NodoBST* nodo)
{
    if (nodo == nullptr) return;
    inorden(nodo->izq);
    std::cout << nodo->valor << " ";
    inorden(nodo->der);
}

// Recorrido preorden (privado)
void BST::preorden(NodoBST* nodo)
{
    if (nodo == nullptr) return;
    std::cout << nodo->valor << " ";
    preorden(nodo->izq);
    preorden(nodo->der);
}

// Recorrido postorden (privado)
void BST::postorden(NodoBST* nodo)
{
    if (nodo == nullptr) return;
    postorden(nodo->izq);
    postorden(nodo->der);
    std::cout << nodo->valor << " ";
}

// Recorridos publicos
void BST::inorden()
{
    inorden(raiz);
    std::cout << std::endl;
}

void BST::preorden()
{
    preorden(raiz);
    std::cout << std::endl;
}

void BST::postorden()
{
    postorden(raiz);
    std::cout << std::endl;
}

void BST::graficarNodo(NodoBST* nodo, std::ofstream& archivo)
{
    if (nodo == nullptr) return;

    archivo << "    " << nodo->valor << " [ordering=\"out\"];\n";

    // Ambos hijos
    if (nodo->izq && nodo->der)
    {
        archivo << "    " << nodo->valor << " -> " << nodo->izq->valor << ";\n";
        archivo << "    " << nodo->valor << " -> " << nodo->der->valor << ";\n";

        // Mantenerlos en la misma fila
        archivo << "    {rank=same; "
                << nodo->izq->valor << "; "
                << nodo->der->valor << ";}\n";
    }

    // Solo hijo izquierdo
    else if (nodo->izq)
    {
        std::string inv = "inv_der_" + std::to_string(nodo->valor);

        archivo << "    " << nodo->valor << " -> " << nodo->izq->valor << ";\n";
        archivo << "    " << inv << " [style=invis,width=0.1,label=\"\"];\n";
        archivo << "    " << nodo->valor << " -> " << inv << " [style=invis];\n";

        archivo << "    {rank=same; "
                << nodo->izq->valor << "; "
                << inv << ";}\n";
    }

    // Solo hijo derecho
    else if (nodo->der)
    {
        std::string inv = "inv_izq_" + std::to_string(nodo->valor);

        archivo << "    " << inv << " [style=invis,width=0.1,label=\"\"];\n";
        archivo << "    " << nodo->valor << " -> " << inv << " [style=invis];\n";
        archivo << "    " << nodo->valor << " -> " << nodo->der->valor << ";\n";

        archivo << "    {rank=same; "
                << inv << "; "
                << nodo->der->valor << ";}\n";
    }

    graficarNodo(nodo->izq, archivo);
    graficarNodo(nodo->der, archivo);
}

void BST::graficar()
{
    std::ofstream archivo("bst.dot");
    archivo << "digraph BST {" << std::endl;
    archivo << "    node [shape=circle, style=filled, fillcolor=white, fontname=\"Helvetica\"];" << std::endl;
    archivo << "    edge [arrowsize=0.7];" << std::endl;
    archivo << "    graph [nodesep=0.6, ranksep=0.7];" << std::endl;

    graficarNodo(raiz, archivo);

    archivo << "}" << std::endl;
    archivo.close();

    system("dot -Tpng bst.dot -o ./Reportes/BST/bst.png");
}