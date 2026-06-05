#ifndef LISTADOBLE_H
#define LISTADOBLE_H

struct NodoListaDoble
{
    int dato;
    NodoListaDoble* siguiente;
    NodoListaDoble* anterior;
};

class ListaDoble
{
    private:
        NodoListaDoble* cabeza;
        NodoListaDoble* cola;
    public:
        // Inicializa la lista
        ListaDoble();
        
        // Agregar al inicio
        void agregarAlInicio(int dato);
        // Agregar al final
        void agregarAlFinal(int dato);
        // mostrar lista
        void mostrarLista();
        // Eliminar un nodo
        void eliminarNodo(int dato);
        // Graficar la lista
        void graficar();
};

#endif