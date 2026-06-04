#include <iostream>
using namespace std;
#include "Estructuras/Cola/Cola.h"
#include "Estructuras/Pila/Pila.h"

int main() {
    // Crear una instancia de la clase Cola
    Cola cola;
    Pila pila;
    while (true)
    {
        cout << "Menu Cola" << endl;
        cout << "1. Encolar" << endl;
        cout << "2. Desencolar" << endl;
        cout << "3. Mostrar cola" << endl;
        cout << "4. Buscar en la cola" << endl;
        cout << "5. Salir" << endl;
        cout << "Pila: " << endl;
        cout << "6. Push" << endl;
        cout << "7. Pop" << endl;
        cout << "8. Mostrar pila" << endl;
        cout << "9. Peek" << endl;
        cout << "10. Graficar" << endl;
        cout << "Ingrese una opcion: ";
        int opcion;
        cin >> opcion;
        switch (opcion)
        {        
            case 1:{
                cout << "Encolar" << endl; 
                int dato;
                cin >> dato;
                cout << "Ingrese el nombre: ";
                string nombre;
                cin >> nombre;
                cout << "Ingrese la estatura: ";
                float estatura;
                cin >> estatura;

                cola.encolar(dato, nombre, estatura);
                system("cls");
                cout << "Dato encolado: " << dato << endl;
                break;
            }
            

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

            case 6:
            cout << "Push" << endl;
            int datoPush;
            cin >> datoPush;
            pila.push(datoPush);
            system("cls");
            cout << "Dato push: " << datoPush << endl;
            break;

            case 7:
            cout << "Pop" << endl;
            try
            {
                int datoPop = pila.pop();
                if (datoPop != -1)
                {
                    cout << "Dato pop: " << datoPop << endl;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            
            break;

            case 8:
            cout << "Mostrar pila" << endl; 
            try
            {
                pila.mostrarPila();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            break;

            case 9:
            cout << "Peek" << endl;
            try
            {
                int datoPeek = pila.peek();
                if (datoPeek != -1)
                {
                    cout << "Dato peek: " << datoPeek << endl;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            
            break;
            case 10:
            {
                cout << "Graficar" << endl;
                cola.graficar();
                pila.graficar();
                break;
            }
            default:
            cout << "Opcion invalida" << endl; 
        }
    }
    
    return 0;
}