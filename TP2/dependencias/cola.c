#include "cola.h"
#include <stdlib.h>

typedef struct nodo {
	void* dato;
	struct nodo* siguiente;
} nodo_t;

struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
};

//Pre: -
//Post:	Crea un nodo con el dato indicado, inicializando el siguiente en NULL.
//		Devuelve NULL si falló la creación, sino un puntero al nodo.
static nodo_t* crear_nodo(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL)
		return NULL;

	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}

cola_t* cola_crear(){

	cola_t* cola = malloc(sizeof(cola_t));
	if(cola == NULL)
		return NULL;

	cola->primero = NULL;
	cola->ultimo = NULL;

	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	while(cola->primero){
		
		void* dato = cola_desencolar(cola);
		if(destruir_dato != NULL)
			destruir_dato(dato);

	}

	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){

	nodo_t* nodo = crear_nodo(valor);
	if(nodo == NULL)
		return false;

	if(cola->primero == NULL)
		cola->primero = nodo;

	if(cola->ultimo != NULL)
		cola->ultimo->siguiente = nodo;
	cola->ultimo = nodo;

	return true;
}

void *cola_ver_primero(const cola_t *cola){
	return( cola->primero == NULL ? NULL : cola->primero->dato);
}

void *cola_desencolar(cola_t *cola){
	
	void* dato = cola_ver_primero(cola);

	if(cola->ultimo == cola->primero)
		cola->ultimo = NULL;

	if(cola->primero){
		nodo_t* siguiente = cola->primero->siguiente;
		free(cola->primero);
		cola->primero = siguiente;
	}

	return dato;
}
