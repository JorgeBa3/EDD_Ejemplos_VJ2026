#include "Estructuras/ListaSimple/Lista.h"

int main() {
    ListaSimple lista;

    lista.insertarFinal("Brasil");
    lista.insertarJugador("Brasil", 9, "Neymar", "Delantero"); // Neymar
    lista.insertarJugador("Brasil", 10, "Pelé", "Delantero"); // Pelé

    lista.insertarFinal ("Argentina");
    lista.insertarJugador("Argentina", 10, "Messi", "Delantero"); // Messi
    lista.insertarJugador("Argentina", 11, "Di María", "Delantero"); // Di María

    lista.insertarFinal ("Guatemala");
    lista.insertarJugador("Guatemala", 10, "Roderio", "Delantero"); // Roderio

    lista.mostrarLista();
    lista.graficar();
    return 0;
}