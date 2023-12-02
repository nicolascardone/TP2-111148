#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "ataque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct adversario{
	lista_t *pokemones;
	lista_t *pokemones_adversario;
	lista_t *ataques_usados;
};

char nombres[][20] = {"Rayo","Latigo","Chispa","Furia","Lanzallamas","Ascuas","Golpe","Terremoto"
						,"Avalancha","Corte","Absorber","Hojas","Megapuño","Hidropulso","OndaVoltio"
						,"Cascada","Buceo","Retribucion"};
adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = calloc(1,sizeof(adversario_t));
	if(adversario == NULL)
		return NULL;
	adversario->pokemones_adversario = lista_crear();
	adversario->ataques_usados = lista_crear();
	adversario->pokemones = pokemon;
	return adversario;
}
int numero_aleratorio(int minimo,int maximo){
	srand(time(NULL));
	return minimo + rand() % (maximo - minimo + 1);
}
bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	size_t maximo = lista_tamanio(adversario->pokemones);
	int posicion = numero_aleratorio(0,(int)maximo-1);
	pokemon_t *poke1 = lista_elemento_en_posicion(adversario->pokemones,(size_t)posicion);
	bool validez = true;
	int posicion2;
	while(validez){
		posicion2 = numero_aleratorio(0,(int)maximo-1);
		if(posicion2 != posicion)
			validez = false;
	}
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->pokemones,(size_t)posicion2);
	bool validez2 = true;
	int posicion3;
	while(validez2){
		posicion3 = numero_aleratorio(0,(int)maximo-1);
		if(posicion3 != posicion && posicion3 != posicion2)
			validez = false;
	}
	pokemon_t *poke3 = lista_elemento_en_posicion(adversario->pokemones,(size_t)posicion3);
	const char *nombre_poke1 = pokemon_nombre(poke1);
	const char *nombre_poke2 = pokemon_nombre(poke2);
	const char *nombre_poke3 = pokemon_nombre(poke3);
	*nombre1 = malloc(strlen(nombre_poke1) + 1);
	*nombre2 = malloc(strlen(nombre_poke2) + 1);
	*nombre3 = malloc(strlen(nombre_poke3) + 1);
	if(*nombre1 == NULL|| *nombre2 == NULL || *nombre3 == NULL )
		return false;
	strcpy(*nombre1,nombre_poke1);
	strcpy(*nombre2,nombre_poke2);
	strcpy(*nombre3,nombre_poke3);
	adversario->pokemones_adversario = lista_insertar(adversario->pokemones_adversario,poke1);
	adversario->pokemones_adversario = lista_insertar(adversario->pokemones_adversario,poke2);
	return true;
}
int comparador(void *pokemon1, void *nombre){
	
	if(strcmp(pokemon_nombre(pokemon1),nombre) == 0)
		return 0;
	return 1;
}
bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemones,comparador,nombre3);
	adversario->pokemones_adversario = lista_insertar(adversario->pokemones_adversario,pokemon3);
	return true;
}
int comparador_2(void *ataque1, void* ataque2){
	if(ataque1 == ataque2)
		return 0;
	return 1;
}
jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	const struct ataque *ataque = NULL;
	pokemon_t *poke = NULL;
	while (ataque == NULL && lista_buscar_elemento(adversario->ataques_usados,comparador_2,(void *)ataque) == NULL)
	{
		int posicion = numero_aleratorio(0,2);
		pokemon_t *poke = lista_elemento_en_posicion(adversario->pokemones_adversario,(size_t)posicion);
		int posicion_ataque = numero_aleratorio(0,17);
		ataque = pokemon_buscar_ataque(poke,nombres[posicion_ataque]);
	}
	adversario->ataques_usados = lista_insertar(adversario->ataques_usados,(void *)ataque);
	jugada_t jugada = { ataque->nombre, pokemon_nombre(poke)};
	return jugada;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	lista_destruir(adversario->ataques_usados);
	lista_destruir(adversario->pokemones_adversario);
	lista_destruir(adversario->pokemones);
	free(adversario);
}
