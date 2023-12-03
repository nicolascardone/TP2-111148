#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* Este main debe ser modificado para que el usuario pueda jugar el juego. Las
* instrucciones existentes son solamente a modo ilustrativo del funcionamiento
* muy alto nivel del juego.
*
* Las interacciones deben realizarse por entrada/salida estandar y estar validadas.
*
* Se aconseja en todo momento mostrar información relevante para el jugador (por
* ejemplo, mostrar puntaje actual y movimientos disponibles) para hacer que el
* juego sea facil de utilizar.
*/
char *pedir_archivo(){
	printf("\t INGRESE EL NOMBRE DEL ARCHIVO \n");
	char *archivo = (char *)malloc(100 * sizeof(char));
	if(archivo == NULL){
		printf("ERROR DE MEMORIA \n");
		return NULL;
	}

	fgets(archivo,100,stdin);
	archivo[strlen(archivo) -1] = '\0';

	return archivo;
}
bool imprimir(void *pokemon, void *aux){
	printf("%s \n",pokemon_nombre(pokemon));
	return true;
}
void mostrar_pokemon_disponibles(lista_t *pokemones){
	printf("\t POKEMONES DISPONIBLES: \n");
	lista_con_cada_elemento(pokemones,imprimir,NULL);
}
void pedir_pokemones_jugador(char **nombre1,char **nombre2,char **nombre3){
	unsigned int maximo_tamano = 100;
	*nombre1 = (char *)malloc(maximo_tamano * sizeof(char));
	*nombre2 = (char *)malloc(maximo_tamano * sizeof(char));
	*nombre3 = (char *)malloc(maximo_tamano * sizeof(char));
	if(*nombre1 == NULL || *nombre2 == NULL || *nombre3 == NULL){
		printf("\t ERROR DE MEMORIA /n");
		return;
	}
	printf("\t INGRESE EL NOMBRE DEL PRIMER POKEMON \n");
	fgets(*nombre1,(int)maximo_tamano,stdin);
	(*nombre1)[strlen(*nombre1) -1] = '\0';
	printf("\t INGRESE EL NOMBRE DEL SEGUNDO POKEMON \n");
	fgets(*nombre2,(int)maximo_tamano,stdin);
	(*nombre2)[strlen(*nombre2) -1] = '\0';
	printf("\t INGRESE EL NOMBRE DEL TERCER POKEMON \n");
	fgets(*nombre3,(int)maximo_tamano,stdin);
	(*nombre3)[strlen(*nombre3) -1] = '\0';
	return;
}
jugada_t jugador_pedir_nombre_y_ataque(){
	printf("\t INGRESE EL NOMBRE DEL POKEMON A UTILIZAR EN LA RONDA \n");
	char pokemon[100];
	fgets(pokemon,sizeof(pokemon),stdin);
	pokemon[strlen(pokemon) -1] = '\0';
	printf("\t INGRESE EL NOMBRE DEL ATAQUE A UTILIZAR EN LA RONDA \n");
	char ataque[100];
	fgets(ataque,sizeof(ataque),stdin);
	ataque[strlen(ataque) -1] = '\0';

	jugada_t jugada;
	strcpy(jugada.ataque,ataque);
	strcpy(jugada.pokemon,pokemon);
	return jugada;
}
int main(int argc, char *argv[])
{
	juego_t *juego = juego_crear();

	//Pide al usuario un nombre de archivo de pokemones
	char *archivo = pedir_archivo();

	//Carga los pokemon
	JUEGO_ESTADO estado_carga = juego_cargar_pokemon(juego, archivo);
	if(estado_carga == ERROR_GENERAL){
		printf("\t ERROR AL CARGAR ARCHIVO \n");
		juego_destruir(juego);
		return 0;
	}
	if(estado_carga == POKEMON_INSUFICIENTES){
		printf("\t ERROR POR FALTA DE POKEMONES \n");
		juego_destruir(juego);
		return 0;
	}

	//Crea un adversario que será utilizado como jugador 2
	lista_t *lista = juego_listar_pokemon(juego);
	adversario_t *adversario =
		adversario_crear(lista);

	//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
	mostrar_pokemon_disponibles(lista);

	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
	
	char *eleccionJugador1, *eleccionJugador2, *eleccionJugador3;
	pedir_pokemones_jugador(&eleccionJugador1,&eleccionJugador2,&eleccionJugador3);
	if(eleccionJugador1 == NULL || eleccionJugador2 == NULL || eleccionJugador3 == NULL){
		juego_destruir(juego);
		adversario_destruir(adversario);
		return 0;
	}
	//pedirle al adversario que indique los 3 pokemon que quiere usar
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
	printf("Pokemones Jugador:   \t Pokemones Adversario \n");
	printf("%s    \t         %s\n",eleccionJugador1,eleccionAdversario1);
	printf("%s    \t         %s\n",eleccionJugador2,eleccionAdversario2);
	printf("%s    \t         %s\n",eleccionAdversario3,eleccionJugador3);
	//Seleccionar los pokemon de los jugadores
	JUEGO_ESTADO estado_seleccion = juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	while (estado_seleccion != TODO_OK)
	{
		if(estado_seleccion == POKEMON_REPETIDO){
			printf("NO SE PUEDEN INGRESAR POKEMONES REPETIDOS");
		}else if(estado_seleccion == POKEMON_INEXISTENTE){
			printf("NO SE PUEDEN INGRESAR POKEMONES INEXISTENTES");
		}
		free(eleccionJugador1);
		free(eleccionJugador2);
		free(eleccionJugador3);
		pedir_pokemones_jugador(&eleccionJugador1,&eleccionJugador2,&eleccionJugador3);
		if(eleccionJugador1 == NULL || eleccionJugador2 == NULL || eleccionJugador3 == NULL){
			juego_destruir(juego);
			adversario_destruir(adversario);
			return 0;
		}
		estado_seleccion = juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	}
	
	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1,
				  eleccionAdversario2, eleccionAdversario3);

	//informarle al adversario cuales son los pokemon del jugador
	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
					eleccionJugador2, eleccionJugador3);
	int rondas = 1; 
	while (!juego_finalizado(juego)) {
		resultado_jugada_t resultado_ronda;
		jugada_t jugada_jugador;
		jugada_t jugada_adversario;
		printf("Ronda %d \n",rondas);
		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
		inicio:

		jugada_jugador = jugador_pedir_nombre_y_ataque();

		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
		jugada_adversario =
			adversario_proxima_jugada(adversario);

		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
						    jugada_adversario);
		printf("%s \n",jugada_jugador.pokemon);
		printf("%s \n",jugada_jugador.ataque);
		if(resultado_ronda.jugador1 == ATAQUE_ERROR){
			printf("ERROR AL ELEGIR POKEMON Y ATAQUE");
			goto inicio;
		}
		rondas++;
		printf(" \t \t Puntaje Jugador: %d \n",juego_obtener_puntaje(juego,JUGADOR1));
		printf("\t\t Puntaje Adeversario: %d \n",juego_obtener_puntaje(juego,JUGADOR2));
		
		//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
		adversario_informar_jugada(adversario, jugada_jugador);
	}

	juego_destruir(juego);
}
