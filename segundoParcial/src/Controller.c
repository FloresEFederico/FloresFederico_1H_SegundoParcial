#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "clientes.h"
#include "ventas.h"
#include "parser.h"
#include "informes.h"
#include <string.h>

/** \brief Carga los datos de los envios desde el archivo clientes.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int controller_cargarArchivoClientes(char* path , LinkedList* this)
{
	int retorno = -1;

	FILE* pFile = fopen(path,"r");

	if(pFile != NULL && path != NULL && this)
	{
		if(!parser_ClienteDesdeTexto(pFile,this))
		{
			retorno = 0;
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief Guarda los datos de los envios en el archivo clientes.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int controller_guardarArchivoClientes(char* path , LinkedList* this)
{
	int retorno = -1;
	FILE* pFile = fopen(path,"w");
	if(pFile != NULL && path != NULL && this != NULL && ll_len(this) > 0)
	{
		if(!parser_ClienteATexto(pFile,this))
		{
			retorno = 0;
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief Carga los datos de los envios desde el archivo ventas.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int controller_cargarArchivoVentas(char* path , LinkedList* this)
{
	int retorno = -1;

	FILE* pFile = fopen(path,"r");

	if(pFile != NULL && path != NULL && this)
	{
		if(!parser_VentaDesdeTexto(pFile,this))
		{
			retorno = 0;
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief Guarda los datos de los envios en el archivo ventas.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int controller_guardarArchivoVentas(char* path , LinkedList* this)
{
	int retorno = -1;
	FILE* pFile = fopen(path,"w");
	if(pFile != NULL && path != NULL && this != NULL && ll_len(this) > 0)
	{
		if(!parser_VentaATexto(pFile,this))
		{
			retorno = 0;
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief Guarda los datos de los envios en el archivo a cobrar.csv o cobrados.csv dependiendo del estado de venta que se le haya pasado por parametro (modo texto).
 *
 * \param path char*
 * \param pArrayListVentas LinkedList*
 * \param pArrayListClientes LinkedList*
 * \param estado int;
 * \return int
 *
 */
int controller_generarInformeDeCliente(char* path,LinkedList* pArrayListVentas,LinkedList* pArrayListClientes,int estado)
{
	int retorno = -1;
	FILE* pFile = fopen(path,"w");
	if(pFile != NULL && path != NULL && pArrayListVentas != NULL && pArrayListClientes != NULL && ll_len(pArrayListVentas) > 0 && ll_len(pArrayListClientes) > 0)
	{
		if(!parser_InformeATexto(pFile,pArrayListVentas,pArrayListClientes,estado))
		{
			retorno = 0;
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief imprime las estadisticas de venta: maxima y minima cantidad de afiches vendidos y mayor cantidad de afiches vendidos por venta.
 *
 * \param pArrayListVentas LinkedList*
 * \param pArrayListClientes LinkedList*
 * \param estado int;
 * \return int
 *
 */
int controller_generarEstadisticas(LinkedList* pArrayListVentas,LinkedList* pArrayListClientes)
{
	int retorno = -1;
	if(pArrayListVentas != NULL && pArrayListClientes != NULL)
	{

		if(!informe_imprimirClienteDeMayoryMenorCantidadAfichesVendidos(pArrayListVentas,pArrayListClientes) &&
		   !informe_imprimirVentaConMasAfichesVendidos(pArrayListVentas,pArrayListClientes))
		{
			retorno = 0;
		}
	}
	return retorno;
}

