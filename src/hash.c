#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define FACTOR_CARGA_MAXIMO 0.7

struct par {
	const char *clave;
	void *valor;
	struct par *siguiente;
};

struct hash {
	struct par **pares;
	size_t cantidad;
	size_t capacidad;
};

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (hash == NULL)
		return NULL;
	if (capacidad < 3)
		hash->capacidad = 3;
	else
		hash->capacidad = capacidad;
	hash->pares = calloc(hash->capacidad, sizeof(struct par *));
	if (hash->pares == NULL) {
		free(hash);
		return NULL;
	}
	return hash;
}
int funcion_hash(const char *k)
{
	int valor = 0;
	while (*k) {
		valor += *k;
		k++;
	}
	return valor;
}
char *strdup(const char *fuente)
{
	size_t longitud = strlen(fuente) + 1;
	char *destino = (char *)malloc(longitud);
	if (destino == NULL)
		return NULL;
	strcpy(destino, fuente);
	return destino;
}
void rehash(hash_t *hash)
{
	hash_t *hash_nuevo = hash_crear(hash->capacidad * 2);
	for (int i = 0; i < hash->capacidad; i++) {
		struct par *actual = hash->pares[i];
		while (actual != NULL) {
			hash_insertar(hash_nuevo, actual->clave, actual->valor,
				      NULL);
			actual = actual->siguiente;
		}
	}
	hash_t auxilar = *hash;
	*hash = *hash_nuevo;
	*hash_nuevo = auxilar;
	hash_destruir(hash_nuevo);
}
hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	if ((float)hash->cantidad / (float)hash->capacidad >
	    FACTOR_CARGA_MAXIMO)
		rehash(hash);
	int posicion = (int)funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->pares[posicion];
	while (actual != NULL) {
		if (strcmp(actual->clave, clave) == 0) {
			if (anterior != NULL) {
				*anterior = actual->valor;
			}
			actual->valor = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}
	struct par *nuevo_par = calloc(1, sizeof(struct par));
	if (nuevo_par == NULL)
		return NULL;
	if (anterior != NULL) {
		*anterior = NULL;
	}
	nuevo_par->clave = strdup(clave);
	if (nuevo_par->clave == NULL) {
		free(nuevo_par);
		return NULL;
	}
	nuevo_par->valor = elemento;
	nuevo_par->siguiente = hash->pares[posicion];
	hash->pares[posicion] = nuevo_par;
	hash->cantidad++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	int posicion = (int)funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->pares[posicion];
	struct par *anterior = NULL;
	while (actual != NULL) {
		if (strcmp(actual->clave, clave) == 0) {
			struct par *aux = actual;
			void *elemento = actual->valor;
			if (anterior == NULL) {
				hash->pares[posicion] = actual->siguiente;
				free((char *)actual->clave);
				free(actual);
				hash->cantidad--;
				return elemento;
			}
			anterior->siguiente = actual->siguiente;
			free((char *)aux->clave);
			free(aux);
			hash->cantidad--;
			return elemento;
		}
		anterior = actual;
		actual = actual->siguiente;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	int posicion = (int)funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->pares[posicion];
	while (actual != NULL) {
		if (strcmp(actual->clave, clave) == 0) {
			return actual->valor;
		}
		actual = actual->siguiente;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;
	int posicion = (int)funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->pares[posicion];
	while (actual != NULL) {
		if (strcmp(actual->clave, clave) == 0) {
			return true;
		}
		actual = actual->siguiente;
	}
	return false;
	;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	for (int i = 0; i < hash->capacidad; i++) {
		struct par *actual = hash->pares[i];
		while (actual != NULL) {
			struct par *aux = actual;
			actual = actual->siguiente;
			free((char *)aux->clave);
			if (destructor != NULL)
				destructor(aux->valor);
			free(aux);
		}
	}
	free(hash->pares);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;
	for (int i = 0; i < hash->capacidad; i++) {
		struct par *actual = hash->pares[i];
		while (actual != NULL) {
			n++;
			if (f(actual->clave, actual->valor, aux) == false)
				return n;
			actual = actual->siguiente;
		}
	}
	return n;
}
