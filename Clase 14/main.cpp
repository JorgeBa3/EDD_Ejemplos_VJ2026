#include <iostream>
#include "Estructuras/ArbolB/ArbolB.h"
using namespace std;

int main(){
    ArbolB arbolb(2); // grado minimo 2 -> tipo 2-3-4
    while (true)
    {
        cout << "Arbol B" << endl;
        cout << "1. Agregar nodo" << endl;
        cout << "2. Graficar" << endl;
        cout << "3. Buscar" << endl;
        cout << "4. Salir" << endl;
        int opcion;
        cin >> opcion;
        if (opcion == 1)
        {
            int valor;
            cin >> valor;
            arbolb.insertar(valor);
        }
        else if (opcion == 2)
        {
            arbolb.inorden();
            arbolb.graficar();
        }
        else if (opcion == 3)
        {
            int valor;
            cin >> valor;
            cout << (arbolb.buscar(valor) ? "Encontrado" : "No encontrado") << endl;
        }
        else if (opcion == 4)
        {
            break;
        }
    }

    return 0;
}