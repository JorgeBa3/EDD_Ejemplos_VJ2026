#include <iostream>

int arreglo[] = {5,20,3,8,15,9,10,2};

// Función para imprimir el arreglo
void imprimir(){
    std::cout << "[" ;
    for (int i = 0; i < 8; i++)
    {
        std::cout << arreglo[i] << ", ";
    }
    std::cout << "]" << std::endl ;
}

void burbuja(int arreglo[], int n){
    for (int i = 0; i < n; i++)
    {
        bool banderaCambio = false;
        for (int j = 0; j < n -i -1; j++)
        {
            if (arreglo[j]>arreglo[j+1])
            {
                int temp = arreglo[j];
                arreglo[j]= arreglo[j+1];
                arreglo[j+1] = temp;
                banderaCambio = true;
                imprimir();
            }
        }
        if (!banderaCambio) break;
    }
    
}

int busquedaLineal(int arreglo[], int n, int objetivo){
    for (int i = 0; i < n; i++)
    {
        if(arreglo[i] == objetivo) return i;
    }
    return -1;
}

int busquedaBinaria(int arrelog[], int n, int objetivo){
    int izq = 0, der = n-1;
    while (izq <= der)
    {
        int medio = (izq + der)/2;
        if (arreglo[medio] == objetivo)
        {
            return medio;
        }
        else if (arreglo[medio] < objetivo)
        {
            izq = medio + 1;
        }else{
            der = medio - 1;
        }
        
        
    }
    return -1;
}

int main(){
    int tamaño = 8;
    imprimir();
    std::cout << "Ordenamiento burbuja" << std::endl;
    burbuja(arreglo, tamaño);
    imprimir();

    std::cout << "Busqueda lineal" << std::endl;
    std::cout << "Buscar 15" << std::endl;
    std::cout << "El 15 se encuentra en la posición: " << busquedaLineal(arreglo, tamaño, 15)
    << "del arreglo "<< std::endl;
    std::cout << "Buscar 15 busqueda binaria" << std::endl;
    std::cout << "El 15 se encuentra en la posición: " << busquedaBinaria(arreglo, tamaño, 15)
    << "del arreglo "<< std::endl;
    return 0;
}