#include <iostream>
#include "Estructuras/AVL/AVL.h"
using namespace std;
int main(){
    AVL arbol2;
    while (true)
    {
        cout << "Arbol avl" << endl;
        cout << "1. Agregar nodo" << endl;
        cout << "2. Graficar" << endl;
        cout << "3. Salir" << endl;
        int opcion;
        cin >> opcion;  
        if (opcion ==1)
        {
            int nodo;
            cin >> nodo;  
            arbol2.insertar(nodo);
        }else if (opcion ==2)
        {            
            arbol2.inorden();
            arbol2.graficar();
        }else if (opcion ==3)
        {            
            break;
        }
        
        
    }
        

    return 0;
}