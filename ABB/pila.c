#include "pila.h"

#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

// Constantes para las funciones, deben ser mayores a 0.
#define CAPACIDAD_INICIAL 10
#define FACTOR_REDIMENSION 2
#define FRACCION_ACHICAR 4

//Pre: Pila ya creada
//Post: Devuelve verdadero si la pila cumple las condiciones para disminuir su capacidad. Estas son:
// 1. La cantidad de elementos de la pila  es FRACCION_ACHICAR veces menor que su capacidad.
// 2. La nueva capacidad (la actual dividido FACTOR_REDIMENSION) es mayor a CAPACIDAD_INICIAL.
bool puede_achicar(pila_t* pila) {
	return (pila->cantidad * FRACCION_ACHICAR <= pila->capacidad) && (pila->capacidad/FACTOR_REDIMENSION > CAPACIDAD_INICIAL);
}

//Pre: Pila ya creada
//Post: Devuelve verdadero si se pudo redimensionar la capacidad de datos de la pila a nueva_capacidad, sino falso y deja todo como está.
bool redimensionar_pila(pila_t* pila, size_t nueva_capacidad) {
	void** nuevos_datos = realloc(pila->datos, nueva_capacidad * sizeof(void*));

	if (nueva_capacidad && !nuevos_datos) {
		return false;
	}

	pila->capacidad = nueva_capacidad;
	pila->datos = nuevos_datos;
	return true;
}

//La capacidad de las nuevas pilas es CAPACIDAD_INICIAL.
pila_t *pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));

	if (!pila)
	    return NULL;

	pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

	if (!pila->datos){
	    free(pila);
	    return NULL;
	}

	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INICIAL;
	return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

//Si la pila esta llena, multiplica su capacidad por FACTOR_REDIMENSION. Si no lo logra, devuelve falso.
bool pila_apilar(pila_t *pila, void *valor) {
	if ( (pila->cantidad == pila->capacidad) && !redimensionar_pila(pila, FACTOR_REDIMENSION * pila->capacidad) ) {
		return false;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_ver_tope(const pila_t *pila) {
	if (pila->cantidad <= 0) {
		return NULL;
	}

	return pila->datos[pila->cantidad - 1];
}

//Si se cumplen las condiciones para achicar la capacidad dadas por puede_achicar(), se intenta dividir la capacidad en FACTOR_REDIMENSION.
void *pila_desapilar(pila_t *pila){
	void* tope = pila_ver_tope(pila);
	if (pila->cantidad > 0)
		pila->cantidad--;

	if (puede_achicar(pila))
		redimensionar_pila(pila, pila->capacidad/FACTOR_REDIMENSION);

	return tope;
}