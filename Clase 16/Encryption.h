#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <cstdint>


class Encryption {
private:
    static const std::string CLAVE; // clave fija del sistema, ver .cpp o aquí mismo

public:
    // Aplica/revierte el cifrado XOR (la misma función hace ambas operaciones)
    // 0 0 | 0
    // 0 1 | 1
    // 1 0 | 1
    // 1 1 | 0
    static std::string xorCifrar(const std::string& texto, const std::string& clave) {
        std::string resultado = texto;
        for (size_t i = 0; i < texto.size(); ++i) {
            resultado[i] = texto[i] ^ clave[i % clave.size()];
        }
        return resultado;
    }

    // Encripta usando la clave por defecto del sistema
    static std::string encriptar(const std::string& textoPlano) {
        return xorCifrar(textoPlano, CLAVE);
    }

    // Desencripta (idéntico a encriptar, por la propiedad simétrica de XOR)
    static std::string desencriptar(const std::string& textoCifrado) {
        return xorCifrar(textoCifrado, CLAVE);
    }
};

// Definición de la clave (puede moverse a un .cpp si se prefiere separar)
inline const std::string Encryption::CLAVE = "CoffeeTrack2026EDD";

#endif