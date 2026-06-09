#ifndef COLA_H
#define COLA_H
#include <iostream>
#include <string>
#include <fstream>
struct NodoCola
{
    int dato;
    std::string nombre;
    float estatura;
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
        void encolar(int dato, std::string nombre, float estatura);
        // mostrar cola
        void mostrarCola();
        // Desencolar
        void desencolar();

        // Esta vacia
        bool estaVacia();

        // Buscar la posicion en cola
        int buscar(int dato);

        // GRaficar en Graphviz
        void graficar();

        void cargaMasiva(std::string nombreArchivo);
};

#endif