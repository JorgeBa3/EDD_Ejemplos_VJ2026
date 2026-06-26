#include <iostream>
#include "Estructuras/BST/BST.h"

int main()
{
    BST arbol;

    arbol.insertar(50);
    arbol.insertar(30);
    arbol.insertar(70);
    arbol.insertar(20);
    arbol.insertar(40);
    
    arbol.insertar(60);
    arbol.insertar(65);
    arbol.insertar(85);
    arbol.insertar(80);
    arbol.insertar(66);

    std::cout << "Inorden   (ascendente):    "; arbol.inorden();
    std::cout << "Preorden  (raiz primero):  "; arbol.preorden();
    std::cout << "Postorden (raiz al final): "; arbol.postorden();

    arbol.graficar();

    return 0;
}