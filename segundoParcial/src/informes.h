
#ifndef INFORMES_H_
#define INFORMES_H_
#include "LinkedList.h"
#include "clientes.h"
#include "ventas.h"

typedef struct
{
	int idCliente;
	int estadoDeVenta;
}Informe;

int informe_cantidadDeVentasPorClienteSegunEstadoDeVenta(void* this,void* pInforme);
int informe_cantidadDeAfichesPorClienteSegunEstadoDeVenta(void* this,void* pInforme);
int informe_imprimirClienteDeMayoryMenorCantidadAfichesVendidos(LinkedList* pArrayListVentas,LinkedList* pArrayListClientes);
int informe_imprimirVentaConMasAfichesVendidos(LinkedList* pArrayListVentas,LinkedList* pArrayListClientes);

#endif /* INFORMES_H_ */
