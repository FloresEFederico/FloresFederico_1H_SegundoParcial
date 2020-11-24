#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "clientes.h"
#include "ventas.h"
#include "informes.h"

/** \brief Parsea los datos los datos de los envios desde el archivo clientes.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int parser_ClienteDesdeTexto(FILE* pFile , LinkedList* this)
{
	int retorno = -1;
	Cliente* pElement;
	char auxIdCliente[2000];
	char auxNombre[2000];
	char auxApellido[2000];
	char auxCuit[2000];
	if(pFile != NULL && this)
	{

		fscanf(pFile,"%[^,],%[^,],%[^,],%[^\n]\n",auxIdCliente,auxNombre,auxApellido,auxCuit);
		do
		{
			if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^\n]\n",auxIdCliente,auxNombre,auxApellido,auxCuit) == 4)
			{
				pElement = cliente_newParametros(atoi(auxIdCliente),auxNombre,auxApellido,auxCuit);
				if(pElement != NULL)
				{
					ll_add(this,pElement);
				}
			}

		}while(!feof(pFile));
		retorno = 0;
	}
    return retorno;
}

/** \brief Guarda los datos del cliente de la lista al archivo clientes.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int parser_ClienteATexto(FILE* pFile , LinkedList* this)
{
	int retorno = -1;
	Cliente* pElement;
	char auxIdCliente[2000];
	char auxNombre[2000];
	char auxApellido[2000];
	char auxCuit[2000];
	if(pFile != NULL && this != NULL)
	{
		fprintf(pFile,"id_cliente,nombre_cliente,apellido_cliente,cuit_cliente\n");
		for(int i=0;i<ll_len(this);i++)
		{
			pElement = ll_get(this,i);
			if(pElement != NULL &&
				!cliente_getIdTxt(pElement,auxIdCliente) &&
				!cliente_getNombre(pElement,auxNombre) &&
				!cliente_getApellido(pElement,auxApellido) &&
				!cliente_getCuit(pElement,auxCuit))
			{
				fprintf(pFile,"%s,%s,%s,%s\n",auxIdCliente,auxNombre,auxApellido,auxCuit);
			}
		}
		retorno = 0;
	}
	return retorno;
}

/** \brief Parsea los datos los datos de las ventas desde el archivo ventas.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int parser_VentaDesdeTexto(FILE* pFile , LinkedList* this)
{
	int retorno = -1;
	Venta* pElement;
	char auxId[2000];
	char auxNombreArchivo[2000];
	char auxCantidad[2000];
	char auxZona[2000];
	int auxZonaNumero;
	char auxEstado[2000];
	int auxEstadoNumero;
	char auxIdCliente[2000];
	if(pFile != NULL && this)
	{
		fscanf(pFile,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",auxId,auxNombreArchivo,auxCantidad,auxZona,auxEstado,auxIdCliente);
		do
		{
			if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",auxId,auxNombreArchivo,auxCantidad,auxZona,auxEstado,auxIdCliente) == 6 &&
			   !venta_obtenerNumeroDeEstado(&auxEstadoNumero,auxEstado) &&
			   !venta_obtenerNumeroDeZona(&auxZonaNumero,auxZona))
			{
				pElement = venta_newParametros(atoi(auxId),auxNombreArchivo,atoi(auxCantidad),auxZonaNumero,auxEstadoNumero,atoi(auxIdCliente));
				if(pElement != NULL)
				{
					ll_add(this,pElement);
				}
			}
		}while(!feof(pFile));
		retorno = 0;
	}
    return retorno;
}

/** \brief Guarda los datos de la venta de la lista al archivo ventas.csv (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int parser_VentaATexto(FILE* pFile , LinkedList* this)
{
	int retorno = -1;
	Venta* pElement;
	char auxId[2000];
	char auxNombreArchivo[2000];
	char auxCantidad[2000];
	int auxZonaDePago;
	char auxNombreDeZona[2000];
	int auxEstado;
	char auxNombreDeEstado[2000];
	char auxIdCliente[2000];
	if(pFile != NULL && this != NULL)
	{
		fprintf(pFile,"id_venta,nombre_archivo,cantidad_afiches,zona_pago,estado_venta,id_cliente\n");
		for(int i=0;i<ll_len(this);i++)
		{
			pElement = ll_get(this,i);
			if(pElement != NULL &&
				!venta_getIdTxt(pElement,auxId) &&
				!venta_getNombreArchivo(pElement,auxNombreArchivo) &&
				!venta_getCantidadDeAfichesTxt(pElement,auxCantidad) &&
				!venta_getZonaDePago(pElement,&auxZonaDePago) &&
				!venta_obtenerNombreDeZona(auxZonaDePago,auxNombreDeZona) &&
				!venta_getEstado(pElement,&auxEstado) &&
				!venta_obtenerNombreDeEstado(auxEstado,auxNombreDeEstado) &&
				!venta_getIdClienteTxt(pElement,auxIdCliente))
			{
				fprintf(pFile,"%s,%s,%s,%s,%s,%s\n",auxId,auxNombreArchivo,auxCantidad,auxNombreDeZona,auxNombreDeEstado,auxIdCliente);
			}
		}
		retorno = 0;
	}
	return retorno;
}

/** \brief Guarda los datos de la venta de la lista al archivo a cobrar.csv o cobrados.csv segun el estado que se haya pasado por parametro (modo texto).
 *
 * \param path char*
 * \param this LinkedList*
 * \return int
 *
 */
int parser_InformeATexto(FILE* pFile,LinkedList* pArrayListVentas,LinkedList* pArrayListClientes,int estado)
{
	int retorno = -1;
	Cliente* pCliente;
	Informe pAuxInforme;
	int auxIdClient;
	char auxIdCliente[2000];
	char auxNombre[2000];
	char auxApellido[2000];
	char auxCuit[2000];
	int cantidadDeVentasCobradas;
	char bufferCantidad[20];
	if(pFile != NULL && pArrayListVentas != NULL && pArrayListClientes)
	{
		if(!estado)
		{
			fprintf(pFile,"id_cliente,nombre_cliente,apellido_cliente,cuit_cliente,cant_ventasACobrar\n");
		}else
		{
			fprintf(pFile,"id_cliente,nombre_cliente,apellido_cliente,cuit_cliente,cant_ventascobradas\n");
		}
		pAuxInforme.estadoDeVenta = estado;
		for(int i=0;i<ll_len(pArrayListClientes);i++)
		{
			pCliente = ll_get(pArrayListClientes,i);
			if(pCliente != NULL &&
				!cliente_getIdTxt(pCliente,auxIdCliente) &&
				!cliente_getNombre(pCliente,auxNombre) &&
				!cliente_getApellido(pCliente,auxApellido) &&
				!cliente_getCuit(pCliente,auxCuit) &&
				!cliente_getId(pCliente,&auxIdClient))
			{
				pAuxInforme.idCliente = auxIdClient;
				cantidadDeVentasCobradas = ll_reduceInt2(pArrayListVentas,informe_cantidadDeVentasPorClienteSegunEstadoDeVenta,&pAuxInforme);
				sprintf(bufferCantidad,"%d",cantidadDeVentasCobradas);
				fprintf(pFile,"%s,%s,%s,%s,%s\n",auxIdCliente,auxNombre,auxApellido,auxCuit,bufferCantidad);
			}
		}
		retorno = 0;
	}
	return retorno;
}
