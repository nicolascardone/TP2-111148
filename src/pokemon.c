#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>
struct pokemon {
	char nombre[20];
	enum TIPO tipo;
	struct ataque ataques[3];
};

struct info_pokemon {
	pokemon_t *pokemones;
	int cantidad;
};

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL) {
		return NULL;
	}
	informacion_pokemon_t *info = malloc(sizeof(informacion_pokemon_t));
	if (info == NULL)
		return NULL;
	FILE *archivo = fopen(path, "r");
	if (archivo == NULL) {
		free(info);
		return NULL;
	}
	char linea[500];
	char tipo;
	info->pokemones = NULL;
	info->cantidad = 0;
	bool poke = false;
	int contador_ataques = 0;
	pokemon_t pokemon;
	struct ataque ataque;
	while (fgets(linea, 500, archivo) != NULL) {
		if (!poke) {
			if (sscanf(linea, "%[^;];%c", pokemon.nombre, &tipo) ==
			    2) {
				switch (tipo) {
				case 'N':
					pokemon.tipo = NORMAL;
					break;
				case 'F':
					pokemon.tipo = FUEGO;
					break;
				case 'A':
					pokemon.tipo = AGUA;
					break;
				case 'P':
					pokemon.tipo = PLANTA;
					break;
				case 'E':
					pokemon.tipo = ELECTRICO;
					break;
				case 'R':
					pokemon.tipo = ROCA;
					break;
				default:
					fclose(archivo);
					return info;
				}
				poke = true;
			}
		} else {
			if (sscanf(linea, "%[^;];%c;%u", ataque.nombre, &tipo,
				   &ataque.poder) == 3) {
				switch (tipo) {
				case 'N':
					ataque.tipo = NORMAL;
					break;
				case 'F':
					ataque.tipo = FUEGO;
					break;
				case 'A':
					ataque.tipo = AGUA;
					break;
				case 'P':
					ataque.tipo = PLANTA;
					break;
				case 'E':
					ataque.tipo = ELECTRICO;
					break;
				case 'R':
					ataque.tipo = ROCA;
					break;
				default:
					fclose(archivo);
					return info;
				}
				pokemon.ataques[contador_ataques] = ataque;
				contador_ataques++;
			} else {
				free(info);
				fclose(archivo);
				return NULL;
			}
		}
		if (contador_ataques == 3) {
			info->cantidad++;
			info->pokemones = realloc(info->pokemones,
						  (size_t)(info->cantidad) *
							  sizeof(pokemon_t));
			info->pokemones[info->cantidad - 1] = pokemon;
			contador_ataques = 0;
			poke = false;
		}
	}
	if (info->cantidad == 0) {
		fclose(archivo);
		return NULL;
	}
	fclose(archivo);

	return info;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL)
		return NULL;
	for (int i = 0; i < ip->cantidad; i++) {
		if (strcmp(ip->pokemones[i].nombre, nombre) == 0) {
			pokemon_t *pokemon = &ip->pokemones[i];
			return pokemon;
		}
	}
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return 0;
	return ip->cantidad;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	char *nombre_devolver = pokemon->nombre;
	return nombre_devolver;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon != NULL) {
		return pokemon->tipo;
	}
	return NORMAL;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL || nombre == NULL)
		return NULL;
	for (int i = 0; i < 3; i++) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0) {
			struct ataque *ataque = &pokemon->ataques[i];
			return ataque;
		}
	}
	return NULL;
}
int comparar(const void *a, const void *b)
{
	return strcmp((const char *)a, (const char *)b);
}
void cambiar(pokemon_t *pokemon_1, pokemon_t *pokemon_2)
{
	pokemon_t aux = *pokemon_1;
	*pokemon_1 = *pokemon_2;
	*pokemon_2 = aux;
}
int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL) {
		return 0;
	}
	if (f == NULL)
		return 0;
	pokemon_t *pokemones = ip->pokemones;
	int contador = 0;
	for (int n = 0; n < ip->cantidad; n++) {
		for (int p = 0; p < ip->cantidad - n - 1; p++) {
			if (comparar(pokemones[p].nombre,
				     pokemones[p + 1].nombre) > 0) {
				cambiar(&pokemones[p], &pokemones[p + 1]);
			}
		}
	}
	for (int i = 0; i < ip->cantidad; i++) {
		pokemon_t *pokemon = &pokemones[i];
		f(pokemon, aux);
		contador++;
	}
	return contador;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	int contador = 0;
	if (pokemon == NULL)
		return 0;
	if (f == NULL)
		return 0;
	for (int i = 0; i < 3; i++) {
		f(&pokemon->ataques[i], aux);
		contador++;
	}
	return contador;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		if (ip->pokemones != NULL) {
			free(ip->pokemones);
		}
		free(ip);
	}
}
