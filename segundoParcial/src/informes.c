#include <stdio.h>
#include <stdlib.h>
#include "informes.h"
#include <string.h>

/** \brief calcula la cantidad de ventas en estado a cobrar o cobrada que tenga cada cliente.
 *
 * \param void* this: puntero a void
 * \param void* pInforme: puntero a void
 * \return 1 si se encontro la venta, 0 en el caso de que no.
 *
 */
int informe_cantidadDeVentasPorClienteSegunEstadoDeVenta(void* this,void* pInforme)
{
	Venta* pVenta = (Venta*)this;
	Informe* pInfo = (Informe*)pInforme;
	int respuesta = 0;
	int auxIdCliente;
	int auxEstado;
	if(this != NULL && this != NULL)
	{
		if(!venta_getIdCliente(pVenta,&auxIdCliente) &&
		   auxIdCliente == pInfo->idCliente &&
		   !venta_getEstado(pVenta,&auxEstado) &&
		   auxEstado == pInfo->estadoDeVenta)
		{
			respuesta = 1;
		}
	}
	return respuesta;
}

/** \brief imprime la mayor y menor cantidad de afiches vendidos por cliente.
 *
 * \param LinkedList* pArrayListVentas
 * \param LinkedList* pArrayListClientes
 * \return 0 si TodoOk, -1 si ERROR.
 */
int informe_imprimirClienteDeMayoryMenorCantidadAfichesVendidos(LinkedList* pArrayListVentas,LinkedList* pArrayListClientes)
{
	int retorno = -1;
	int cantidadDeAfichesVendidos = 0;
	int i;
	Informe pAuxInfo;
	Cliente* pCliente;
	int mayorCantidad;
	int indiceClienteMayor;
	int menorCantidad;
	int indiceClienteMenor;
	int auxIdCliente;
	if(pArrayListVentas != NULL && pArrayListClientes != NULL)
	{
		pAuxInfo.estadoDeVenta = COBRADO;
		for(i=0;i<ll_len(pArrayListClientes);i++)
		{
			pCliente = ll_get(pArrayListClientes,i);
			if(!cliente_getId(pCliente,&pAuxInfo.idCliente))
			{
				cantidadDeAfichesVendidos = ll_reduceInt2(pArrayListVentas,informe_cantidadDeAfichesPorClienteSegunEstadoDeVenta,&pAuxInfo);
				if(i==0 || mayorCantidad < cantidadDeAfichesVendidos)
				{
					mayorCantidad = cantidadDeAfichesVendidos;
					indiceClienteMayor = i;
				}
				if(i== 0 || menorCantidad > cantidadDeAfichesVendidos)
				{
					menorCantidad = cantidadDeAfichesVendidos;
					indiceClienteMenor = i;
				}
			}
		}
		pCliente = ll_get(pArrayListClientes,indiceClienteMayor);
		if(!cliente_getId(pCliente,&auxIdCliente))
		{
			printf("\nEl Cliente de id %d tiene la mayor cantidad de afiches vendidos.\nSiendo la cantidad total: %d\n",auxIdCliente,mayorCantidad);
		}
		pCliente = ll_get(pArrayListClientes,indiceClienteMenor);
		if(!cliente_getId(pCliente,&auxIdCliente))
		{
			printf("\nEl Cliente de id %d tiene la menor cantidad de afiches vendidos.\nSiendo la cantidad total: %d\n",auxIdCliente,menorCantidad);
		}
		retorno = 0;
	}
	return retorno;
}

/** \brief retorna la cantidad de afiches de ventas cobradas que tenga un cliente.
 *
 * \param void* this
 * \param void* pInforme
 * \return la cantidad de afiches del cliente si TodoOk, 0 si el cliente no tiene ninguna venta.
 */
int informe_cantidadDeAfichesPorClienteSegunEstadoDeVenta(void* this,void* pInforme)
{
	Venta* pVenta = (Venta*)this;
	Informe* pInfo = (Informe*)pInforme;
	int respuesta = 0;
	int auxIdCliente;
	int auxEstado;
	int auxCantidadDeAfiches;
	if(this != NULL && pInforme != NULL)
	{
		if(!venta_getIdCliente(pVenta,&auxIdCliente) &&
		   auxIdCliente == pInfo->idCliente &&
		   !venta_getEstado(pVenta,&auxEstado) &&
		   auxEstado == pInfo->estadoDeVenta && !venta_getCantidadDeAfiches(pVenta,&auxCantidadDeAfiches))
		{
			respuesta = auxCantidadDeAfiches;
		}
	}
	return respuesta;
}

/** \brief imprime la venta con mayor cantidad de afiches vendidos, junto con el id y cuit del cliente al que le pertenece.
 *
 * \param LinkedList* pArrayListVentas.
 * \param LinkedList* pArrayListClientes.
 * \return 0 si todoOk, -1 si ERROR.
 */
int informe_imprimirVentaConMasAfichesVendidos(LinkedList* pArrayListVentas,LinkedList* pArrayListClientes)
{
	int retorno = -1;
	Venta* pVenta;
	Cliente* pCliente;
	int auxCantidadDeAfiches;
	int mayorCantidad;
	int auxEstado;
	int auxIdCliente;
	int indiceMayor;
	int i;
	int indiceCliente;
	char auxCuit[CUIT_LEN];
	if(pArrayListVentas != NULL && pArrayListClientes != NULL)
	{
		for(i=0;i<ll_len(pArrayListVentas);i++)
		{
			pVenta = ll_get(pArrayListVentas,i);
			if(!venta_getCantidadDeAfiches(pVenta,&auxCantidadDeAfiches) && !venta_getEstado(pVenta,&auxEstado) && auxEstado == COBRADO)
			{
				if(i==0 || mayorCantidad < auxCantidadDeAfiches)
				{
					mayorCantidad = auxCantidadDeAfiches;
					indiceMayor = i;
				}
			}
		}
		pVenta = ll_get(pArrayListVentas,indiceMayor);
		if(!venta_getIdCliente(pVenta,&auxIdCliente))
		{
			indiceCliente = cliente_buscarPorId(pArrayListClientes,auxIdCliente);
			if(indiceCliente != -1)
			{
				pCliente = ll_get(pArrayListClientes,indiceCliente);
				if(!cliente_getCuit(pCliente,auxCuit))
				{
					printf("\nLa Venta con mas afiches vendidos,le pertenece al cliente de Id:%d y CUIT:%s\n"
							"con la cantida de %d de afiches vendidos.\n",auxIdCliente,auxCuit,mayorCantidad);
					retorno = 0;
				}
			}
		}
	}
	return retorno;
}
