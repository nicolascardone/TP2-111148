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
	srand((unsigned int)time(NULL));
	int rango = maximo - minimo + 1;
	int numero = rand() % rango + minimo;
	return numero;
}
bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	size_t maximo = lista_tamanio(adversario->pokemones);
	int posicion = numero_aleratorio(0,(int)maximo-1);
	pokemon_t *poke1 = lista_elemento_en_posicion(adversario->pokemones,(size_t)posicion);
	lista_quitar_de_posicion(adversario->pokemones,(size_t) posicion);
	maximo = lista_tamanio(adversario->pokemones);
	int posicion2 = numero_aleratorio(0,(int)maximo-1);
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->pokemones,(size_t)posicion2);
	lista_quitar_de_posicion(adversario->pokemones,(size_t) posicion2);
	maximo = lista_tamanio(adversario->pokemones);
	int posicion3 = numero_aleratorio(0,(int)maximo-1);
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
int comparador_3(void *pokemon1, void *nombre){
	
	if(strcmp(pokemon_nombre(pokemon1),nombre) == 0)
		return 0;
	return 1;
}
bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemones,comparador_3,nombre3);
	adversario->pokemones_adversario = lista_insertar(adversario->pokemones_adversario,pokemon3);
	return true;
}
int comparador_2(void *ataque1, void* ataque2){
	if(ataque1 == ataque2)
		return 0;
	return 1;
}
void funcion(const struct ataque *ataque, void* lista){
	lista = lista_insertar(lista,(void *)ataque);
}
int comparar1(void * ataque1, void *ataque2){
	if(ataque1 == ataque2)
		return 0;
	return 1;
}
jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	int posicion = numero_aleratorio(0,2);
	pokemon_t *poke = lista_elemento_en_posicion(adversario->pokemones_adversario,(size_t)posicion);
	lista_t *lista = lista_crear();
	con_cada_ataque(poke,funcion,lista);
	bool seguir = true;
	int posicion2 = numero_aleratorio(0,2);
	struct ataque *ataque = lista_elemento_en_posicion(lista,(size_t)posicion2);
	if(lista_buscar_elemento(adversario->ataques_usados,comparar1,(void *)ataque) != NULL){
		while (seguir)
		{
			posicion2 = numero_aleratorio(0,2);
			ataque = lista_elemento_en_posicion(lista,(size_t)posicion2);
			if(lista_buscar_elemento(adversario->ataques_usados,comparar1,(void *)ataque) == NULL){
				seguir = false;
			}
		}
		
	}
	adversario->ataques_usados = lista_insertar(adversario->ataques_usados,(void *)ataque);
	lista_destruir(lista);
	jugada_t jugada;
	strcpy(jugada.ataque,ataque->nombre);
	strcpy(jugada.pokemon,pokemon_nombre(poke));
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
