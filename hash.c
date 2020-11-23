#include "hash.h"
#include <stdlib.h>


/* ******************************************************************
 *                  DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

#define CAPACIDAD_INICIAL 10
#define PROP_AGRANDAR 0.7
#define PROP_ACHICAR 0.15
#define FACTOR_REDIM 2

enum estados {
	VACIO,
	BORRADO,
	OCUPADO
};

typedef struct elemento {
	char* clave;
	void* dato;
	enum estados estado;
} elem_t;

struct hash {
	elem_t* elementos;
	size_t cap;
	size_t cant;
	hash_destruir_dato_t function_destruir;
};

struct hash_iter {
	hash_t* hash;
	size_t pos;
};

/* ******************************************************************
 *                        FUNCIONES INTERNAS
 * *****************************************************************/

// Función de hash
// (djb2, http://www.cse.yorku.ca/~oz/hash.html)
// Tiene algunas modificaciones para comodidad y
// para que no tire warnings el compilador.
size_t hash_func(char *str, size_t cap){
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */

    return hash % cap;
}

// Inicializa esa cantidad de elementos del vector elementos,
// poniendo su estado en VACIO.
void inicializar_elementos(elem_t* elementos, size_t cant){
	for(int i = 0; i < cant; i++){
		elementos[i].estado = VACIO;
	}
}

// Devuelve una nueva capacidad ideal para ese hash de acuerdo a su carga,
// según la proporción de la cantidad de elementos con la capacidad del hash:
// - Si es menor a PROP_ACHICAR, devuelve la capacidad actual
//   del hash dividido FACTOR_REDIM.
// - Si es mayor a PROP_AGRANDAR, devuelve la capacidad actual
//   del hash multiplicado por FACTOR_REDIM.
// - Sino, devuelve la capacidad actual del hash.
size_t nueva_cap(const hash_t* hash){
	size_t cap = hash->cap;
	float proporcion = (float)(hash->cant / hash->cap);

	if(proporcion < PROP_ACHICAR){
		cap = hash->cap/FACTOR_REDIM;
		if(cap < CAPACIDAD_INICIAL)
			cap = CAPACIDAD_INICIAL;
	}

	if(proporcion > PROP_AGRANDAR){
		cap = hash->cap * FACTOR_REDIM;
	}

	return cap;
}

bool redim_hash(hash_t* hash){
	size_t cap = nueva_cap(hash);
	if(cap == hash->cap)
		return true;

	//hago cosas
	return true;
}

bool es_valido_guardar(elem_t* elemento, char* extra){
	if(elemento->estado == VACIO || elemento->estado == BORRADO) return true;
	return false;
}
bool es_elemento_buscado(elem_t* elemento, char* extra){
	if(elemento->clave == extra) return true;
	return false;
}

/* ******************************************************************
 *                         PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	elem_t* elementos = malloc(sizeof(elem_t)*CAPACIDAD_INICIAL);
	if(!elementos){
		free(hash);
		return NULL;
	}

	inicializar_elementos(elementos, CAPACIDAD_INICIAL);
	hash->cap = CAPACIDAD_INICIAL;
	hash->cant = 0;
	hash->function_destruir = destruir_dato;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = size_t buscar_elemento(hash->elementos, hash->cap,pos_ini, es_valido_guardar,NULL);
	if(pos_final == hash->cap) return false;
	elem_t *elemento = hash->elementos+pos_final;
	elemento->clave = clave;
	elemento->dato = dato;
	elemento-> estado = OCUPADO;
	hash->cant++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = size_t buscar_elemento(hash->elementos, hash->cap,pos_ini, es_elemento_buscado,clave);
	if(pos_final == hash->cap) return NULL;
	elem_t *elemento = hash->elementos+pos_final;
	elemento->clave = NULL;
	elemento-> estado = BORRADO;
	hash->cant--;
	return elemento->dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = size_t buscar_elemento(hash->elementos, hash->cap,pos_ini, es_elemento_buscado,clave);
	if(pos_final == hash->cap) return NULL;
	elem_t *elemento = hash->elementos+pos_final;
	return elemento->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	ize_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = size_t buscar_elemento(hash->elementos, hash->cap,pos_ini, es_elemento_buscado,clave);
	if(pos_final == hash->cap) return false;
	elem_t *elemento = hash->elementos+pos_final;
	if(elemento->estado != OCUPADO) return false;
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t *hash){
	elem_t* elementos = hash->elementos;
	
	for(size_t i = 0, i < hash->cap, i++){
		if(elementos[i]->estado != OCUPADO) continue;

	}
	return;
}

/* ******************************************************************
 *                       PRIMITIVAS ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter)
		return NULL;

	iter->pos = 0;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	return NULL;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return true;
}

void hash_iter_destruir(hash_iter_t *iter){
	return;
}
