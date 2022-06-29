#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "LinkedList.h"
#include "Controller.h"
#include "Movie.h"
#include "Parser.h"
#include "Validaciones.h"
#include "Menu.h"

int controller_getPath(char* path)
{
	int todoOk = 0;

	if(path != NULL)
	{
		printf("\nSugerencia: utilice la extensi�n '.csv' o '.txt'\n");
		printf("Escriba el nombre del archivo: ");
		fflush(stdin);
		gets(path);
		if(strlen(path) == 0)
		{
			printf("\nNombre de archivo inv�lido\n\n");
			return todoOk;
		}
		validarPath(path);

		todoOk = 1;
	}

    return todoOk;
}

int controller_loadFromText(char* path , LinkedList* pArrayListMovie)
{
	int todoOk = 0;

	if(path != NULL && pArrayListMovie != NULL)
	{
		FILE* f = fopen(path, "r");

		if(f == NULL)
		{
			printf("Ese archivo no existe. Verifique si lo escribi� correctamente o si olvid� la extensi�n.\n\n");
			return todoOk;
		}

		parser_MovieFromText(f, pArrayListMovie);

		printf("\nArchivo '%s' cargado correctamente\n\n", path);
		fclose(f);
		todoOk = 1;
	}

    return todoOk;
}

int controller_ListMovie(LinkedList* pArrayListMovie)
{
	if(pArrayListMovie != NULL)
	{
		eMovie* auxMov = NULL;
		int flag = 0;

		printf("\n                       *** Listado de Peliculas ***\n\n");
		printf("  ID                          Titulo                Genero           Rating\n");
		printf("--------------------------------------------------------------------------------\n");
		for(int i = 0; i < ll_len(pArrayListMovie); i++)
		{
			auxMov = (eMovie*) ll_get(pArrayListMovie, i);
			if(auxMov != NULL)
			{
				mostrarPelicula(auxMov);
				flag = 1;
			}
		}
		if(flag == 0)
		{
			printf("                         No hay pel�culas cargadas\n\n");
		}
		printf("\n");
	}

    return 1;
}

int controller_saveAsText(char* path , LinkedList* pArrayListMovie)
{
	if(path != NULL && pArrayListMovie != NULL)
	{
		FILE* f = fopen(path, "r");

		if(f == NULL)
		{
			printf("\nEse archivo no existe, se crear� y se le guardar� la Nueva Lista\n");
		}

		f = fopen(path, "w");

		eMovie* auxMov = NULL;

		char generoString[20];
		char encabezado1[20] = "id_peli";
		char encabezado2[20] = "titulo";
		char encabezado3[20] = "genero";
		char encabezado4[20] = "rating";

		fprintf(f,"%s,%s,%s,%s \n", encabezado1, encabezado2, encabezado3, encabezado4);

		for(int i = 0; i < ll_len(pArrayListMovie); i++)
		{
			auxMov = (eMovie*) ll_get(pArrayListMovie, i);

			eMovie_getGeneroString(auxMov, generoString);

			fprintf(f,"%d,%s,%s,%f\n", auxMov->id, auxMov->titulo, generoString, auxMov->rating);
		}
		printf("\nLa Nueva Lista se guardo correctamente en el Archivo '%s'\n\n", path);
		fclose(f);
	}

    return 1;
}

int controller_filter(LinkedList* pArrayListMovie)
{
	int todoOk = 0;
	int generoSeleccionado;

	if(pArrayListMovie != NULL)
	{
		if(ll_len(pArrayListMovie) == 0)
		{
			printf("\nNo hay pel�culas en el sistema\n\n");
			return todoOk;
		}

		printf("\nG�neros disponibles\n");
		printf("1. Drama\n");
		printf("2. Comedia\n");
		printf("3. Accion\n");
		printf("4. Terror\n\n");

		printf("Seleccione el g�nero que quiera filtrar (1 - 4): ");
		scanf("%d", &generoSeleccionado);
		validarOpcion(&generoSeleccionado);

		switch(generoSeleccionado)
		{
		case 1:
		{
			LinkedList* listaPeliculasDrama = NULL;
			listaPeliculasDrama = ll_filter(pArrayListMovie, filtrarDrama);
			if(listaPeliculasDrama == NULL || ll_len(listaPeliculasDrama) == 0)
			{
				printf("No hay pel�culas del g�nero Drama.\n\n");
				ll_deleteLinkedList(listaPeliculasDrama);
			}else
			{
				controller_ListMovie(listaPeliculasDrama);
				controller_saveFilter("PeliculasDrama.txt", listaPeliculasDrama);
			}

			break;
		}

		case 2:
		{
			LinkedList* listaPeliculasComedia = NULL;
			listaPeliculasComedia = ll_filter(pArrayListMovie, filtrarComedia);
			if(listaPeliculasComedia == NULL || ll_len(listaPeliculasComedia) == 0)
			{
				printf("No hay pel�culas del g�nero Comedia.\n\n");
				ll_deleteLinkedList(listaPeliculasComedia);
			}else
			{
				controller_ListMovie(listaPeliculasComedia);
				controller_saveFilter("PeliculasComedia.txt", listaPeliculasComedia);
			}

			break;
		}
		case 3:
		{
			LinkedList* listaPeliculasAccion = NULL;
			listaPeliculasAccion = ll_filter(pArrayListMovie, filtrarAccion);
			if(listaPeliculasAccion == NULL || ll_len(listaPeliculasAccion) == 0)
			{
				printf("No hay pel�culas del g�nero Accion.\n\n");
				ll_deleteLinkedList(listaPeliculasAccion);
			}else
			{
				controller_ListMovie(listaPeliculasAccion);
				controller_saveFilter("PeliculasAccion.txt", listaPeliculasAccion);
			}

			break;
		}
		default:
		{
			LinkedList* listaPeliculasTerror = NULL;
			listaPeliculasTerror = ll_filter(pArrayListMovie, filtrarTerror);
			if(listaPeliculasTerror == NULL || ll_len(listaPeliculasTerror) == 0)
			{
				printf("No hay pel�culas del g�nero Terror.\n\n");
				ll_deleteLinkedList(listaPeliculasTerror);
			}else
			{
				controller_ListMovie(listaPeliculasTerror);
				controller_saveFilter("PeliculasTerror.txt", listaPeliculasTerror);
			}

			break;
		}
		}
	}

	return todoOk;
}
int controller_saveFilter(char* path , LinkedList* pArrayFilterList)
{
	if(path != NULL && pArrayFilterList != NULL)
	{
		FILE* f = fopen(path, "r");

		if(f == NULL)
		{
			printf("\nSe cre� un nuevo archivo llamado '%s' con esta lista.\n\n", path);
		}else
		{
			printf("\nSe carg� esta lista en el archivo llamado '%s'\n\n", path);
		}

		f = fopen(path, "w");
		eMovie* auxMov = NULL;

		char generoString[20];

		char encabezado1[20] = "id_peli";
		char encabezado2[20] = "titulo";
		char encabezado3[20] = "genero";
		char encabezado4[20] = "rating";

		fprintf(f,"%s,%s,%s,%s \n", encabezado1, encabezado2, encabezado3, encabezado4);

		for(int i = 0; i < ll_len(pArrayFilterList); i++)
		{
			auxMov = (eMovie*) ll_get(pArrayFilterList, i);

			eMovie_getGeneroString(auxMov, generoString);

			fprintf(f,"%d,%s,%s,%f\n", auxMov->id, auxMov->titulo, generoString, auxMov->rating);
		}

		fclose(f);
	}

    return 1;
}
