#ifndef COLA_H

#define COLA_H
struct NodoCola
{
    int dato;
    NodoCola* siguiente;
};

class Cola
{
    private:
        NodoCola* frente;
        NodoCola* final;
    public:
        // Inicializa la cola
        Cola();
        
        // Encolar
        void encolar(int dato);
        // mostrar cola
        void mostrarCola();
        // Desencolar
        void desencolar();

        // Esta vacia
        bool estaVacia();

        // Buscar la posicion en cola
        int buscar(int dato);
};

#endif