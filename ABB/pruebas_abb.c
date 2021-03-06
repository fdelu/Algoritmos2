/*
 * prueba_hash.c
 * Pruebas para el tipo de dato abstracto Tabla de Hash
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 (Modificado para ABB)
 */

#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#define LARGO_CLAVE 10

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio(abb_comparar_clave_t cmp) {
    abb_t* abb = abb_crear(cmp, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio() {
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_borrar(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null(abb_comparar_clave_t cmp)
{
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

 int inv_strcmp(const char *s1, const char *s2) {
 	return -1 * strcmp(s1, s2);
 }


//llena el arbol con fin-inicio elementos de forma tal que quede balanceado
//si inicializar es true, inicializa los arreglos de valores y claves mientras llena el arbol
bool llenar_arbol_balanceado(abb_t* abb, size_t** valores, char (*claves)[LARGO_CLAVE], size_t inicio, size_t fin, bool inicializar) {
	if(inicio >= fin)
		return true;

	size_t medio = (inicio+fin)/2;
	if(inicializar) {
		valores[medio] = malloc(sizeof(size_t));
		sprintf(claves[medio], "%08lu", medio);
		*(valores[medio]) = medio;
	}
	bool ok = abb_guardar(abb, claves[medio], valores[medio]);

	if(!ok) return false;
	return llenar_arbol_balanceado(abb, valores, claves, inicio, medio, inicializar) && 
		llenar_arbol_balanceado(abb, valores, claves, medio+1, fin, inicializar);
}

static void prueba_abb_volumen(size_t largo) {
    abb_t* abb = abb_crear(strcmp, free);

    char (*claves)[LARGO_CLAVE] = malloc(largo * LARGO_CLAVE);

    size_t* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = llenar_arbol_balanceado(abb, valores, claves, 0, largo, true);

    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    print_test("Prueba abb borrar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Destruye el abb y crea uno nuevo que sí libera */
    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    /* Inserta 'largo' parejas en el abb */
    ok = llenar_arbol_balanceado(abb, valores, claves, 0, largo, false);

    free(claves);

    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb);
}


static void prueba_abb_iterar() {
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    //Inserta 3 valores 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    char *valor;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    // Primer valor
    clave = abb_iter_in_ver_actual(iter);
    valor = (char*)abb_obtener(abb, clave);
    print_test("Prueba abb iterador ver actual, es una clave valida", abb_pertenece(abb, clave));
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[1]);
    print_test("el valor coincide con la clave", strcmp(valores[1], valor) == 0 );
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    // Segundo valor
    clave = abb_iter_in_ver_actual(iter);
    valor = (char*)abb_obtener(abb, clave);
    print_test("Prueba abb iterador ver actual, es una clave valida", abb_pertenece(abb, clave));
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[0]);
    print_test("el valor coincide con la clave", strcmp(valores[0], valor) == 0 );
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    // Tercer valor
    clave = abb_iter_in_ver_actual(iter);
    valor = (char*)abb_obtener(abb, clave);
    print_test("Prueba abb iterador ver actual, es una clave valida", abb_pertenece(abb, clave));
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[2]);
    print_test("el valor coincide con la clave", strcmp(valores[2], valor) == 0 );
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    // Vuelve a tratar de avanzar, por las dudas
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo) {
    abb_t* abb = abb_crear(strcmp, free);

    char (*claves)[LARGO_CLAVE] = malloc(largo * LARGO_CLAVE);

    size_t* valores[largo];

    // Inserta 'largo' parejas en el abb 
    bool ok = llenar_arbol_balanceado(abb, valores, claves, 0, largo, true);

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    size_t i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( *valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void prueba_orden_strcmp() {
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_guardar(abb, "a", NULL);
    abb_guardar(abb, "b", NULL);
    abb_guardar(abb, "c", NULL);

    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 1 (strcmp)", strcmp(clave, "a") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 2 (strcmp)", strcmp(clave, "b") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 3 (strcmp)", strcmp(clave, "c") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("iter al final", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void prueba_orden_otra() {
    abb_t* abb = abb_crear(inv_strcmp, NULL);
    abb_guardar(abb, "a", NULL);
    abb_guardar(abb, "b", NULL);
    abb_guardar(abb, "c", NULL);

    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 1 (otra)", strcmp(clave, "c") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 2 (otra)", strcmp(clave, "b") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("prueba orden correcto 3 (otra)", strcmp(clave, "a") == 0);
    abb_iter_in_avanzar(iter);
    clave = abb_iter_in_ver_actual(iter);
    print_test("iter al final", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

bool prueba_visitar(const char *clave, void *valor, void *n) {
    if(*((size_t*) valor) == 4) return false;
    *((size_t*) n) += 1;
    *((size_t*) valor) += *((size_t*) n);;
    return true;
}

void prueba_iterador_interno() {
    abb_t* abb = abb_crear(strcmp, NULL);
    size_t val1 = 1;
    size_t val2 = 2;
    size_t val3 = 3;
    size_t val4 = 4;
    size_t val5 = 5;
    abb_guardar(abb, "b", &val2);
    abb_guardar(abb, "a", &val1);
    abb_guardar(abb, "e", &val5);
    abb_guardar(abb, "d", &val4);
    abb_guardar(abb, "c", &val3);
    size_t n = 0;
    abb_in_order(abb, prueba_visitar, &n);
    print_test("prueba visitar", val1 == 2 && val2 == 4 && val3 == 6 && val4 == 4 && val5 == 5);

    abb_destruir(abb);
}
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_estudiante() {
    /* Ejecuta todas las pruebas unitarias. */
    printf("\n\nPruebas con strcmp()\n");
    prueba_crear_abb_vacio(strcmp);
    prueba_abb_insertar(strcmp);
    prueba_abb_reemplazar(strcmp);
    prueba_abb_reemplazar_con_destruir(strcmp);
    prueba_abb_borrar(strcmp);
    prueba_abb_clave_vacia(strcmp);
    prueba_abb_valor_null(strcmp);
    

    printf("\n\nPruebas con otra función de comparación\n");
    prueba_crear_abb_vacio(inv_strcmp);
    prueba_abb_insertar(inv_strcmp);
    prueba_abb_reemplazar(inv_strcmp);
    prueba_abb_reemplazar_con_destruir(inv_strcmp);
    prueba_abb_borrar(inv_strcmp);
    prueba_abb_clave_vacia(inv_strcmp);
    prueba_abb_valor_null(inv_strcmp);

    printf("\n\nPruebas iterador\n");
    prueba_iterar_abb_vacio();
    prueba_abb_iterar();
    prueba_orden_strcmp();
    prueba_orden_otra();

    printf("\n\nPruebas iterador interno\n");
    prueba_iterador_interno();

    printf("\n\nPruebas de volumen\n");
    prueba_abb_volumen(5000);
    prueba_abb_iterar_volumen(5000);
}

#ifndef CORRECTOR

int main() {
	pruebas_abb_estudiante();
	return 0;
}

#endif