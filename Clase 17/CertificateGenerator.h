#ifndef CERTIFICATE_GENERATOR_H
#define CERTIFICATE_GENERATOR_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/stat.h>   // mkdir en Linux/Mac
#include <sys/types.h>

#ifdef _WIN32
    #include <direct.h>  // _mkdir en Windows
#endif

#include "HashFunction.h"
#include "Encryption.h"
#include "MerkleTree.h"



// Estructura simple para representar los datos necesarios de un lote.
// En tu sistema real, esto vendría directamente del nodo AVL.
struct DatosLote {
    std::string fecha;
    int codigoLote;
    std::string codigoFinca;
    std::string nombreFinca;
    std::string region;
    std::string tipoCafe;
    int sacos;
    std::string rutaTomada;
    float distanciaKm;
    std::string estado; // se espera "certificado_emitido" al momento de generar
};

class CertificateGenerator {
private:
    static const std::string CARPETA_SALIDA;

    // Crea la carpeta certificados/ si no existe (multiplataforma)
    static void asegurarCarpeta() {
        struct stat info;
        if (stat(CARPETA_SALIDA.c_str(), &info) != 0) {
            // No existe -> crearla
#ifdef _WIN32
            _mkdir(CARPETA_SALIDA.c_str());
#else
            mkdir(CARPETA_SALIDA.c_str(), 0755);
#endif
            std::cout << "[INFO] Carpeta '" << CARPETA_SALIDA << "/' creada.\n";
        }
    }

public:
    // ---------------------------------------------------------------
    // Construye el texto plano del certificado, EXACTAMENTE con el
    // formato especificado en el enunciado (pág. 8).
    // ---------------------------------------------------------------
    static std::string construirContenido(const DatosLote& lote) {
        std::string c;
        c += "CERTIFICADO DE ENTREGA - EDD COFFEETRACK\n";
        c += "==========================================\n";
        c += "Fecha : " + lote.fecha + "\n";
        c += "Codigo lote : " + std::to_string(lote.codigoLote) + "\n";
        c += "Finca : " + lote.nombreFinca + "\n";
        c += "Region : " + lote.region + "\n";
        c += "Tipo de cafe : " + lote.tipoCafe + "\n";
        c += "Sacos : " + std::to_string(lote.sacos) + "\n";
        c += "Ruta tomada : " + lote.rutaTomada + "\n";
        c += "Distancia : " + std::to_string(static_cast<int>(lote.distanciaKm)) + " km\n";
        c += "Estado final : " + lote.estado + "\n";
        return c;
    }

    // Resultado de generar un certificado: lo que el AVL necesita guardar
    struct ResultadoCertificado {
        std::string hashContenido;  // -> hash_certificado del nodo AVL
        std::string nombreArchivo;  // ej. "a3f9c2d1e8b047....txt"
        std::string rutaCompleta;   // ej. "certificados/a3f9c2d1e8b047....txt"
        bool exito;
        std::string mensajeError;
    };

    // ---------------------------------------------------------------
    // Genera el certificado .txt encriptado de UN lote y devuelve los
    // datos que hay que guardar en el nodo AVL.
    // NOTA: esta función NO toca el Árbol de Merkle; eso se hace afuera
    // (en el caso 4.1 inmediatamente, en el caso 4.2 al final de todos).
    // ---------------------------------------------------------------
    static ResultadoCertificado generarCertificado(const DatosLote& lote) {
        ResultadoCertificado resultado;
        resultado.exito = false;

        asegurarCarpeta();

        // 1. Contenido en texto plano
        std::string contenido = construirContenido(lote);

        // 2. hash_contenido (hoja del Merkle, se guarda en el AVL)
        resultado.hashContenido = HashFunction::hash(contenido);

        // 3. hash_nombre = hash del codigo_lote (NO del contenido)
        std::string hashNombre = HashFunction::hash(std::to_string(lote.codigoLote));

        // 4. Encriptar el contenido antes de escribirlo a disco
        std::string contenidoEncriptado = Encryption::encriptar(contenido);

        // 5. Construir nombre y ruta del archivo
        resultado.nombreArchivo = hashNombre + ".txt";
        resultado.rutaCompleta = CARPETA_SALIDA + "/" + resultado.nombreArchivo;

        // 6. Escribir el archivo en modo binario (importante: el texto
        //    encriptado puede contener bytes no imprimibles tras el XOR,
        //    así que NO se debe abrir en modo texto, para no perder datos)
        std::ofstream archivo(resultado.rutaCompleta, std::ios::binary);
        if (!archivo.is_open()) {
            resultado.mensajeError = "No se pudo crear el archivo: " + resultado.rutaCompleta;
            return resultado;
        }
        archivo.write(contenidoEncriptado.data(), contenidoEncriptado.size());
        archivo.close();

        resultado.exito = true;
        return resultado;
    }

    // ---------------------------------------------------------------
    // Lee un certificado .txt encriptado desde disco y devuelve su
    // contenido EN TEXTO PLANO (ya desencriptado). Útil para la opción
    // 4.3 (verificar integridad) y para depuración manual.
    // ---------------------------------------------------------------
    static std::string leerYDesencriptar(const std::string& rutaArchivo) {
        std::ifstream archivo(rutaArchivo, std::ios::binary);
        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + rutaArchivo);
        }

        std::string contenidoEncriptado((std::istreambuf_iterator<char>(archivo)),
                                          std::istreambuf_iterator<char>());
        archivo.close();

        return Encryption::desencriptar(contenidoEncriptado);
    }

    // ---------------------------------------------------------------
    // Opción 4.1 completa: genera UN certificado y actualiza el Merkle
    // inmediatamente (tal como pide el enunciado).
    // ---------------------------------------------------------------
    static ResultadoCertificado generarYActualizarMerkle(const DatosLote& lote, MerkleTree& merkle) {
        ResultadoCertificado resultado = generarCertificado(lote);
        if (resultado.exito) {
            merkle.agregarHoja(resultado.hashContenido);
        }
        return resultado;
    }

    // ---------------------------------------------------------------
    // Opción 4.2 completa: genera certificados para TODOS los lotes
    // de una fecha (recorrido in-order del AVL ya hecho afuera, aquí
    // solo se recibe el vector resultante) y reconstruye el Merkle
    // UNA SOLA VEZ al final con todos los hashes existentes + nuevos.
    //
    // 'hashesPrevios' son los hash_certificado de lotes que YA tenían
    // certificado antes de esta operación masiva (de otras fechas).
    // ---------------------------------------------------------------

    // TODO: Implementar la carga masiva con el json
    static std::vector<ResultadoCertificado> generarMasivoYActualizarMerkle(
            const std::vector<DatosLote>& lotesDelDia,
            MerkleTree& merkle,
            const std::vector<std::string>& hashesPrevios) {

        std::vector<ResultadoCertificado> resultados;
        std::vector<std::string> todosLosHashes = hashesPrevios;

        for (const auto& lote : lotesDelDia) {
            ResultadoCertificado r = generarCertificado(lote);
            resultados.push_back(r);
            if (r.exito) {
                todosLosHashes.push_back(r.hashContenido);
            }
        }

        // Reconstrucción única al final, más eficiente que una por una
        merkle.build(todosLosHashes);

        return resultados;
    }
};

const std::string CertificateGenerator::CARPETA_SALIDA = "certificados";

#endif