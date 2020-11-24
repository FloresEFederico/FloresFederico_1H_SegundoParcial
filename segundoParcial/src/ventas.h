
#ifndef VENTAS_H_
#define VENTAS_H_
#include "LinkedList.h"
#include "clientes.h"
#define ARCHIVO_LEN 30
#define A_COBRAR 0
#define COBRADO 1
#define MIN_AFICHES 1
#define MAX_AFICHES 100
#define CABA 1
#define ZONA_SUR 2
#define ZONA_OESTE 3
typedef struct
{
	int id;
	char nombreArchivo[ARCHIVO_LEN];
	int cantidadDeAfiches;
	int zonaDePago;
	int estado;
	int idCliente;
}Venta;

Venta* venta_new();
Venta* venta_newParametros(int id,char* nombreArchivoStr,int cantidadDeAfiches,int zonaDePago,int estado,int idCliente);
void venta_delete(Venta* this);

int venta_setId(Venta* this,int id);
int venta_getId(Venta* this,int* id);
int venta_setNombreArchivo(Venta* this,char* nombreArchivo);
int venta_getNombreArchivo(Venta* this,char* nombreArchivo);
int venta_setCantidadDeAfiches(Venta* this,int cantidadDeAfiches);
int venta_getCantidadDeAfiches(Venta* this,int* cantidadDeAfiches);
int venta_setZonaDePago(Venta* this,int zonaDePago);
int venta_getZonaDePago(Venta* this,int* zonaDePago);
int venta_setEstado(Venta* this,int estado);
int venta_getEstado(Venta* this,int* estado);
int venta_setIdCliente(Venta* this,int idCliente);
int venta_getIdCliente(Venta* this,int* idCliente);


int venta_setIdTxt(Venta* this,char* id);
int venta_getIdTxt(Venta* this,char* id);
int venta_setCantidadDeAfichesTxt(Venta* this,char* cantidadDeAfiches);
int venta_getCantidadDeAfichesTxt(Venta* this,char* cantidadDeAfiches);
int venta_setZonaDePagoTxt(Venta* this,char* zonaDePago);
int venta_getZonaDePagoTxt(Venta* this,char* zonaDePago);
int venta_setEstadoTxt(Venta* this,char* estado);
int venta_getEstadoTxt(Venta* this,char* estado);
int venta_setIdClienteTxt(Venta* this,char* idCliente);
int venta_getIdClienteTxt(Venta* this,char* idCliente);

int venta_altaVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente);
int venta_modificarVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente);
int venta_cobrarVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente);
int venta_imprimirVenta(void* this);
int venta_imprimirVentasPorEstado(LinkedList* pArrayListVentas,int estado);
int venta_buscarPorId(LinkedList* pArrayListVentas,int idABuscar);
int venta_buscarPorIdSegunEstado(LinkedList* pArrayListVentas,int idABuscar,int estado);
int venta_obtenerNombreDeZona(int zonaDePago,char* zonaStr);
int venta_obtenerNombreDeEstado(int estado,char* estadoStr);
int venta_obtenerNumeroDeZona(int* zonaDePago,char* zonaStr);
int venta_obtenerNumeroDeEstado(int* estado,char* estadoStr);
#endif /* VENTAS_H_ */
