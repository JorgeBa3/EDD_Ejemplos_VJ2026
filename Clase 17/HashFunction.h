#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>

/*
 * ============================================================================
 *  HashFunction — Función de hash propia para EDD CoffeeTrack Fase 2
 * ============================================================================
 *
 *  JUSTIFICACIÓN (para el README):
 *  El enunciado permite implementar "una versión simplificada de SHA-256 o
 *  una función propia documentada", con el único requisito de que sea:
 *
 *    1. Determinista: la misma entrada SIEMPRE produce la misma salida.
 *    2. Salida de longitud fija >= 32 caracteres hexadecimales.
 *    3. Suficientemente distribuida para evitar colisiones en la práctica.
 *
 *  Esta implementación usa el algoritmo FNV-1a (Fowler–Noll–Vo) combinado
 *  con un paso de mezcla final tipo "avalancha" (inspirado en el
 *  finalizador de MurmurHash3). FNV-1a es muy simple de auditar línea por
 *  línea (importante para la defensa) y tiene buena distribución para
 *  textos largos, pero difunde mal con entradas muy cortas (ej. un
 *  codigo_lote como "1" o "2" produce hashes parecidos entre sí). El paso
 *  de avalancha corrige esa debilidad: garantiza que cualquier cambio en
 *  la entrada, sin importar lo pequeño, se propague a todos los bits de
 *  salida.
 *
 *  Para cumplir el requisito de 32+ caracteres hexadecimales, se ejecutan
 *  DOS pasadas completas (FNV-1a + avalancha) con semillas (offset basis)
 *  distintas, generando dos hashes de 64 bits (16 hex c/u) que se
 *  concatenan -> 32 hex chars. Esto reduce drásticamente la probabilidad
 *  de colisión respecto a usar un solo hash de 64 bits, similar en
 *  espíritu a como SHA-256 combina múltiples rondas para ampliar el
 *  espacio de salida.
 *
 *  NOTA: No es criptográficamente segura (no resiste ataques deliberados),
 *  pero cumple el propósito académico del proyecto: detectar alteraciones
 *  accidentales o intencionales no sofisticadas en los certificados.
 * ============================================================================
 */

class HashFunction {
private:
    // Constantes estándar de FNV-1a de 64 bits
    static const uint64_t FNV_PRIME = 1099511628211ULL;
    static const uint64_t FNV_OFFSET_1 = 14695981039346656037ULL; // offset estándar
    static const uint64_t FNV_OFFSET_2 = 9223372036854775837ULL;  // offset alterno (primo cercano a 2^63)

    // Una pasada de FNV-1a con un offset basis dado
    static uint64_t fnv1a(const std::string& data, uint64_t offsetBasis) {
        uint64_t h = offsetBasis;
        for (unsigned char c : data) {
            h ^= static_cast<uint64_t>(c);
            h *= FNV_PRIME;
        }
        return h;
    }

    // Paso de mezcla final ("avalancha"), inspirado en el finalizador de
    // MurmurHash3. Garantiza que un cambio pequeño en la entrada (incluso
    // un solo caracter, como "1" vs "2") se difunda en TODOS los bits de
    // salida, corrigiendo la debilidad de FNV-1a puro con entradas cortas.
    static uint64_t avalancha(uint64_t h) {
        h ^= h >> 33;
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 33;
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= h >> 33;
        return h;
    }

    static std::string toHex16(uint64_t value) {
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << value;
        return ss.str();
    }

public:
    // Función principal: recibe cualquier string y devuelve 32 caracteres hex
    //
    // NOTA SOBRE ENTRADAS CORTAS: FNV-1a difunde mal cuando el input tiene
    // muy pocos bytes (ej. un codigo_lote como "1" o "2"), porque el hash
    // apenas alcanza a mezclar el estado interno. Para evitarlo, antes de
    // hashear se antepone un "salt" fijo y se incluye la longitud original
    // del dato; esto obliga a que SIEMPRE se procesen suficientes bytes
    // para que ambas pasadas (h1, h2) difundan bien, sin afectar el
    // determinismo (mismo input -> mismo salt -> mismo resultado siempre).


    // TODO: Agregar al enuncuado que todos tengan la misma entrada 
    static std::string hash(const std::string& data) {
        std::string entradaFortalecida = "EDDCOFFEETRACK:" + std::to_string(data.size()) + ":" + data;
        uint64_t h1 = avalancha(fnv1a(entradaFortalecida, FNV_OFFSET_1));
        uint64_t h2 = avalancha(fnv1a(entradaFortalecida, FNV_OFFSET_2));
        return toHex16(h1) + toHex16(h2); // 16 + 16 = 32 caracteres hex
    }
};

#endif