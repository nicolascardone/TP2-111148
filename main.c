#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
		free(archivo);
		juego_destruir(juego);
		return 0;
	}
	if(estado_carga == POKEMON_INSUFICIENTES){
		printf("\t ERROR POR FALTA DE POKEMONES \n");
		free(archivo);
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
		free(archivo);
		adversario_destruir(adversario);
		free(eleccionJugador1);
		free(eleccionJugador2);
		free(eleccionJugador3);
		return 0;
	}
	//pedirle al adversario que indique los 3 pokemon que quiere usar
	
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
			free(archivo);
			free(eleccionJugador1);
			free(eleccionJugador2);
			free(eleccionJugador3);
			return 0;
		}
		estado_seleccion = juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	}
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
	
	printf("Pokemones Jugador:   \t Pokemones Adversario \n");
	printf("%s    \t         %s\n",eleccionJugador1,eleccionAdversario1);
	printf("%s    \t         %s\n",eleccionJugador2,eleccionAdversario2);
	printf("%s    \t         %s\n",eleccionAdversario3,eleccionJugador3);
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
		printf("\n");
		printf("Ronda: %d \n",rondas);
		jugada_jugador = jugador_pedir_nombre_y_ataque();
		jugada_adversario =
				adversario_proxima_jugada(adversario);
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
							jugada_adversario);
		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
		while(resultado_ronda.jugador1 == ATAQUE_ERROR){
			printf("ERROR ELEGIENDO ATAQUE Y POKEMON \n");
			jugada_jugador = jugador_pedir_nombre_y_ataque();

			//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
			resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
								jugada_adversario);
		}
		rondas++;
		printf(" \t \t Puntaje Jugador: %d \n",juego_obtener_puntaje(juego,JUGADOR1));
		printf("\t\t Puntaje Adeversario: %d \n",juego_obtener_puntaje(juego,JUGADOR2));
		printf("JUGADA JUGADOR:  \t JUGADA ADVERSARIO: \n");
		printf("%s \t\t\t %s\n",jugada_jugador.pokemon,jugada_adversario.pokemon);
		printf("%s \t\t\t %s\n",jugada_jugador.ataque,jugada_adversario.ataque);
		printf("============================================================================================== \n");
		
		//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
		adversario_informar_jugada(adversario, jugada_jugador);
	}

	if(juego_obtener_puntaje(juego,JUGADOR1) > juego_obtener_puntaje(juego,JUGADOR2)){
		printf("FELICITACIONES GANASTE \n");
	}else if(juego_obtener_puntaje(juego,JUGADOR2) > juego_obtener_puntaje(juego,JUGADOR1)){
		printf("PERDISTE, SUERTE LA PROXIMA \n");
	}else{
		printf("EMPATE \n");
	}
	printf(" \t PUNTAJE FINAL: \n");
	printf(" \t \t Puntaje Jugador: %d \n",juego_obtener_puntaje(juego,JUGADOR1));
	printf("\t\t Puntaje Adeversario: %d \n",juego_obtener_puntaje(juego,JUGADOR2));
	free(eleccionJugador1);
	free(eleccionJugador2);
	free(eleccionJugador3);
	free(eleccionAdversario1);
	free(eleccionAdversario2);
	free(eleccionAdversario3);
	free(archivo);
	adversario_destruir(adversario);
	juego_destruir(juego);
	return 0;
}
