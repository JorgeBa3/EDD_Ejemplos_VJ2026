#ifndef ARBOLB_H
#define ARBOLB_H

#include <vector>
#include <fstream>

struct NodoB
{
    //TODO: Trabajar el proyecto con arrays
    std::vector<int> claves;
    std::vector<NodoB*> hijos;
    bool esHoja;

    NodoB(bool hoja);
};

class ArbolB
{
    private:
        NodoB* raiz;
        int t; // grado minimo: cada nodo tiene entre (t-1) y (2t-1) claves

        // Divide el hijo i de "padre" (que esta lleno) en dos nodos
        void dividirHijo(NodoB* padre, int i);

        // Inserta un valor en un nodo que NO esta lleno
        void insertarNoLleno(NodoB* nodo, int valor);

        // Recorridos / busqueda privados
        void inorden(NodoB* nodo);
        NodoB* buscar(NodoB* nodo, int valor);

        // Graficacion privada
        void graficarNodo(NodoB* nodo, std::ofstream& archivo, int& contador, int idPadre, bool tienePadre);

    public:
        ArbolB(int grado = 2); // grado=2 -> arbol 2-3-4 (1 a 3 claves por nodo)

        void insertar(int valor);
        bool buscar(int valor);

        void inorden();
        void graficar();
};

#endif