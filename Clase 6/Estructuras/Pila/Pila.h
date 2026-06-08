#ifndef PILA_H

#define PILA_H
#include <string>

struct NodoPila
{
    int numero;
    std::string nombre;
    std::string posicion;
    NodoPila* siguiente;
};

class Pila
{
    private:
        NodoPila* cima;
    public:
        // Inicializa la cola
        Pila();
        // En Pila.h, dentro de public:
        NodoPila* getCima() { return cima; }
        
        // Encolar
        void push(int numero, std::string nombre, std::string posicion);
        // mostrar cola
        void mostrarPila();
        // Desencolar
        int pop();

        // Esta vacia
        bool estaVacia();

        // Buscar la posicion en cola
        int peek();

        // GRaficar en Graphviz
        void graficar();

        // graficar pila para la lista de pilas
        void graficarPila(std::ofstream& archivo, const std::string& prefijo);
};

#endif