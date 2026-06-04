#ifndef COLA_H

#define COLA_H
struct NodoCola
{
    int dato;
    std::string finca;
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
        void encolar(int dato, std::string nombre);
        // mostrar cola
        void mostrarCola();
        // Desencolar
        void desencolar();

        // Esta vacia
        bool estaVacia();

        // Buscar la posicion en cola
        int buscar(int dato);
        void graficarCola();
};

#endif