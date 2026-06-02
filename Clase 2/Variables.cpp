#include <iostream>

int edad = 3;
std::string nombre = "Jorge";
float estatura = 1.75;
bool mayorDeEdad = true;

bool esMayorDeEdad() {
    return edad >= 18;
}

int main() {

    while(true){
        std::cout << "------------- Menu Ejemplo -------------------" << std::endl;
        std::cout << "1. Imprimir nombre" << std::endl;
        std::cout << "2. Imprimir edad" << std::endl;
        std::cout << "3. Imprimir estatura" << std::endl;
        std::cout << "4. Imprimir mayor de edad" << std::endl;
        std::cout << "5. Salir" << std::endl;
        std::cout << "6. Limpiar consola"<< std::endl;
        int opcion;
        std::cout << "Ingrese una opción: ";
        std::cin >> opcion;
        if(opcion == 1) {
            std::cout << "Mi nombre es: " << nombre << std::endl;
        } else if(opcion == 2) {
            std::cout << "Mi edad es: " << edad << " años" << std::endl;
        } else if(opcion == 3) {
            std::cout << "Mi estatura es: " << estatura << " metros" << std::endl;
        } else if(opcion == 4) {
            std::cout << "Soy mayor de edad: " << (esMayorDeEdad() ? "Sí" : "No") << std::endl;
        } else if(opcion == 5) {
            std::cout << "Saliendo del programa..." << std::endl;
            break;
        } 
        else if(opcion == 6) {
            system("cls");
        }
        else {
            std::cout << "Opción no válida. Por favor, intente de nuevo." << std::endl;
            system("cls");
            return -1;
            
        }
    }
    return 0;
}

