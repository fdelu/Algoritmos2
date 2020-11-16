#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MSG_ERROR "ERROR"

typedef struct calc_token tok_t;

// Lee una línea del standard input, devolviendo un vector a memoria dinamica
// con strings de cada caracter (menos espacios) de la línea por separado. Devuelve
// NULL si falla. Se debe liberar el vector con free_strv() de strutil.h
char** leer_linea(){

	char* linea = NULL;
	size_t tam = 0;
	if(getline(&linea, &tam, stdin) != -1){

		char** vector = infix_split(linea);
		free(linea);
		return vector;
	}
	free(linea); //Documentación de getline pide liberar aunque falle la función
	return NULL;
}

// Desapila e imprime operadores hasta que un elemento cumpla
// la condición de corte.
// Pre: Los strings de la pila deben ser parseables por calc_parse() de
// calc_helper.h. El corte puede ser NULL, e imprime todos los elementos.
void print_hasta(pila_t* operadores, bool (*corte)(char* operador, char* extra), char* extra){

	char* anterior = pila_ver_tope(operadores);

	while(anterior && (!corte || corte(anterior, extra))){
		printf("%s ", anterior);
		pila_desapilar(operadores);
		anterior = pila_ver_tope(operadores);
	}
}

// Recibe el string del operador leído actual y del operador anterior apilado.
// Devuelve true si el operador anterior se debe imprimir antes de apilar
// el actual (según algoritmo de Shunting-yard), sino false.
// Pre: Los strings de los operadores deben ser parseables por
// calc_parse() de calc_helper.h (el anterior puede ser NULL)
bool debe_escribir(char* op_anterior, char* actual){
	
	if(!op_anterior)
		return false;

	tok_t tok_ant;
	tok_t tok_act;
	calc_parse(op_anterior, &tok_ant);
	calc_parse(actual, &tok_act);

	return( (tok_ant.type == TOK_OPER) && 
		( (tok_ant.oper.precedencia > tok_act.oper.precedencia) ||
		( (tok_ant.oper.precedencia == tok_act.oper.precedencia) &&
		(tok_ant.oper.asociatividad == ASSOC_LEFT) )));
}

// Devuelve true si el string es un token de paréntesis izquierdo
// (según calc_parse()), sino false. 
// Pre: el string del es un operador parseable por calc_parse() de calc_helper.h
bool es_lparen(char* operador, char* extra){

	tok_t tok;
	calc_parse(operador, &tok);
	return tok.type == TOK_LPAREN;
}

// Imprime el string pasado en notación posfija.
// Pre: El input es un string de elemtentos en orden de notación infija.
// Imprime MSG_ERROR si falla.
void print_postfix(char** input){

	pila_t* operadores = pila_crear();
	if(!operadores){
		printf("%s\n", MSG_ERROR);
		return;
	}

	while(*input){
		struct calc_token tok;
		if(!calc_parse(*input, &tok)){
			pila_destruir(operadores);
			printf("%s\n", MSG_ERROR);
			return;
		}

		if(tok.type == TOK_NUM){
			printf("%s ", *input);
		} else if (tok.type == TOK_OPER){
			print_hasta(operadores, debe_escribir, *input);
			if(!pila_apilar(operadores, (void*) *input)){
				pila_destruir(operadores);
				printf("%s\n", MSG_ERROR);
				return;
			}
		} else if (tok.type == TOK_LPAREN){
			if(!pila_apilar(operadores, (void*) *input)){
				pila_destruir(operadores);
				printf("%s\n", MSG_ERROR);
				return;
			}
		} else if (tok.type == TOK_RPAREN){
			print_hasta(operadores, es_lparen, NULL);
			pila_desapilar(operadores); //Descarto el paréntesis
		}
		input++;
	}

	print_hasta(operadores, NULL, NULL);
	pila_destruir(operadores);
}

int main(){

	while(!feof(stdin)){
		char** input = leer_linea();
		if(input){
			print_postfix(input);
			printf("\n");
			free_strv(input);
		}
	}

	return 0;
}
