#include <iostream>
#include <fstream>
#include <algorithm>
#include "AVL.h"

// ─────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────

AVL::AVL()
{
    raiz = nullptr;
}

// ─────────────────────────────────────────────
//  Utilidades internas
// ─────────────────────────────────────────────

int AVL::altura(NodoAVL* nodo)
{
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

int AVL::factorBalance(NodoAVL* nodo)
{
    if (nodo == nullptr) return 0;
    return altura(nodo->izq) - altura(nodo->der);
}

void AVL::actualizarAltura(NodoAVL* nodo)
{
    if (nodo == nullptr) return;
    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));
}

// ─────────────────────────────────────────────
//  Rotaciones
// ─────────────────────────────────────────────

//        y                x
//       / \              / \
//      x   C   --->    A    y
//     / \                  / \
//    A   B                B   C
NodoAVL* AVL::rotarDerecha(NodoAVL* y)
{
    NodoAVL* x  = y->izq;
    NodoAVL* B  = x->der;

    x->der = y;
    y->izq = B;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;   // nueva raíz del subárbol
}

//      x                  y
//     / \                / \
//    A    y   --->      x   C
//        / \           / \
//       B   C         A   B
NodoAVL* AVL::rotarIzquierda(NodoAVL* x)
{
    NodoAVL* y  = x->der;
    NodoAVL* B  = y->izq;

    y->izq = x;
    x->der = B;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;   // nueva raíz del subárbol
}

// Aplica la rotación correcta según el factor de balance
NodoAVL* AVL::balancear(NodoAVL* nodo)
{
    actualizarAltura(nodo);
    int fb = factorBalance(nodo);

    // Caso Izquierda-Izquierda
    if (fb > 1 && factorBalance(nodo->izq) >= 0)
        return rotarDerecha(nodo);

    // Caso Izquierda-Derecha
    if (fb > 1 && factorBalance(nodo->izq) < 0)
    {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (fb < -1 && factorBalance(nodo->der) <= 0)
        return rotarIzquierda(nodo);

    // Caso Derecha-Izquierda
    if (fb < -1 && factorBalance(nodo->der) > 0)
    {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;    // ya estaba balanceado
}

// ─────────────────────────────────────────────
//  Inserción
// ─────────────────────────────────────────────

NodoAVL* AVL::insertar(NodoAVL* nodo, int valor)
{
    // Inserción BST normal
    if (nodo == nullptr)
    {
        NodoAVL* nuevo = new NodoAVL();
        nuevo->valor   = valor;
        nuevo->altura  = 1;
        nuevo->izq     = nullptr;
        nuevo->der     = nullptr;
        return nuevo;
    }

    if (valor < nodo->valor)
        nodo->izq = insertar(nodo->izq, valor);
    else if (valor > nodo->valor)
        nodo->der = insertar(nodo->der, valor);
    else
        return nodo;    // duplicados ignorados

    // Rebalanceo en la vuelta de la recursión
    return balancear(nodo);
}

void AVL::insertar(int valor)
{
    raiz = insertar(raiz, valor);
}

// ─────────────────────────────────────────────
//  Eliminación
// ─────────────────────────────────────────────

NodoAVL* AVL::minimoNodo(NodoAVL* nodo)
{
    NodoAVL* actual = nodo;
    while (actual->izq != nullptr)
        actual = actual->izq;
    return actual;
}

NodoAVL* AVL::eliminar(NodoAVL* nodo, int valor)
{
    if (nodo == nullptr) return nullptr;

    if (valor < nodo->valor)
        nodo->izq = eliminar(nodo->izq, valor);
    else if (valor > nodo->valor)
        nodo->der = eliminar(nodo->der, valor);
    else
    {
        // Nodo encontrado
        if (nodo->izq == nullptr || nodo->der == nullptr)
        {
            NodoAVL* temp = nodo->izq ? nodo->izq : nodo->der;

            if (temp == nullptr)
            {
                // Sin hijos
                delete nodo;
                return nullptr;
            }
            else
            {
                // Un solo hijo: copiar contenido y borrar el temporal
                *nodo = *temp;
                delete temp;
            }
        }
        else
        {
            // Dos hijos: sucesor inorden (mínimo del subárbol derecho)
            NodoAVL* sucesor = minimoNodo(nodo->der);
            nodo->valor = sucesor->valor;
            nodo->der   = eliminar(nodo->der, sucesor->valor);
        }
    }

    return balancear(nodo);
}

void AVL::eliminar(int valor)
{
    raiz = eliminar(raiz, valor);
}

// ─────────────────────────────────────────────
//  Búsqueda
// ─────────────────────────────────────────────

bool AVL::buscar(int valor)
{
    NodoAVL* actual = raiz;
    while (actual != nullptr)
    {
        if (valor == actual->valor)  return true;
        if (valor <  actual->valor)  actual = actual->izq;
        else                         actual = actual->der;
    }
    return false;
}

// ─────────────────────────────────────────────
//  Recorridos
// ─────────────────────────────────────────────

void AVL::inorden(NodoAVL* nodo)
{
    if (nodo == nullptr) return;
    inorden(nodo->izq);
    std::cout << nodo->valor << " ";
    inorden(nodo->der);
}

void AVL::preorden(NodoAVL* nodo)
{
    if (nodo == nullptr) return;
    std::cout << nodo->valor << " ";
    preorden(nodo->izq);
    preorden(nodo->der);
}

void AVL::postorden(NodoAVL* nodo)
{
    if (nodo == nullptr) return;
    postorden(nodo->izq);
    postorden(nodo->der);
    std::cout << nodo->valor << " ";
}

void AVL::inorden()   { inorden(raiz);   std::cout << std::endl; }
void AVL::preorden()  { preorden(raiz);  std::cout << std::endl; }
void AVL::postorden() { postorden(raiz); std::cout << std::endl; }

// ─────────────────────────────────────────────
//  Graficación con Graphviz
// ─────────────────────────────────────────────

void AVL::graficarNodo(NodoAVL* nodo, std::ofstream& archivo)
{
    if (nodo == nullptr) return;

    // Mostrar valor y factor de balance en la etiqueta
    archivo << "    " << nodo->valor
            << " [label=\"" << nodo->valor
            << "\\nFB=" << factorBalance(nodo) << "\""
            << ", ordering=\"out\"];\n";

    if (nodo->izq && nodo->der)
    {
        archivo << "    " << nodo->valor << " -> " << nodo->izq->valor << ";\n";
        archivo << "    " << nodo->valor << " -> " << nodo->der->valor << ";\n";
        archivo << "    {rank=same; "
                << nodo->izq->valor << "; "
                << nodo->der->valor << ";}\n";
    }
    else if (nodo->izq)
    {
        std::string inv = "inv_der_" + std::to_string(nodo->valor);
        archivo << "    " << nodo->valor << " -> " << nodo->izq->valor << ";\n";
        archivo << "    " << inv << " [style=invis,width=0.1,label=\"\"];\n";
        archivo << "    " << nodo->valor << " -> " << inv << " [style=invis];\n";
        archivo << "    {rank=same; " << nodo->izq->valor << "; " << inv << ";}\n";
    }
    else if (nodo->der)
    {
        std::string inv = "inv_izq_" + std::to_string(nodo->valor);
        archivo << "    " << inv << " [style=invis,width=0.1,label=\"\"];\n";
        archivo << "    " << nodo->valor << " -> " << inv << " [style=invis];\n";
        archivo << "    " << nodo->valor << " -> " << nodo->der->valor << ";\n";
        archivo << "    {rank=same; " << inv << "; " << nodo->der->valor << ";}\n";
    }

    graficarNodo(nodo->izq, archivo);
    graficarNodo(nodo->der, archivo);
}

void AVL::graficar()
{
    std::ofstream archivo("avl.dot");
    archivo << "digraph AVL {\n";
    archivo << "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Helvetica\"];\n";
    archivo << "    edge [arrowsize=0.7];\n";
    archivo << "    graph [nodesep=0.6, ranksep=0.7];\n";

    graficarNodo(raiz, archivo);

    archivo << "}\n";
    archivo.close();

    system("dot -Tpng avl.dot -o ./Reportes/AVL/avl.png");
}