#ifndef AVL_H
#define AVL_H

#include <string>
#include <fstream>

struct NodoAVL
{
    int valor;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;
};


class AVL
{
    private:
        NodoAVL* raiz;

        // Utilidades
        int altura(NodoAVL* nodo);
        int factorBalance(NodoAVL* nodo);
        void actualizarAltura(NodoAVL* nodo);

        // Rotaciones
        NodoAVL* rotarDerecha(NodoAVL* y);
        NodoAVL* rotarIzquierda(NodoAVL* x);
        NodoAVL* balancear(NodoAVL* nodo);

        // Operaciones recursivas privadas
        NodoAVL* insertar(NodoAVL* nodo, int valor);
        NodoAVL* eliminar(NodoAVL* nodo, int valor);
        NodoAVL* minimoNodo(NodoAVL* nodo);

        // Recorridos privados
        void inorden(NodoAVL* nodo);
        void preorden(NodoAVL* nodo);
        void postorden(NodoAVL* nodo);

        // Graficación privada
        void graficarNodo(NodoAVL* nodo, std::ofstream& archivo);

    public:
        AVL();

        void insertar(int valor);
        void eliminar(int valor);
        bool buscar(int valor);

        void inorden();
        void preorden();
        void postorden();

        void graficar();
};

#endif