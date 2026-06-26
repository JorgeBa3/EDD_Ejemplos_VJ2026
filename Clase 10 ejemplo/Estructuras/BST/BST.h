#ifndef BST_H
#define BST_H

#include <string>

struct NodoBST
{
    int valor;
    NodoBST* izq;
    NodoBST* der;
};


class BST
{
    private:
        NodoBST* raiz;
        NodoBST* insertar(NodoBST* nodo, int valor);
        void inorden(NodoBST* nodo);
        void preorden(NodoBST* nodo);
        void postorden(NodoBST* nodo);

    public:
        BST();
        void insertar(int valor);
        void inorden();
        void preorden();
        void postorden();

        void graficar();
        void graficarNodo(NodoBST* nodo, std::ofstream& archivo);
};

#endif