/*
 ============================================================================
 Name        : segundoParcial.c
 Author      : Flores Federico 1-H
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utn.h"
#include "clientes.h"
#include "ventas.h"
#include "informes.h"
#include "Controller.h"
#include "LinkedList.h"
int main(void) {
	setbuf(stdout,NULL);
	LinkedList* listaDeClientes = ll_newLinkedList();
	LinkedList* listaDeVentas = ll_newLinkedList();
	int option;
	if(!controller_cargarArchivoClientes("clientes.csv",listaDeClientes) && !controller_cargarArchivoVentas("ventas.csv",listaDeVentas))
	{
		do
		{
			if(!utn_getNumero(&option,"\n-SISTEMA VENTA DE AFICHES-\n"
									  "1-Agregar Cliente.\n"
									  "2-Vender Afiches.\n"
									  "3-Modificar Venta.\n"
									  "4-Cobrar Venta.\n"
									  "5-Generar Informe de Cobros.\n"
									  "6-Generar Informe de Deudas.\n"
									  "7-Generar Estadisticas.\n"
									  "8-Salir.\nElija una Opcion(1-8): ","\nOpcion invalida!\n",1,8,3))
			{
				switch(option)
				{
				case 1:
					if(!cliente_altaCliente(listaDeClientes) && !controller_guardarArchivoClientes("clientes.csv",listaDeClientes))
					{
						printf("-----------------------------------------------\n");
					}else
					{
						printf("\nHubo un error al agregar un Cliente!\n");
					}
					break;
				case 2:
					if(!venta_altaVenta(listaDeVentas,listaDeClientes) && !controller_guardarArchivoVentas("ventas.csv",listaDeVentas))
					{
						printf("-----------------------------------------------\n");
					}else
					{
						printf("\nHubo un error al vender afiches\n");
					}
					break;
				case 3:
					if(!venta_modificarVenta(listaDeVentas,listaDeClientes) && !controller_guardarArchivoVentas("ventas.csv",listaDeVentas))
					{
						printf("-----------------------------------------------\n");
					}else
					{
						printf("\nHubo un error al modificar la venta\n");
					}
					break;
				case 4:
					if(!venta_cobrarVenta(listaDeVentas,listaDeClientes) && !controller_guardarArchivoVentas("ventas.csv",listaDeVentas))
					{
						printf("-----------------------------------------------\n");
					}else
					{
						printf("\nHubo un error al cambiar el estado de la venta!\n");
					}
					break;
				case 5:
					if(!controller_generarInformeDeCliente("cobrados.csv",listaDeVentas,listaDeClientes,COBRADO))
					{
						printf("Se genero el archivo exitosamente\n");
					}else
					{
						printf("\nHubo un error al generar el archivo!\n");
					}
					break;
				case 6:
					if(!controller_generarInformeDeCliente("a cobrar.csv",listaDeVentas,listaDeClientes,A_COBRAR))
					{
						printf("Se genero el archivo exitosamente\n");
					}else
					{
						printf("\nHubo un error al generar el archivo!\n");
					}
					break;
				case 7:
					if(!controller_generarEstadisticas(listaDeVentas,listaDeClientes))
					{
						printf("-----------------------------------------------\n");
					}else
					{
						printf("\nHubo un error al generar la estadistica!\n");
					}
					break;
				}
			}
		}while(option != 8);
	}
	printf("\nHasta luego!\n");
	return EXIT_SUCCESS;
}
