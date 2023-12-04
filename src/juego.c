#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdio.h>

struct jugador{
	lista_t *lista;
	lista_t *ataques;
	int puntaje;
};
struct juego{
	informacion_pokemon_t *info;
	lista_t *pokemones;
	struct jugador *jugador1;
	struct jugador *jugador2;
	int rondas;
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
	juego->info = pokemon_cargar_archivo(archivo);
	if(juego->info == NULL){
		pokemon_destruir_todo(juego->info);
		juego->info = NULL;
		return ERROR_GENERAL;
	}
	if(pokemon_cantidad(juego->info) < 6){
		pokemon_destruir_todo(juego->info);
		juego->info = NULL;
		return POKEMON_INSUFICIENTES;
	}
	juego->jugador1 = calloc(1,sizeof(struct jugador));
	juego->jugador2 = calloc(1,sizeof(struct jugador));
	if(juego->jugador1 == NULL || juego->jugador2 == NULL)
		return ERROR_GENERAL;
	juego->jugador1->lista = lista_crear();
	juego->jugador2->lista = lista_crear();
	juego->jugador1->ataques = lista_crear();
	juego->jugador2->ataques = lista_crear();
	if(juego->jugador1->lista == NULL || juego->jugador2->lista == NULL || juego->jugador1->ataques == NULL
		|| juego->jugador2->ataques == NULL){
		return ERROR_GENERAL;
	}
	return TODO_OK;
}
void insertar(pokemon_t *pokemon, void *aux){
	aux = lista_insertar(aux,pokemon);
}
lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(juego == NULL)
		return NULL;
	if(juego->info == NULL)
		return NULL;
	juego->pokemones = lista_crear();
	con_cada_pokemon(juego->info,insertar,juego->pokemones);
	return juego->pokemones;
}
void insertar2(const struct ataque *ataque, void *aux){
	aux = lista_insertar(aux,(void*)ataque);
}
JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	pokemon_t *poke1 = pokemon_buscar(juego->info,nombre1);
	pokemon_t *poke2 = pokemon_buscar(juego->info,nombre2);
	pokemon_t *poke3 = pokemon_buscar(juego->info,nombre3);
	if(poke1 == NULL || poke2 == NULL || poke3 == NULL){
		return POKEMON_INEXISTENTE;
	}
	if(strcmp(nombre1,nombre2) == 0 || strcmp(nombre1,nombre3) == 0 
		|| strcmp(nombre2,nombre3) == 0)
		return POKEMON_REPETIDO;
	if(jugador == JUGADOR1){
		juego->jugador1->lista = lista_insertar(juego->jugador1->lista,(void *)poke1);
		juego->jugador1->lista = lista_insertar(juego->jugador1->lista,(void *)poke2);
		juego->jugador2->lista = lista_insertar(juego->jugador2->lista,(void *)poke3);
		con_cada_ataque(poke1,insertar2,juego->jugador1->ataques);
		con_cada_ataque(poke2,insertar2,juego->jugador1->ataques);
		con_cada_ataque(poke3,insertar2,juego->jugador2->ataques);

	}else if(jugador == JUGADOR2){
		juego->jugador2->lista = lista_insertar(juego->jugador2->lista,(void *)poke1);
		juego->jugador2->lista = lista_insertar(juego->jugador2->lista,(void *)poke2);
		juego->jugador1->lista = lista_insertar(juego->jugador1->lista,(void *)poke3);
		con_cada_ataque(poke1,insertar2,juego->jugador2->ataques);
		con_cada_ataque(poke2,insertar2,juego->jugador2->ataques);
		con_cada_ataque(poke3,insertar2,juego->jugador1->ataques);
	}
	return TODO_OK;
}
int comparador(void *elemento1, void *elemento2){
	
	if(elemento1 == elemento2){
		return 0;
	}
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
		if(pokemon_tipo(pokemon) == PLANTA){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == AGUA){
			if(ataque->poder % 2 != 0){
				*resultado = ataque->poder / 2;
				(*resultado)++;
			}else
				*resultado = ataque->poder / 2;
			resultado_ataque = ATAQUE_INEFECTIVO;
		}
		else{
			*resultado = ataque->poder;
			resultado_ataque = ATAQUE_REGULAR;
		}
		break;
	case PLANTA:
		if(pokemon_tipo(pokemon) == ROCA){
			*resultado = ataque->poder * 3;
			resultado_ataque = ATAQUE_EFECTIVO;
		}
		else if(pokemon_tipo(pokemon) == FUEGO){
			if(ataque->poder % 2 != 0){
				*resultado = ataque->poder / 2;
				(*resultado)++;
			}else
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
			if(ataque->poder % 2 != 0){
				*resultado = ataque->poder / 2;
				(*resultado)++;
			}else
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
			if(ataque->poder % 2 != 0){
				*resultado = ataque->poder / 2;
				(*resultado)++;
			}else
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
			if(ataque->poder % 2 != 0){
				*resultado = ataque->poder / 2;
				(*resultado)++;
			}else
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
bool ataque_utilizado(lista_t *ataques, const struct ataque *ataque){
	void* elemento = lista_buscar_elemento(ataques,comparador,(void*)ataque);
	if(elemento != NULL){
		return false;
	}
	return true;
}
resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado ;
	bool error1 = false;
	const struct ataque *ataque1 = NULL;
	const struct ataque *ataque2 = NULL;
	pokemon_t *poke = pokemon_buscar(juego->info,jugada_jugador1.pokemon);
	pokemon_t *pokemon1 = lista_buscar_elemento(juego->jugador1->lista,comparador,(void*)poke);
	pokemon_t *poke2 = pokemon_buscar(juego->info,jugada_jugador2.pokemon);
	pokemon_t *pokemon2 = lista_buscar_elemento(juego->jugador2->lista,comparador,(void*)poke2);
	const char *a1 = jugada_jugador1.ataque;
	const char *a2 = jugada_jugador2.ataque;
	if(pokemon1 != NULL)
		ataque1 = pokemon_buscar_ataque(pokemon1,a1);
	if(pokemon2 != NULL)
		ataque2 = pokemon_buscar_ataque(pokemon2,a2);
	if(pokemon1 == NULL || ataque1 == NULL){
		resultado.jugador1 = ATAQUE_ERROR;
		error1 = true;
	}
	if(pokemon2 == NULL || ataque2 == NULL){
		resultado.jugador2 = ATAQUE_ERROR;
		error1 = true;
	}
	if(!error1){
		lista_t *lista_1 = juego->jugador1->ataques;
		lista_t *lista_2 = juego->jugador2->ataques;
		if(ataque_utilizado(lista_1,ataque1)){
			resultado.jugador1 = ATAQUE_ERROR;
			error1 = true;
		}
		if(ataque_utilizado(lista_2,ataque2)){
			resultado.jugador2 = ATAQUE_ERROR;
			error1 = true;
		}
		unsigned int resultado1;
		unsigned int resultado2;
		if(error1 == false){
			resultado.jugador1 = calcular_resultado_jugada(pokemon2,ataque1,&resultado1);
			juego->jugador1->puntaje += (int)resultado1;
			resultado.jugador2 = calcular_resultado_jugada(pokemon1,ataque2,&resultado2);
			juego->jugador2->puntaje += (int)resultado2;
			juego->rondas++;
			for(size_t i = 0; i < lista_tamanio(juego->jugador1->ataques); i++){
				if(lista_elemento_en_posicion(juego->jugador1->ataques,i) == (void*)ataque1){
					lista_quitar_de_posicion(juego->jugador1->ataques,i);
					break;
				}
			}
			for(size_t n = 0; n < lista_tamanio(juego->jugador1->ataques); n++){
				if(lista_elemento_en_posicion(juego->jugador2->ataques,n) == (void*)ataque2){
					lista_quitar_de_posicion(juego->jugador2->ataques,n);
					break;
				}
			}
		}
	}
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if(juego == NULL)
		return 0;
	if(juego->jugador1 != NULL){
		if(jugador == JUGADOR1){
			return juego->jugador1->puntaje;
		}
	}
	if(juego->jugador2 != NULL){
		if(jugador == JUGADOR2){
			return juego->jugador2->puntaje;
		}
	}
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	if(juego->rondas >= 9)
		return true;
	return false;
}
void juego_destruir(juego_t *juego)
{
	if(juego == NULL)
		return;
	if(juego->jugador1){
		if(juego->jugador1->lista)
			lista_destruir(juego->jugador1->lista);
		if(juego->jugador1->ataques)
			lista_destruir(juego->jugador1->ataques);
		free(juego->jugador1);
	}
	if(juego->jugador1){
		if(juego->jugador2->lista)
			lista_destruir(juego->jugador2->lista);
		if(juego->jugador2->ataques)
			lista_destruir(juego->jugador2->ataques);
		free(juego->jugador2);
	}
	pokemon_destruir_todo(juego->info);
	lista_destruir(juego->pokemones);
	free(juego);
}