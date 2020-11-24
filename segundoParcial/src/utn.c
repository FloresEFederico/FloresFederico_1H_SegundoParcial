/*
 * utn.c
 *
 *  Created on: 3 sep. 2020
 *      Author: Flores E. Federico
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int getInt(int* pResultado); //utilizada en utn_getNumero
static int esNumerica(char* cadena,int limite);//utilizada en getInt
static int myGets(char* cadena,int longitud);//utilizada en getInt, getFloat y getString
static int getFloat(float* pFloat);// utilizada en utn_getNumeroFloat
static int esFlotante(char* array,int limite); // utilizada en getFloat
static int getString(char* pResultado,int longitud); //utilizada en utn_getCadena
static int esSoloLetras(char* pResultado,int limite); //utilizada en getString
static int getName(char* pResultado,int longitud); //utilizada en utn_getNombre
static int esNombre(char* pResultado,int limite); //utilizada en getNombre
static int toNombre(char text[],int len); //utilizada en getNombre
static int getDni(char* pResultado,int longitud); //utilizada en utn_getDni
static int getCuit(char* pResultado,int longitud); //utilizada en utn_getCuit
static int esDni(char* pResultado,int limite); //utilizada en getDni
static int esCuit(char* pResultado,int limite); //utilizada en getCuit
static int getDireccion(char* pResultado,int longitud); // utilizada en utn_getDireccion
static int getPassword(char* pResultado,int longitud); // utilizada en utn_getPassword
static int esAlfaNumerica(char* pResultado); //utilizada en getDireccion
static int esTexto(char* pResultado); // utilizada en getDireccion
static int getNumeroTxt(char* pResultado,int longitud);

/*
 * utn_getNumero : pide al usuario un numero entero
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * minimo : valor minimo valido (inclusive)
 * maximo : valor maximo valido (inclusive)
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getNumero(int* pResultado,char* mensaje,char* mensajeError,int minimo,int maximo,int reintentos){
	int retorno = -1;
	int bufferInt;
	int resultadoScan;

	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && minimo <= maximo && reintentos > 0){
		do{
			printf("%s", mensaje);
			fflush(stdin);
			resultadoScan = getInt(&bufferInt);
			if(resultadoScan == 1 && bufferInt >= minimo && bufferInt <= maximo){
				*pResultado = bufferInt;
				retorno = 0;
				break;
			}else{
				printf("\n-REINTENTOS : %d\n",reintentos);
				printf("%s", mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}


int utn_getNumeroTxt(char* pResultado,int longitud,char* mensaje,char* mensajeError,int min,int max,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getNumeroTxt(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud &&
				min <= max &&
				atoi(bufferChar) >= min &&
				atoi(bufferChar) <= max)
			{
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

static int getNumeroTxt(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) && esNumerica(buffAux,sizeof(buffAux)))
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * getInt: pide un texto al usuario, lo almacena como cadena, valida y convierte el texto a numero y lo devuelve como int
 * presultado: puntero numero entero
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getInt(int* pResultado){
	int retorno = 0;
	char buffer[64];
	if(pResultado != NULL){

		if(!myGets(buffer,sizeof(buffer)) && esNumerica(buffer,sizeof(buffer))){
			*pResultado = atoi(buffer);
			retorno = 1;
		}
	}
	return retorno;
}

/**
* esNumerica: Verifica si la cadena ingresada es numerica
* cadena: cadena de caracteres a ser analizada
* limite: limite de la cadena
* Retorno: 1 (verdadero) si la cadena es numerica , 0 (falso) si no y -1 en caso de ERROR de parametro
*
*/
static int esNumerica(char* cadena,int limite){
	int retorno = 1; //ERROR
	int i = 0;
	if(cadena != NULL && limite > 0){
		retorno = 1; //VERDADERO
		if(cadena[0] == '-' || cadena[0] == '+'){
			i = 1;
		}
		for(;cadena[i] != '\0';i++){
			if(cadena[i] < '0' || cadena[i] > '9' ){
				retorno = 0; //FALSO
				break;
			}
		}
	}
	return retorno;
}

/**
* myGets: Lee de stdin hasta que encuentra un '\n' o hasta que haya copiado en cadena un máximo de ' longitud - 1' caracteres .
* cadena: Puntero al espacio de memoria donde se copiara la cadena obtenida
* longitud:  Define el tamaño de cadena
* Retorno: 0 (EXITO) si se obtiene una cadena y -1 (ERROR) si no
*
*/
static int myGets(char* cadena, int longitud){
	int retorno = -1; //ERROR
	char bufferString[4096];

	if(cadena != NULL && longitud > 0)
	{
		fflush(stdin);
		if(fgets(bufferString,sizeof(bufferString),stdin) != NULL)
		{
			if(bufferString[strnlen(bufferString,sizeof(bufferString))-1] == '\n')
			{
				bufferString[strnlen(bufferString,sizeof(bufferString))-1] = '\0';
			}
			if(strlen(bufferString) <= longitud)
			{
				strncpy(cadena,bufferString,longitud);
				retorno = 0; //EXITO
			}
		}

	}
	return retorno;
}

/*
 * utn_getNumeroFloat : Solicita un numero flotante al usuario, luego de verificarlo devuelve el resultado
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * minimo : valor minimo valido (inclusive)
 * maximo : valor maximo valido (inclusive)
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getNumeroFloat(float* pResultado,char* mensaje,char* mensajeError,float minimo,float maximo,int reintentos){
	int retorno = -1;
	float bufferFloat;
	int resultadoScan;
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && minimo <= maximo && reintentos > 0){
		do{
			printf("%s", mensaje);
			fflush(stdin);
			resultadoScan = getFloat(&bufferFloat);
			if(resultadoScan && minimo <= bufferFloat && maximo >= bufferFloat){
				*pResultado = bufferFloat;
				retorno = 0;
				break;
			}else{
				printf("%s", mensajeError);
				printf("\n-REINTENTOS: %d \n",reintentos);
				reintentos--;
			}

		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * getFloat: verifica si la cadena ingresada es flotante
 * pFloat: puntero numero float
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getFloat(float* pFloat){
	int retorno = 0;
	char bufferFloat[64];
	if(pFloat != NULL){
		fflush(stdin);

		if(!myGets(bufferFloat,sizeof(bufferFloat)) && esFlotante(bufferFloat,sizeof(bufferFloat))){
			*pFloat = atof(bufferFloat);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * esFlotante: Verifica si la cadena ingresada es flotante
 * cadena: cadena de caracteres a ser analizada
 * limite: limite de la cadena
 * Retorno: 1 (verdadero) si la cadena es flotante , 0 (falso) si no y -1 en caso de ERROR de parametro
 *
 */
static int esFlotante(char* array,int limite){
	int retorno = -1;
	int i = 0;
	int contadorDePuntos = 0;

	if(array != NULL && limite > 0){
		retorno = 1;
		if(array[0] != '+' || array[0] != '-'){
			i = 1;
		}
		while(array[i] != '\0'){
			if((array[i] < '0' || array[i] > '9') && array[i] != '.'){
				retorno = 0;
				break;
			}
			if(array[i] == '.'){
				contadorDePuntos++;
				if(contadorDePuntos > 1){
					retorno = 0;
					break;
				}
			}
			i++;
		}
	}
	return retorno;
}


/*
 * utn_getCaracter : pide al usuario una letra
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * minimo : valor minimo valido (inclusive)
 * maximo : valor maximo valido (inclusive)
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getCaracter(char* pResultado,char* mensaje,char* mensajeError,int minimo,int maximo,int reintentos){
	int retorno = -1;
	char bufferChar;
	int resultadoScan;

	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && minimo <= maximo && reintentos > 0){
		do{
			printf("%s", mensaje);
			fflush(stdin);
			resultadoScan = scanf("%c",&bufferChar);
			if(resultadoScan == 1 && bufferChar >= minimo && bufferChar <= maximo){
				*pResultado = bufferChar;
				retorno = 0;
				break;
			}else{
				printf("\n-REINTENTOS : %d\n",reintentos);
				printf("%s", mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * utn_getResponse : pide al usuario una letra y solo puede ser 's' o 'n'.
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getResponse(char* pResultado,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar;
	int resultadoScan;

	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s", mensaje);
			fflush(stdin);
			resultadoScan = scanf("%c",&bufferChar);
			if(resultadoScan == 1 && (bufferChar == 's' || bufferChar == 'n')){
				*pResultado = bufferChar;
				retorno = 0;
				break;
			}else{
				printf("\n-REINTENTOS : %d\n",reintentos);
				printf("%s", mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}



/*
 * utn_getString: pide al usuario un string
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getCadena(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos){
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getString(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * getString: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getString(char* pResultado,int longitud){
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) && esSoloLetras(buffAux,sizeof(buffAux))){
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * esSoloLetras: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por letras y/o espacio
 * pResultado: cadena de caracteres
 * limite: tamaño de la cadena
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esSoloLetras(char* pResultado,int limite){
	int respuesta = 1; // TODO OK
	int i;
	if(pResultado != NULL && limite > 0){
		for(i=0; i <= limite && pResultado[i] != '\0';i++){
			if((pResultado[i] < 'a' || pResultado[i] > 'z') &&
			   (pResultado[i] < 'A' || pResultado[i] > 'Z') &&
			   pResultado[i] != ' ')
			{
				respuesta = 0;
				break;
			}
		}
	}
	return respuesta;
}

/*
 * utn_getNombre: pide al usuario un nombre
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getNombre(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getName(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * utn_getDni: pide al usuario un Dni
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getDni(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getDni(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * utn_getCuit: pide al usuario un Cuit.
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getCuit(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getCuit(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}

/*
 * utn_getNombre: pide al usuario una contraseña
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getPassword(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getPassword(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}


/*
 * utn_getNombre: pide al usuario una direccion
 * pResultado : Direccion de memoria de la variable donde escribe el valor ingresado por el usuario
 * longitud: tamaño total del array de caracteres
 * mensaje : El mensaje que imprime para pedir un valor.
 * mensajeError: El mensaje que imprime si el rango no es valido.
 * Reintentos: cantidad de veces que tiene el usuario para ingresar un valor valido
 * Retorno: devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.
 *
 */
int utn_getDireccion(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos)
{
	int retorno = -1;
	char bufferChar[45];
	if(pResultado != NULL && mensaje != NULL && mensajeError != NULL && reintentos > 0){
		do{
			printf("%s",mensaje);
			if(getDireccion(bufferChar,sizeof(bufferChar)) &&
				strlen(bufferChar) < longitud){
				strncpy(pResultado,bufferChar,longitud-1);
				retorno = 0;
				break;
			}else{
				printf("%s",mensajeError);
				reintentos--;
			}
		}while(reintentos >= 0);
	}
	return retorno;
}


/*
 * getNombre: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getName(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) &&
		  esNombre(buffAux,sizeof(buffAux)) &&
		  toNombre(buffAux,sizeof(buffAux))
		  )
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * getDni: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getDni(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) &&
		  esDni(buffAux,sizeof(buffAux)))
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * getDni: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getCuit(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) &&
		  esCuit(buffAux,sizeof(buffAux)))
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}


/*
 * getPassWord: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getPassword(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) &&
			esAlfaNumerica(buffAux))
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}

/*
 * getDireccion: pide un texto al usuario, lo almacena como cadena, valida y lo devuelve
 * presultado: puntero a  array de caracter
 * longitud: tamaño del array
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int getDireccion(char* pResultado,int longitud)
{
	int retorno = 0;
	char buffAux[1000];
	if(pResultado != NULL && longitud > 0){
		fflush(stdin);
		if(!myGets(buffAux,sizeof(buffAux)) &&
			esTexto(buffAux))
		{
			strncpy(pResultado,buffAux,longitud);
			retorno = 1;
		}
	}
	return retorno;
}


/*
 * esNombre: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por letras
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esNombre(char* pResultado,int limite)
{
	int respuesta = 1; // TODO OK
	int i;

	if(pResultado != NULL && limite > 0){

		if(!strlen(pResultado))
		{
			respuesta = 0;
		}else
		{
			for(i=0; i <= limite && pResultado[i] != '\0';i++){
				if((pResultado[i] < 'a' || pResultado[i] > 'z') &&
				   (pResultado[i] < 'A' || pResultado[i] > 'Z'))
				{
					respuesta = 0;
					break;
				}
			}
		}

	}
	return respuesta;
}



/*
 * esDni: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por numeros y/o '-'
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esDni(char* pResultado,int limite)
{
	int respuesta = 1;
	int i;
	int longitudDeDni = strlen(pResultado);

	if(pResultado != NULL && limite > 0){
		if(!strlen(pResultado))
		{
			respuesta = 0;
		}else
		{
			for(i=0; i <= limite && pResultado[i] != '\0';i++){
				if((pResultado[i] < '0' || pResultado[i] > '9') || (longitudDeDni != 8))
				{
					respuesta = 0;
					break;
				}
			}
		}
	}
	return respuesta;
}

/*
 * esCuit: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por numeros y/o '-'
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esCuit(char* pResultado,int limite)
{
	int respuesta = 1;
	int i;
	int longitudDeDni = strlen(pResultado);

	if(pResultado != NULL && limite > 0){
		if(!strlen(pResultado))
		{
			respuesta = 0;
		}else
		{
			for(i=0; i <= limite && pResultado[i] != '\0';i++){
				if((pResultado[i] < '0' || pResultado[i] > '9') || longitudDeDni != 13)
				{
					if((i== 2 && pResultado[i] == '-') || (i== 11 && pResultado[i] == '-') )
					{
						continue;
					}else
					{
						respuesta = 0;
						break;
					}
				}
			}
		}
	}
	return respuesta;
}


/*
 * toNombre: Recibe una cadena de caracteres, convirtiendo la primera letra en mayuscula y las demas en minuscula
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int toNombre(char text[],int len)
{
	int retorno = 0;
	int i;
	if(text != NULL && len > 0)
	{
		for(i=0;i<len && text[i] != '\0';i++)
		{
			if(i!=0)
			{
				text[i] = tolower(text[i]);
			}else
			{
				text[i] = toupper(text[i]);
			}
		}
		retorno = 1;
	}
	return retorno;
}

/*
 * esAlfaNumerica: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por letras y/o espacio y/o numeros
 * pResultado: cadena de caracteres
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esAlfaNumerica(char* pResultado)
{
	int retorno = 1;
	int i;
	if(pResultado != NULL){
		for(i=0;pResultado[i] != '\0';i++){
			if((pResultado[i] < 'a' || pResultado[i] > 'z') &&
			   (pResultado[i] < 'A' || pResultado[i] > 'Z') &&
			   (pResultado[i] < '0' || pResultado[i] > '9')){
				retorno = 0;
				break;
			}
		}
	}
	return retorno;
}

/*
 * esTexto: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por letras y/o espacio y/o numeros y/o signos de exclamacion/interrogacion
 * pResultado: cadena de caracteres
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int esTexto(char* pResultado)
{
	int retorno = 1;
	int i;
	if(pResultado != NULL){
		for(i=0;pResultado[i] != '\0';i++){
			if((pResultado[i] != '_') &&
			   (pResultado[i] != '-') &&
			   (pResultado[i] < 'a' || pResultado[i] > 'z') &&
			   (pResultado[i] < 'A' || pResultado[i] > 'Z') &&
			   (pResultado[i] < '0' || pResultado[i] > '9')){
				retorno = 0;
				break;
			}
		}
	}
	return retorno;
}

/*
 * esTelefono: Recibe una cadena de caracteres y devuelve 1 en caso de que el texto este compuesto solo por numeros y/o guionrd
 * pResultado: cadena de caracteres
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
int esTelefono(char* pResultado){
	int retorno = 1;
	int i;
	int numeroDeGuiones = 0;
	if(pResultado != NULL){
		for(i=0;pResultado[i] != '\0';i++){
			if(((pResultado[i] != '-') && (pResultado[i] < '0' || pResultado[i] > '9')) || (numeroDeGuiones > 1)){
				retorno = 0;
				break;
			}
			if(pResultado[i] == '-'){
				numeroDeGuiones++;
			}
		}
	}
	return retorno;
}

/*
 * insertion: burbujeo de de un array de enteros
 * array: array de enteros
 * len: longitud de array
 * Retorno: VOID
 */
void insertion(int array[],int len){
	int i;
	int j;
	int temp;
	for(i=1;i<len;i++){
		temp = array[i];
		j = i - 1;
		while(j >= 0 && temp < array[j]){ 	// temp<array[j] o temp>array[j]
			array[j+1] = array[j];
			j--;
		}
		array[j+1] = temp; //insercion
	}
}


/*
 * \brief 	imprime un array de enteros
 * \param pArray Puntero al array
 * \param longitud Define el tamaño del array
 * \returtn retorna 0 {EXITO} o -1 {ERROR}
 */
int imprimeArrayEntero(int pArray[],int longitud){
	int retorno = -1;
	int i;
	if(pArray != NULL && longitud > 0){
		for(i=0;i<longitud;i++){
			printf("\nIndice: [%d] - Valor: %d",i,pArray[i]);
		}
		retorno = 0;
	}
	return retorno;
}

/*
 * \brief 	imprime un array de flotantes
 * \param pArray Puntero al array
 * \param longitud Define el tamaño del array
 * \returtn retorna 0 {EXITO} o -1 {ERROR}
 */
int imprimeArrayFlotante(float pArray[],int longitud){
	int retorno = -1;
	int i;
	if(pArray != NULL && longitud > 0){
		for(i=0;i<longitud;i++){
			printf("\nIndice: [%d] - Valor: %.2f",i,pArray[i]);
		}
		retorno = 0;
	}
	return retorno;
}


/*
 * \brief 	inicializa un array de enteros
 * \param pArray Puntero al array
 * \param longitud Define el tamaño del array
 * \param valorInicial Es el valor a ser cargado en todas las posiciones del array
 * \returtn retorna 0 {EXITO} o -1 {ERROR}
 */
int initArrayEntero(int pArray[],int longitud, int valorInicial){
	int retorno = -1;
	int i;
	if(pArray != NULL && longitud > 0){
		for(i=0;i<longitud;i++){
			pArray[i] = valorInicial;
		}
		retorno = 0;
	}
	return retorno;
}


/*
 * \brief 	inicializa un array de flotantes
 * \param pArray Puntero al array
 * \param longitud Define el tamaño del array
 * \param valorInicial Es el valor a ser cargado en todas las posiciones del array
 * \returtn retorna 0 {EXITO} o -1 {ERROR}
 */
int initArrayFlotante(float pArray[],int longitud, float valorInicial){
	int retorno = -1;
	int i;
	if(pArray != NULL && longitud > 0){
		for(i=0;i<longitud;i++){
			pArray[i] = valorInicial;
		}
		retorno = 0;
	}
	return retorno;
}
