#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "ataque.h"
#include "hash.h"

struct juego{
	informacion_pokemon_t *info;
	lista_t *pokemones;
	hash_t *pokemones_jugador;
	hash_t *puntaje_jugadores;
};
juego_t *juego_crear()
{
	juego_t *juego = calloc(1,sizeof(juego_t));
	if(juego == NULL)
		return NULL;
	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	informacion_pokemon_t *ip = pokemon_cargar_archivo(archivo);
	if(ip == NULL)
		return ERROR_GENERAL;
	if(pokemon_cantidad(ip) < 4)
		return POKEMON_INSUFICIENTES;
	juego->info = ip;
	return TODO_OK;
}
void insertar(pokemon_t *pokemon, void *aux){
	aux = lista_insertar(aux,pokemon);
	if(aux == NULL)
		return;
}
lista_t *juego_listar_pokemon(juego_t *juego)
{
	lista_t *lista = lista_crear();
	if(lista == NULL)
		return NULL;
	con_cada_pokemon(juego->info,insertar,lista);
	juego->pokemones = lista;
	return lista;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	hash_t *hash = hash_crear(4);
	if(hash == NULL)
		return ERROR_GENERAL;
	if(strcmp(nombre1,nombre2) == 0 || strcmp(nombre1,nombre3) == 0 
		|| strcmp(nombre2,nombre3) == 0)
		return POKEMON_REPETIDO;
	if(pokemon_buscar(juego->info,nombre1) == NULL ||pokemon_buscar(juego->info,nombre2) == NULL
		|| pokemon_buscar(juego->info,nombre2) == NULL)
		return POKEMON_INEXISTENTE;
	lista_t *lista = lista_crear();
	if(lista == NULL)
		return ERROR_GENERAL;
	lista = lista_insertar(lista,pokemon_buscar(juego->info,nombre1));
	lista = lista_insertar(lista,pokemon_buscar(juego->info,nombre2));
	lista = lista_insertar(lista,pokemon_buscar(juego->info,nombre3));
	const char *nombre_jugador;
	if(jugador == JUGADOR1){
		nombre_jugador = "jugador1";
		hash = hash_insertar(hash,nombre_jugador,lista,NULL);
	}
	else{
		nombre_jugador = "jugador2";
		hash = hash_insertar(hash,nombre_jugador,lista,NULL);
	}
	if(hash == NULL)
		return ERROR_GENERAL;
	juego->pokemones_jugador = hash;
	return TODO_OK;
}
int comparador(void *pokemon1, void *pokemon2){
	
	if(pokemon1 == pokemon2)
		return 0;
	return 1;
}
RESULTADO_ATAQUE calcular_resultado_jugada(pokemon_t *pokemon, const struct ataque *ataque,unsigned int *resultado){
	RESULTADO_ATAQUE resultado_ataque;
	switch (ataque->tipo)
	{
	case NORMAL:
		*resultado = ataque->poder;
		resultado_ataque = ATAQUE_REGULAR;
		break;
	case FUEGO:
		if(pokemon_tipo(pokemon) == PLANTA)
			*resultado = ataque->poder * 3;
		else if(pokemon_tipo(pokemon) == AGUA)
			*resultado = ataque->poder / 2;
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	case PLANTA:
		if(pokemon_tipo(pokemon) == ELECTRICO){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == PLANTA){
			*resultado = ataque->poder / 2;
			resultado_ataque = ATAQUE_INEFECTIVO;
		}
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	case ROCA:
		if(pokemon_tipo(pokemon) == ELECTRICO){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == PLANTA){
			*resultado = ataque->poder / 2;
			resultado_ataque = ATAQUE_INEFECTIVO;
		}
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	case ELECTRICO:
		if(pokemon_tipo(pokemon) == AGUA){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == ROCA){
			*resultado = ataque->poder / 2;
			resultado_ataque = ATAQUE_INEFECTIVO;
		}
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	case AGUA:
		if(pokemon_tipo(pokemon) == FUEGO){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == ELECTRICO){
			*resultado = ataque->poder / 2;
			resultado_ataque = ATAQUE_INEFECTIVO;
		}
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	}
	return resultado_ataque;
}
resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	const char *jugador1 = "jugador1";
	const char *jugador2 = "jugador2";
	resultado_jugada_t resultado;
	lista_t *lista1 = hash_obtener(juego->pokemones_jugador,jugador1);
	lista_t *lista2 = hash_obtener(juego->pokemones_jugador,jugador2);
	const char *pokemon_jugador1 = jugada_jugador1.pokemon;
	const char *pokemon_jugador2 = jugada_jugador2.pokemon;
	pokemon_t *pokemon1 = lista_buscar_elemento(lista1,comparador,pokemon_buscar(juego->info,pokemon_jugador1));
	pokemon_t *pokemon2 = lista_buscar_elemento(lista2,comparador,pokemon_buscar(juego->info,pokemon_jugador2));
	const char *ataque_jugador1 = jugada_jugador1.ataque;
	const char *ataque_jugador2 = jugada_jugador2.ataque;
	const struct ataque *ataque1 = pokemon_buscar_ataque(pokemon1,ataque_jugador1);
	const struct ataque *ataque2 = pokemon_buscar_ataque(pokemon2,ataque_jugador2);
	if(pokemon1 == NULL || ataque1 == NULL)
		resultado.jugador1 = ATAQUE_ERROR;
	if(pokemon2 == NULL || ataque2 == NULL)
		resultado.jugador2 = ATAQUE_ERROR;
	unsigned int resultado1;
	unsigned int resultado2;
	hash_t *hash = hash_crear(4);
	juego->puntaje_jugadores = hash;
	if(resultado.jugador1 != ATAQUE_ERROR){
		resultado.jugador1 = calcular_resultado_jugada(pokemon1,ataque2,&resultado1);
		if(hash_obtener(juego->puntaje_jugadores,"jugador1") != NULL){
			void * aux = hash_obtener(juego->puntaje_jugadores,"jugador1");
			unsigned int * r1 = (unsigned int *) aux;
			unsigned int r2 = *r1;
			resultado1 = (unsigned int)resultado1 + r2;
		}
		void * resultado1_hash = &resultado1;
		juego->puntaje_jugadores = hash_insertar(juego->puntaje_jugadores,"jugador1",resultado1_hash,NULL);
	}
	if(resultado.jugador2 != ATAQUE_ERROR){
		resultado.jugador2 = calcular_resultado_jugada(pokemon2,ataque1,&resultado2);
		if(hash_obtener(juego->puntaje_jugadores,"jugador2")){
			void * aux2 = hash_obtener(juego->puntaje_jugadores,"jugador2");
			unsigned int * r3 = (unsigned int *) aux2;
			unsigned int r4 = *r3;
			resultado2 = (unsigned int ) resultado2 + r4;
		}
		void * resultado2_hash = &resultado2;
		juego->puntaje_jugadores = hash_insertar(juego->puntaje_jugadores,"jugador2",resultado2_hash,NULL);
	}
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	return true;
}

void juego_destruir(juego_t *juego)
{
}
