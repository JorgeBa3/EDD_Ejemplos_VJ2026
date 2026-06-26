#include <iostream>
#include "Estructuras/BST/BST.h"
#include "Estructuras/AVL/AVL.h"

int main(){
    BST arbol;
    AVL arbol2;
    arbol2.insertar(30);
    arbol2.insertar(20);
    arbol2.insertar(10);
    
    arbol2.inorden();
    arbol2.preorden();
    arbol2.postorden();
    arbol2.graficar();
    return 0;
}