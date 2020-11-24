/*
 * utn.h
 *
 *  Created on: 3 sep. 2020
 *      Author: Flores E. Federico
 */

#ifndef UTN_H_
#define UTN_H_

int imprimeArrayEntero(int pArray[],int longitud);
int imprimeArrayFlotante(float pArray[],int longitud);
void insertion(int array[],int len);
int initArrayEntero(int pArray[],int longitud, int valorInicial);
int initArrayFlotante(float pArray[],int longitud, float valorInicial);

//---validacion de datos----
int utn_getNumero(int* pResultado,char* mensaje,char* mensajeError,int mininimo,int maximo,int reintentos);
int utn_getNumeroFloat(float* pResultado,char* mensaje,char* mensajeError,float minimo,float maximo,int reintentos);
int utn_getResponse(char* pResultado,char* mensaje,char* mensajeError,int reintentos);
int utn_getCaracter(char* pResultado,char* mensaje,char* mensajeError,int minimo,int maximo,int reintentos);
int utn_getCadena(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getNombre(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getDni(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getCuit(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getPassword(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getDireccion(char* pResultado,int longitud,char* mensaje,char* mensajeError,int reintentos);
int utn_getNumeroTxt(char* pResultado,int longitud,char* mensaje,char* mensajeError,int min,int max,int reintentos);
int esAlfaNumerica(char* pResultado);
int esTelefono(char* pResultado);
#endif /* UTN_H_ */
