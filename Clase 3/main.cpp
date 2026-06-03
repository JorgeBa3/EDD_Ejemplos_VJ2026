#include <iostream>
using namespace std;
#include "Estructuras/Cola/Cola.h"

int main() {
    // Crear una instancia de la clase Cola
    Cola cola;
    while (true)
    {
        cout << "Menu Cola" << endl;
        cout << "1. Encolar" << endl;
        cout << "2. Desencolar" << endl;
        cout << "3. Mostrar cola" << endl;
        cout << "4. Buscar en la cola" << endl;
        cout << "5. Salir" << endl;
        int opcion;
        cin >> opcion;
        switch (opcion)
        {        
            case 1:
            cout << "Encolar" << endl; 
            int dato;
            cin >> dato;
            cola.encolar(dato);
            system("cls");
            cout << "Dato encolado: " << dato << endl;
            break;

            case 2:
            cout << "Desencolar" << endl;
            cola.desencolar();
            break;
            case 3:
            cout << "Mostrar cola" << endl; 
            cola.mostrarCola();
            break;
            
            case 4:
            cout << "Buscar en la cola" << endl;
            int datoBuscar;
            cin >> datoBuscar;
            try
            {
                int posicion = cola.buscar(datoBuscar);
                if (posicion != -1)
                {
                    cout << "El dato se encuentra en la posicion: " << posicion << endl;
                }
                else
                {
                    cout << "El dato no se encuentra en la cola" << endl;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            
            
            break;


            case 5:
            cout << "Saliendo..." << endl; 
            return 0;
            default:
            cout << "Opcion invalida" << endl; 
        }
    }
    
    return 0;
}