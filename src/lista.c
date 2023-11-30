#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t cantidad;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *nodo_actual;
};

lista_t *lista_crear()
{
	struct lista *lista = malloc(sizeof(lista_t));
	if (lista == NULL)
		return NULL;
	lista->nodo_inicio = NULL;
	lista->nodo_final = NULL;
	lista->cantidad = 0;
	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;
	if (lista->cantidad == 0) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	} else {
		lista->nodo_final->siguiente = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	}
	lista->cantidad++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;
	if (posicion >= lista->cantidad) {
		return lista_insertar(lista, elemento);
	}
	nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	if (posicion == 0) {
		nodo_nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo_nuevo;
	} else {
		nodo_t *nodo_actual = lista->nodo_inicio;
		for (size_t i = 1; i < posicion; i++) {
			nodo_actual = nodo_actual->siguiente;
		}
		nodo_nuevo->siguiente = nodo_actual->siguiente;
		nodo_actual->siguiente = nodo_nuevo;
	}
	lista->cantidad++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0) {
		return NULL;
	}
	if (lista->cantidad == 1) {
		nodo_t *aux = lista->nodo_inicio;
		void *elemento = aux->elemento;
		free(aux);
		lista->nodo_inicio = NULL;
		lista->nodo_final = NULL;
		lista->cantidad = 0;
		return elemento;
	}
	nodo_t *nodo_anteultimo = lista->nodo_inicio;
	while (nodo_anteultimo != NULL &&
	       nodo_anteultimo->siguiente != lista->nodo_final) {
		nodo_anteultimo = nodo_anteultimo->siguiente;
	}
	nodo_t *aux = lista->nodo_final;
	nodo_anteultimo->siguiente = NULL;
	lista->nodo_final = nodo_anteultimo;
	lista->cantidad--;
	void *elemento = aux->elemento;
	free(aux);
	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || lista->cantidad == 0)
		return NULL;
	if (posicion == 0) {
		nodo_t *aux = lista->nodo_inicio;
		lista->nodo_inicio = aux->siguiente;
		lista->cantidad--;
		void *elemento = aux->elemento;
		free(aux);
		return elemento;
	}
	if (posicion >= lista->cantidad - 1) {
		return lista_quitar(lista);
	}
	nodo_t *nodo_actual = lista->nodo_inicio;
	for (size_t i = 1; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}
	nodo_t *aux = nodo_actual->siguiente;
	nodo_actual->siguiente = aux->siguiente;
	void *elemento = aux->elemento;
	free(aux);
	lista->cantidad--;
	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || posicion >= lista->cantidad ||
	    lista->cantidad == 0)
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	if (posicion == 0)
		return nodo_actual->elemento;
	for (size_t i = 0; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}
	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista == NULL || lista->cantidad == 0 || comparador == NULL)
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while (nodo_actual != NULL) {
		if (comparador(nodo_actual->elemento, contexto) == 0)
			return nodo_actual->elemento;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0)
		return NULL;
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0)
		return NULL;
	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0)
		return true;
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0)
		return 0;
	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (lista != NULL) {
		nodo_t *nodo_actual = lista->nodo_inicio;
		while (nodo_actual != NULL) {
			nodo_t *aux = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			free(aux);
		}
		lista->nodo_final = NULL;
		free(lista);
	}
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista != NULL) {
		nodo_t *nodo_actual = lista->nodo_inicio;
		while (nodo_actual != NULL) {
			nodo_t *aux = nodo_actual;
			if (funcion != NULL)
				funcion(nodo_actual->elemento);
			nodo_actual = nodo_actual->siguiente;
			free(aux);
		}
		free(lista);
	}
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;
	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (iterador == NULL)
		return NULL;
	iterador->lista = lista;
	iterador->nodo_actual = lista->nodo_inicio;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->nodo_actual == NULL ||
	    iterador->lista == NULL)
		return false;
	return true;
}
bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (iterador == NULL || !lista_iterador_tiene_siguiente(iterador))
		return false;
	if (iterador->nodo_actual->siguiente == NULL) {
		iterador->nodo_actual = iterador->nodo_actual->siguiente;
		return false;
	}
	iterador->nodo_actual = iterador->nodo_actual->siguiente;
	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->nodo_actual == NULL)
		return NULL;
	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return;
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista == NULL || funcion == NULL)
		return 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	bool validez = true;
	size_t iteraciones = 0;
	while (nodo_actual != NULL && validez) {
		if (funcion(nodo_actual->elemento, contexto) == false) {
			validez = false;
		}
		iteraciones++;
		nodo_actual = nodo_actual->siguiente;
	}
	return iteraciones;
}
