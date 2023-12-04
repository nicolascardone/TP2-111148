#include "adversario.h"
#include "ataque.h"
#include "juego.h"
#include "lista.h"
#include "pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct adversario {
	lista_t *pokemones;
	lista_t *pokemones_adversario;
	int rondas;
	int ataque;
};
adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = calloc(1, sizeof(adversario_t));
	if (adversario == NULL)
		return NULL;
	adversario->pokemones_adversario = lista_crear();
	adversario->pokemones = pokemon;
	return adversario;
}
int numero_aleratorio(int minimo, int maximo)
{
	srand((unsigned int)time(NULL));
	int rango = maximo - minimo + 1;
	int numero = rand() % rango + minimo;
	return numero;
}
bool auxiliar(void *pokemon, void *lista)
{
	lista = lista_insertar(lista, pokemon);
	return true;
}
bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	lista_t *lista_aux = lista_crear();
	lista_con_cada_elemento(adversario->pokemones, auxiliar, lista_aux);
	size_t maximo = lista_tamanio(lista_aux);
	int posicion = numero_aleratorio(0, (int)maximo - 1);
	pokemon_t *poke1 =
		lista_elemento_en_posicion(lista_aux, (size_t)posicion);
	lista_quitar_de_posicion(lista_aux, (size_t)posicion);
	maximo = lista_tamanio(lista_aux);
	int posicion2 = numero_aleratorio(0, (int)maximo - 1);
	pokemon_t *poke2 =
		lista_elemento_en_posicion(lista_aux, (size_t)posicion2);
	lista_quitar_de_posicion(lista_aux, (size_t)posicion2);
	maximo = lista_tamanio(lista_aux);
	int posicion3 = numero_aleratorio(0, (int)maximo - 1);
	pokemon_t *poke3 =
		lista_elemento_en_posicion(lista_aux, (size_t)posicion3);
	const char *nombre_poke1 = pokemon_nombre(poke1);
	const char *nombre_poke2 = pokemon_nombre(poke2);
	const char *nombre_poke3 = pokemon_nombre(poke3);
	*nombre1 = malloc(strlen(nombre_poke1) + 1);
	*nombre2 = malloc(strlen(nombre_poke2) + 1);
	*nombre3 = malloc(strlen(nombre_poke3) + 1);
	if (*nombre1 == NULL || *nombre2 == NULL || *nombre3 == NULL)
		return false;
	lista_destruir(lista_aux);
	strcpy(*nombre1, nombre_poke1);
	strcpy(*nombre2, nombre_poke2);
	strcpy(*nombre3, nombre_poke3);
	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, poke1);
	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, poke2);
	return true;
}
int comparador_3(void *pokemon1, void *nombre)
{
	if (strcmp(pokemon_nombre(pokemon1), nombre) == 0)
		return 0;
	return 1;
}
bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemones,
						    comparador_3, nombre3);
	adversario->pokemones_adversario =
		lista_insertar(adversario->pokemones_adversario, pokemon3);
	return true;
}
void funcion(const struct ataque *ataque, void *lista)
{
	lista = lista_insertar(lista, (void *)ataque);
}
int comparar1(void *ataque1, void *ataque2)
{
	if (ataque1 == ataque2)
		return 0;
	return 1;
}
jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	const struct ataque *ataque = NULL;
	pokemon_t *poke = NULL;
	// pokemon_y_ataque_aleratorio(&ataque,&poke,adversario);
	/*for(size_t i = 0; i < 9; i++){
          if(i < 3){
                  pokemon_t *poke =
  lista_elemento_en_posicion(adversario->pokemones_adversario,i); printf("%s
  \n",pokemon_nombre(poke));;
          }
          struct ataque *ataque =
  lista_elemento_en_posicion(adversario->ataques,i); printf(" \t%s
  \n",ataque->nombre);
  }*/
	lista_t *lista = lista_crear();
	poke = lista_elemento_en_posicion(adversario->pokemones_adversario,
					  (size_t)adversario->rondas);
	con_cada_ataque(poke, funcion, lista);
	ataque = lista_elemento_en_posicion(lista, (size_t)adversario->ataque);
	adversario->ataque++;
	if (adversario->ataque == 3) {
		adversario->rondas++;
		adversario->ataque = 0;
	}
	lista_destruir(lista);
	jugada_t jugada;
	strcpy(jugada.ataque, ataque->nombre);
	strcpy(jugada.pokemon, pokemon_nombre(poke));
	return jugada;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	if (adversario->pokemones_adversario != NULL)
		lista_destruir(adversario->pokemones_adversario);
	free(adversario);
}
