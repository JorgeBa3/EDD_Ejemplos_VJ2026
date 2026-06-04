#ifndef PILA_H

#define PILA_H
struct NodoPila
{
    int dato;
    NodoPila* siguiente;
};

class Pila
{
    private:
        NodoPila* cima;
    public:
        // Inicializa la cola
        Pila();
        
        // Encolar
        void push(int dato);
        // mostrar cola
        void mostrarPila();
        // Desencolar
        int pop();

        // Esta vacia
        bool estaVacia();

        // Buscar la posicion en cola
        int peek();

        void graficarPila();
};

#endif