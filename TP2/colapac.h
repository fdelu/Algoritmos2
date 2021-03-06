#ifndef COLAPAC__H
#define COLAPAC__H

#include <stddef.h>
#include <stdbool.h>
#include "paciente.h"

/* ******************************************************************
 *                     TDA COLA DE PACIENTES
 * *****************************************************************/

typedef struct colapac colapac_t;


// Crea una cola de pacientes, devuelve NULL si falla.
// Se debe destruir con colapac_destruir()
colapac_t* colapac_crear();

// Agrega una cola de la especialidad indicada.
// Devuelve verdadero si se logró o ya existía, sino falso.
bool colapac_agregar(colapac_t* colapac, const char* especialidad);

// Devuelve verdadero si hay una cola de la especialdiad indicada, sino falso.
bool colapac_existe(colapac_t* colapac, const char* especialidad);

// Encola un paciente con la especialidad y urgencia dada.
// Devuelve false si falla (no hay cola de esa especialidad o fallo de memoria),
// o true si se logra encolar.
bool colapac_encolar(colapac_t* colapac, paciente_t* paciente, const char* especialidad, bool urgente);

// Desencola un paciente de la especialidad dada. Devuelve el nombre del paciente o NULL en caso
// de no haber pacientes encolados para esa especialidad o que no haya cola de esa especialidad.
paciente_t* colapac_desencolar(colapac_t* colapac, const char* especialidad);

// Devuelve la cantidad de pacientes en espera para esa especialidad.
// Devuelve 0 si la especialidad no existe.
size_t colapac_cantidad(colapac_t* colapac, const char* especialidad);

// Destruye la cola de pacientes
void colapac_destruir(colapac_t* colapac);

#endif // COLAPAC_H