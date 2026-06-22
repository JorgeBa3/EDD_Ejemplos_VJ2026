#include <iostream>
#include <fstream>
#include "HashFunction.h"
#include "Encryption.h"
#include "MerkleTree.h"
#include "CertificateGenerator.h"
#include "MerkleGraphvizReport.h"


int main() {
    std::cout << "=== EDD COFFEETRACK - Demo Certificados + Merkle ===\n\n";

    MerkleTree merkle;

    // --------------------------------------------------------------
    // Datos de ejemplo (equivalentes a los lotes del JSON del enunciado)
    // --------------------------------------------------------------
    DatosLote lote1;
    lote1.fecha = "2026-06-15";
    lote1.codigoLote = 1;
    lote1.codigoFinca = "F001";
    lote1.nombreFinca = "Finca La Hermosa";
    lote1.region = "Huehuetenango";
    lote1.tipoCafe = "Bourbon";
    lote1.sacos = 45;
    lote1.rutaTomada = "F001 -> F002 -> BENEFICIO";
    lote1.distanciaKm = 172.0f;
    lote1.estado = "certificado_emitido";

    DatosLote lote2;
    lote2.fecha = "2026-06-15";
    lote2.codigoLote = 2;
    lote2.codigoFinca = "F002";
    lote2.nombreFinca = "Finca El Roble";
    lote2.region = "San Marcos";
    lote2.tipoCafe = "Caturra";
    lote2.sacos = 30;
    lote2.rutaTomada = "F002 -> BENEFICIO";
    lote2.distanciaKm = 98.0f;
    lote2.estado = "certificado_emitido";

    DatosLote lote3;
    lote3.fecha = "2026-06-20";
    lote3.codigoLote = 3;
    lote3.codigoFinca = "F003";
    lote3.nombreFinca = "Finca Las Nubes";
    lote3.region = "Alta Verapaz";
    lote3.tipoCafe = "Gesha";
    lote3.sacos = 55;
    lote3.rutaTomada = "F003 -> F002 -> BENEFICIO";
    lote3.distanciaKm = 158.0f;
    lote3.estado = "certificado_emitido";

    // --------------------------------------------------------------
    // OPCIÓN 4.1 — Generar certificado de UN lote a la vez
    // --------------------------------------------------------------
    std::cout << "--- Opcion 4.1: generar certificado individual ---\n";

    auto r1 = CertificateGenerator::generarYActualizarMerkle(lote1, merkle);
    std::cout << "Lote 1 -> archivo: " << r1.nombreArchivo
              << " | hash_certificado: " << r1.hashContenido << "\n";

    auto r2 = CertificateGenerator::generarYActualizarMerkle(lote2, merkle);
    std::cout << "Lote 2 -> archivo: " << r2.nombreArchivo
              << " | hash_certificado: " << r2.hashContenido << "\n";

    std::cout << "Raiz Merkle tras 2 certificados: " << merkle.getRoot() << "\n\n";

    // --------------------------------------------------------------
    // OPCIÓN 4.2 — Generar certificados masivos (resto del lote, lote 3)
    // --------------------------------------------------------------
    std::cout << "--- Opcion 4.2: generar certificados masivos ---\n";

    std::vector<DatosLote> lotesDelDia = { lote3 };
    std::vector<std::string> hashesPrevios = { r1.hashContenido, r2.hashContenido };

    auto resultadosMasivos = CertificateGenerator::generarMasivoYActualizarMerkle(
        lotesDelDia, merkle, hashesPrevios);

    for (auto& r : resultadosMasivos) {
        std::cout << "Lote generado -> archivo: " << r.nombreArchivo
                  << " | hash_certificado: " << r.hashContenido << "\n";
    }
    std::cout << "Raiz Merkle tras generacion masiva: " << merkle.getRoot() << "\n\n";

    // --------------------------------------------------------------
    // OPCIÓN 4.3 — Verificar integridad: leer el .txt real desde disco,
    // desencriptarlo, recalcular su hash y compararlo contra el Merkle.
    // --------------------------------------------------------------
    std::cout << "--- Opcion 4.3: verificar integridad (leyendo desde disco) ---\n";

    std::string contenidoLeido = CertificateGenerator::leerYDesencriptar(r2.rutaCompleta);
    std::cout << "Contenido desencriptado del archivo " << r2.nombreArchivo << ":\n";
    std::cout << contenidoLeido << "\n";

    std::string hashRecalculado = HashFunction::hash(contenidoLeido);
    std::cout << "Hash recalculado desde el archivo: " << hashRecalculado << "\n";
    std::cout << "Hash original (hash_certificado):   " << r2.hashContenido << "\n";

    bool valido = merkle.verificarCertificado(hashRecalculado);
    std::cout << "Resultado: " << (valido ? "CERTIFICADO INTACTO (valido)" : "CERTIFICADO ALTERADO") << "\n\n";

    // --------------------------------------------------------------
    // Simular alteración directa del archivo en disco (ataque) y
    // detectar la alteración con el mismo flujo de verificación.
    // --------------------------------------------------------------
    std::cout << "--- Simulando alteracion directa del archivo en disco ---\n";

    // Leemos, modificamos el contenido en texto plano, y lo re-escribimos
    // encriptado (como si alguien hubiera editado el certificado y vuelto
    // a guardarlo, conociendo el esquema de encriptacion).
    std::string contenidoModificado = contenidoLeido;
    size_t pos = contenidoModificado.find("Sacos : 30");
    if (pos != std::string::npos) {
        contenidoModificado.replace(pos, std::string("Sacos : 30").size(), "Sacos : 999");
    }

    std::string nuevoEncriptado = Encryption::encriptar(contenidoModificado);
    std::ofstream archivoModificado(r2.rutaCompleta, std::ios::binary);
    archivoModificado.write(nuevoEncriptado.data(), nuevoEncriptado.size());
    archivoModificado.close();

    std::string contenidoTrasAtaque = CertificateGenerator::leerYDesencriptar(r2.rutaCompleta);
    std::string hashTrasAtaque = HashFunction::hash(contenidoTrasAtaque);

    std::cout << "Hash tras alteracion: " << hashTrasAtaque << "\n";
    bool validoTrasAtaque = merkle.verificarCertificado(hashTrasAtaque);
    std::cout << "Resultado: " << (validoTrasAtaque ? "CERTIFICADO INTACTO (ERROR, no deberia pasar)" : "CERTIFICADO ALTERADO (correcto, detectado)") << "\n\n";

    // --------------------------------------------------------------
    // OPCIÓN 5.3 — Mostrar hash raíz actual
    // --------------------------------------------------------------
    std::cout << "--- Opcion 5.3: hash raiz actual ---\n";
    std::cout << "Raiz: " << merkle.getRoot() << "\n";
    std::cout << "Certificados cubiertos: " << merkle.getNumCertificados() << "\n\n";

    // --------------------------------------------------------------
    // OPCIÓN 6.4 — Graficar Árbol de Merkle con Graphviz
    // --------------------------------------------------------------
    int datoBuscar;
    std::cin >> datoBuscar;
    std::cout << "--- Opcion 6.4: graficar Arbol de Merkle (Graphviz) ---\n";
    MerkleGraphvizReport::generarReporte(merkle, "reportes", "merkle_tree");

    // Prueba a mano, cambio en el contenido hasheado
    std::string contenido = CertificateGenerator::leerYDesencriptar("certificados/2a070f8d71f6246bffd284e2baef3c4c.txt");
    std::cout << contenido << std::endl;
    return 0;
}