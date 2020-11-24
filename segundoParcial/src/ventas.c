#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utn.h"
#include "LinkedList.h"
#include "ventas.h"
#include <ctype.h>


static int isValidId(int id);
static int esNumerica(char* cadena,int limite);
static int isValidNombreDeArchivo(char* pResultado,int limite);
static int venta_generadorDeId(LinkedList* pArrayListVenta);
static int toArchivo(char* nombreDeArchivo);


/**
 * \brief genera espacio para una venta
 * \return espacio en memoria si todoOK, de lo contrario devuelve NULL.
 */
Venta* venta_new()
{
	return (Venta*)malloc(sizeof(Venta));
}

/**
 * \brief agrega a una venta los valores en modo texto pasados por parametros.
 * \param int id: id
 * \param char* nombreDeArchivoStr: nombreDeArchivo
 * \param char* cantidadDeAfichesStr: cantidad de afiches en modo texto
 * \param int zonaDePago: zona de pago.
 * \param int estado: estado.
 * \param int idCliente: idCliente.
 * \return el puntero a Venta si salio el seteo de datos estuvo bien, sino devuelve un NULL
 */
Venta* venta_newParametros(int id,char* nombreArchivoStr,int cantidadDeAfiches,int zonaDePago,int estado,int idCliente)
{
	Venta* this = venta_new();

	if(this != NULL && id >= 0 && nombreArchivoStr != NULL)
	{
		if(!venta_setId(this,id) &&
			!venta_setNombreArchivo(this,nombreArchivoStr) &&
			!venta_setZonaDePago(this,zonaDePago) &&
			!venta_setCantidadDeAfiches(this,cantidadDeAfiches) &&
			!venta_setEstado(this,estado) &&
			!venta_setIdCliente(this,idCliente))
		{
			return this;
		}else
		{
			venta_delete(this);
			this = NULL;
		}
	}
	return NULL;
}

/**
 * \brief libera la memoria de un puntero a Venta
 * \param Venta* this: puntero de Venta
 */
void venta_delete(Venta* this)
{
	if(this != NULL)
	{
		free(this);
	}
}

/**
 * \brief carga a un venta el id pasado por parametro validando si el id es correcto.
 * \param Venta* this: puntero de Venta
 * \param int id: Id entero
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setId(Venta* this,int id)
{
	int retorno = -1;
	static int maximoId = -1;
	if (this != NULL && isValidId(id))
	{
		retorno = 0;
		if (id < 0)
		{
			maximoId++;
			this->id = maximoId;
		}
		else
		{
			if (id > maximoId)
			{
				maximoId = id;
				this->id = id;
			}
		}
	}
	return retorno;
}

/**
 * \brief imprime los datos de una Venta
 * \param Void* this: puntero a void
 * \return (-1) ERROR/ (0) Ok
 */
int venta_imprimirVenta(void* this)
{
	int retorno = -1;
	Venta* pElement = (Venta*)this;
	int auxId;
	char auxNombreArchivo[ARCHIVO_LEN];
	int auxCantidad;
	int auxZona;
	char nombreDeZona[30];
	int auxEstado;
	char nombreDeEstado[30];
	int auxIdCliente;
	if(!venta_getId(pElement,&auxId) &&
	   !venta_getNombreArchivo(pElement,auxNombreArchivo) &&
	   !venta_getCantidadDeAfiches(pElement,&auxCantidad) &&
	   !venta_getZonaDePago(pElement,&auxZona) &&
	   !venta_getEstado(pElement,&auxEstado) &&
	   !venta_getIdCliente(pElement,&auxIdCliente) &&
	   !venta_obtenerNombreDeZona(auxZona,nombreDeZona) &&
	   !venta_obtenerNombreDeEstado(auxEstado,nombreDeEstado))
	{
		printf("Id: %04d  Nombre de Archivo: %s\n"
				"Cantidad de Afiches: %d Zona de Pago: %s  Id Cliente: %d\n"
				"Estado: %s\n\n",auxId,auxNombreArchivo,auxCantidad,nombreDeZona,auxIdCliente,nombreDeEstado);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief imprime los datos de una Venta segun su estado
 * \param LinkedList* pArrayListVentas: lista de ventas
 * \param int estado: estado de la venta.
 * \return (-1) ERROR/ (0) Ok
 */
int venta_imprimirVentasPorEstado(LinkedList* pArrayListVentas,int estado)
{
	int retorno = -1;
	Venta* pElement;
	int auxEstado;
	int i;
	if(pArrayListVentas != NULL && (estado == A_COBRAR || estado == COBRADO))
	{
		for(i=0;i<ll_len(pArrayListVentas);i++)
		{
			pElement = ll_get(pArrayListVentas,i);
			if(!venta_getEstado(pElement,&auxEstado) && auxEstado == estado)
			{
				venta_imprimirVenta(pElement);
				retorno = 0;
			}
		}
	}
	return retorno;
}

/**
 * \brief Realiza el alta de una venta
 * \param LinkedList* pArrayListVentas: lista de ventas
 * \param LinkedList* pArrayListCliente: lista de Clientes.
 * \return (-1) ERROR/ (0) Ok
 */
int venta_altaVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente)
{
	int retorno = -1;
	int auxIdCliente;
	int existeIdCliente;
	int auxCantidadDeAfiches;
	char auxNombreArchivo[ARCHIVO_LEN];
	int auxZona;
	int auxId;
	int auxEstado;
	Venta* pElement;
	if(pArrayListVenta != NULL &&
	   !ll_map(pArrayListCliente,cliente_imprimirCliente) &&
	   !utn_getNumero(&auxIdCliente,"Ingrese el Id Cliente: ","\nId invalido!\n",100,9999,3))
	{
		existeIdCliente = cliente_buscarPorId(pArrayListCliente,auxIdCliente);
		if(existeIdCliente != -1)
		{
			if(!utn_getNumero(&auxCantidadDeAfiches,"\nIngrese cantidad de afiches(1-100): ","\nCantidad invalida!\n",MIN_AFICHES,MAX_AFICHES,3) &&
			   !utn_getDireccion(auxNombreArchivo,ARCHIVO_LEN,"\nIngrese el nombre del archivo: ","\nNombre invalido!\n",3) &&
			   !utn_getNumero(&auxZona,"\n-Zonas de Pago-\n"
					   	   	   	   	   "1-CABA.\n"
					   	   	   	   	   "2-ZONA SUR.\n"
					   	   	   	   	   "3-ZONA OESTE.\nElija opcion(1-3): ","\nOpcion invalida!\n",1,3,3))
			{
				auxId = venta_generadorDeId(pArrayListVenta);
				auxEstado = A_COBRAR;
				pElement = venta_newParametros(auxId,auxNombreArchivo,auxCantidadDeAfiches,auxZona,auxEstado,auxIdCliente);
				if(pElement != NULL)
				{
					ll_add(pArrayListVenta,pElement);
					printf("\nSe han ingresado todos los datos correctamente!\n"
							"Id de la Venta: %d\n",auxId);
					retorno = 0;
				}else
				{
					printf("\nNo se pudo pasar los datos obtenidos para la venta!\n");
				}
			}
		}else
		{
			printf("\nEl id cliente ingresado no existe\n");
		}
	}
	return retorno;
}

/**
 * \brief modifica un campo de la venta segun elija el usuario: puede modificar el nombre de archivo, cantidad de afiches, zona de pago.
 * \param LinkedList* pArrayListVentas: lista de ventas
 * \param LinkedList* pArrayListCliente: lista de Clientes.
 * \return (-1) ERROR/ (0) Ok
 */
int venta_modificarVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente)
{
	int retorno = -1;
	Venta* pElement;
	int idABuscar;
	int indiceAModificar;
	int idClienteABuscar;
	int indiceCliente;
	int opcionModificar;
	char auxNombreArchivo[ARCHIVO_LEN];
	int auxCantidad;
	int auxZona;
	int flagIn = 0;
	if(pArrayListVenta != NULL && pArrayListCliente && !venta_imprimirVentasPorEstado(pArrayListVenta,A_COBRAR) &&
	   !utn_getNumero(&idABuscar,"Ingrese el Id de la venta: ","\nId invalido!\n",1000,2000,3))
	{
		indiceAModificar = venta_buscarPorId(pArrayListVenta,idABuscar);
		if(indiceAModificar != -1 )
		{
			pElement = ll_get(pArrayListVenta,indiceAModificar);
			if(!venta_getIdCliente(pElement,&idClienteABuscar))
			{
				indiceCliente = cliente_buscarPorId(pArrayListCliente,idClienteABuscar);
				cliente_imprimirCliente(ll_get(pArrayListCliente,indiceCliente));
				if(!utn_getNumero(&opcionModificar,"¿Que campo desea modificar?\n"
												   "1-Nombre de Archivo.\n"
												   "2-Cantidad de Afiches.\n"
												   "3-Zona de pago.\n"
												   "4-Cancelar.\nElija opcion(1-4): ","\nOpcion invalida!\n",1,4,3))
				{
					switch(opcionModificar)
					{
					case 1:
						if(!utn_getDireccion(auxNombreArchivo,ARCHIVO_LEN,"\nIngrese el nuevo nombre de archivo: ","\nArchivo invalido!\n",3) &&
						   !venta_setNombreArchivo(pElement,auxNombreArchivo))
						{
							printf("\nSe modifico el nombre del archivo!\n");
							flagIn = 1;
						}else
						{
							printf("\nSe acabo el numero de intentos para ingresar un nombre valido!\nVolviendo al menu principal.\n");
						}
						break;
					case 2:
						if(!utn_getNumero(&auxCantidad,"\nIngrese el nuevo Nro de cantidad de afiches(1-100): ","\nNumero invalido!\n",MIN_AFICHES,MAX_AFICHES,3) &&
						   !venta_setCantidadDeAfiches(pElement,auxCantidad))
						{
							printf("\nSe modifico la cantidad de afiches!\n");
							flagIn = 1;
						}else
						{
							printf("\nSe acabo el numero de reintentos para ingresar una cantidad valida!\nVolviendo al menu principal.\n");
						}
						break;
					case 3:
						if(!utn_getNumero(&auxZona,"-zonas de pago-\n"
												   "1-CABA.\n"
												   "2-ZONA SUR.\n"
												   "3-ZONA OESTE.\nElija Opcion: ","\nOpcion invalida!\n",1,3,3) &&
						   !venta_setZonaDePago(pElement,auxZona))
						{
							printf("\nSe modifico la Zona de Pago!\n");
							flagIn = 1;
						}else
						{
							printf("\nSe acabo el numero de reintentos para elegir una zona valida!\nVolviendo al menu principal.\n");
						}
						break;
					default:
						printf("\nSe cancelo la modificacion.\n");
						flagIn = 1;
					}
					if(flagIn)
					{
						retorno = 0;
					}
				}

			}
		}
	}
	return retorno;
}

/**
 * \brief cambia el estado de una venta en estado "A COBRAR" a estado "COBRADA"
 * \param LinkedList* pArrayListVentas: lista de ventas
 * \param LinkedList* pArrayListCliente: lista de Clientes.
 * \return (-1) ERROR/ (0) Ok
 */
int venta_cobrarVenta(LinkedList* pArrayListVenta,LinkedList* pArrayListCliente)
{
	int retorno = -1;
	Venta* pElement;
		int idABuscar;
		int indiceACobrar;
		int idClienteABuscar;
		int indiceCliente;
		int opcion;
		if(pArrayListVenta != NULL && pArrayListCliente && !venta_imprimirVentasPorEstado(pArrayListVenta,A_COBRAR) &&
		   !utn_getNumero(&idABuscar,"Ingrese el Id de la venta: ","\nId invalido!\n",1000,2000,3))
		{
			indiceACobrar = venta_buscarPorIdSegunEstado(pArrayListVenta,idABuscar,A_COBRAR);
			if(indiceACobrar != -1 )
			{
				pElement = ll_get(pArrayListVenta,indiceACobrar);
				if(!venta_getIdCliente(pElement,&idClienteABuscar))
				{
					indiceCliente = cliente_buscarPorId(pArrayListCliente,idClienteABuscar);
					cliente_imprimirCliente(ll_get(pArrayListCliente,indiceCliente));
					if(!utn_getNumero(&opcion,"¿Desea confirmar el cambio de estado?\n"
											  "1-Si.\n"
											  "2-No.\nElija opcion(1-2): ","\nOpcion invalida!\n",1,2,3))
					{
						if(opcion == 1 && !venta_setEstado(pElement,COBRADO))
						{
							printf("\nSe ha cambiado el estado!\n");
						}else
						{
							printf("\nSe ha cancelado la operacion.\n");
						}
						retorno = 0;
					}
				}
			}else
			{
				printf("\nNo se encontro a ninguna Venta con ese Id\n");
			}
		}
	return retorno;
}

/**
 * \brief busca si el id pasado por parametro pertenece a una Venta de la lista. En caso de encontrarlo, retorna el indice de esa Venta.
 * \param LinkedList* pArrayListVentas: puntero de LinkedList
 * \param int idABuscar: id a ser buscado
 * \return (-1) ERROR/ (0) Ok
 */
int venta_buscarPorId(LinkedList* pArrayListVentas,int idABuscar)
{
	int indiceDelId = -1;
	Venta* pElement;
	int i;
	int auxId;
	if(pArrayListVentas != NULL)
	{
		for(i=0;i<ll_len(pArrayListVentas);i++)
		{
			pElement = ll_get(pArrayListVentas,i);
			if(!venta_getId(pElement,&auxId) && auxId == idABuscar)
			{
				indiceDelId = i;
				break;
			}
		}
	}
	return indiceDelId;
}

/**
 * \brief busca si el id pasado por parametro pertenece a una Venta de la lista segun el estado pasado por parametro. En caso de encontrarlo, retorna el indice de esa Venta.
 * \param LinkedList* pArrayListVentas: puntero de LinkedList
 * \param int idABuscar: id a ser buscado
 * \param int estado: estado de la venta
 * \return (-1) ERROR/ (0) Ok
 */
int venta_buscarPorIdSegunEstado(LinkedList* pArrayListVentas,int idABuscar,int estado)
{
	int indiceDelId = -1;
	Venta* pElement;
	int i;
	int auxId;
	int auxEstado;
	if(pArrayListVentas != NULL)
	{
		for(i=0;i<ll_len(pArrayListVentas);i++)
		{
			pElement = ll_get(pArrayListVentas,i);
			if(!venta_getId(pElement,&auxId) && !venta_getEstado(pElement,&auxEstado) && auxId == idABuscar && auxEstado == estado)
			{
				indiceDelId = i;
				break;
			}
		}
	}
	return indiceDelId;
}

/**
 * \brief obtiene el Id de una venta.
 * \param Venta* this: puntero de Venta
 * \param int* id: puntero Id
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getId(Venta* this,int* id)
{
	int retorno = -1;
	if(this != NULL && id != NULL)
	{
		*id = this->id;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea un nombreDeArchivo al campo de nombreDeArchivo de una venta validando si el valor pasado por parametro es correcto.
 * \param Venta* this: puntero de Venta
 * \param char* nombreDeArchivo: nombreDeArchivo tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setNombreArchivo(Venta* this,char* nombreArchivo)
{
	int retorno = -1;
	if(this != NULL && nombreArchivo != NULL)
	{
		if(isValidNombreDeArchivo(nombreArchivo,ARCHIVO_LEN) && toArchivo(nombreArchivo))
		{
			strncpy(this->nombreArchivo,nombreArchivo,ARCHIVO_LEN);
			retorno = 0;
		}
	}
	return retorno;
}
/**
 * \brief obtiene el nombreDeArchivo de una venta.
 * \param Venta* this: puntero de Venta
 * \param char* nombreDeArchivo: nombreDeArchivo tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getNombreArchivo(Venta* this,char* nombreArchivo)
{
	int retorno = -1;
	if(this != NULL && nombreArchivo != NULL)
	{
		strncpy(nombreArchivo,this->nombreArchivo,ARCHIVO_LEN);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo cantidadDeArchivos de una Venta pasado por parametro, el valor de cantidadDeArchivos tambien pasado por parametro
 * \param Venta* this: puntero de Venta
 * \param int cantidadDeArchivos: cantidadDeArchivos del tipo entero
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setCantidadDeAfiches(Venta* this,int cantidadDeAfiches)
{
	int retorno = -1;
	char bufferCantidad[400];
	sprintf(bufferCantidad,"%d",cantidadDeAfiches);
	if(this != NULL && esNumerica(bufferCantidad,sizeof(bufferCantidad)))
	{
		this->cantidadDeAfiches = cantidadDeAfiches;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el CantidadDeArchivos de una Venta.
 * \param Venta* this: puntero de Venta
 * \param int* cantidadDeArchivos: puntero CantidadDeArchivos
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getCantidadDeAfiches(Venta* this,int* cantidadDeAfiches)
{
	int retorno = -1;
	if(this != NULL && cantidadDeAfiches != NULL)
	{
		*cantidadDeAfiches = this->cantidadDeAfiches;
		retorno = 0;
	}
	return retorno;
}
/**
 * \brief setea en el campo zonaDePago de una Venta pasado por parametro, el valor de zonaDePago tambien pasado por parametro
 * \param Venta* this: puntero de Venta
 * \param int zonaDePago: zonaDePago del tipo entero
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setZonaDePago(Venta* this,int zonaDePago)
{
	int retorno = -1;
	char bufferZona[400];
	sprintf(bufferZona,"%d",zonaDePago);
	if(this != NULL && esNumerica(bufferZona,sizeof(bufferZona)))
	{
		this->zonaDePago = zonaDePago;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el ZonaDePago de una Venta.
 * \param Venta* this: puntero de Venta
 * \param int* zonaDePago: puntero ZonaDePago
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getZonaDePago(Venta* this,int* zonaDePago)
{
	int retorno = -1;
	if(this != NULL && zonaDePago != NULL)
	{
		*zonaDePago = this->zonaDePago;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo estado de una Venta pasado por parametro, el valor de estado tambien pasado por parametro
 * \param Venta* this: puntero de Venta
 * \param int estado: estado del tipo entero
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setEstado(Venta* this,int estado)
{
	int retorno = -1;
	char bufferEstado[400];
	sprintf(bufferEstado,"%d",estado);
	if(this != NULL && esNumerica(bufferEstado,sizeof(bufferEstado)))
	{
		this->estado = estado;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el Estado de una Venta.
 * \param Venta* this: puntero de Venta
 * \param int* estado: puntero Estado
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getEstado(Venta* this,int* estado)
{
	int retorno = -1;
	if(this != NULL && estado != NULL)
	{
		*estado = this->estado;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo idCliente de una Venta pasado por parametro, el valor de idCliente tambien pasado por parametro
 * \param Venta* this: puntero de Venta
 * \param int idCliente: idCliente del tipo entero
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setIdCliente(Venta* this,int idCliente)
{
	int retorno = -1;
	char bufferIdCliente[400];
	sprintf(bufferIdCliente,"%d",idCliente);
	if(this != NULL && esNumerica(bufferIdCliente,sizeof(bufferIdCliente)))
	{
		this->idCliente = idCliente;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el IdCliente de una Venta.
 * \param Venta* this: puntero de Venta
 * \param int* idCliente: puntero IdCliente
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getIdCliente(Venta* this,int* idCliente)
{
	int retorno = -1;
	if(this != NULL && idCliente != NULL)
	{
		*idCliente = this->idCliente;
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo idCliente de una Venta un id en modo texto, transformandolo a entero antes de setearlo.
 * \param Venta* this: puntero de Venta
 * \param char* idCliente: idCliente del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setIdTxt(Venta* this,char* id)
{
	int retorno = -1;
	if(this != NULL && id != NULL)
	{
		if(esNumerica(id,1000))
		{
			this->id = atoi(id);
			retorno = 0;
		}
	}
	return retorno;
}

/**
 * \brief obtiene el IdCliente de una Venta en modo texto.
 * \param Venta* this: puntero de Venta
 * \param char* idCliente: idCliente del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getIdTxt(Venta* this,char* id)
{
	int retorno = -1;
	if(this != NULL && id != NULL)
	{
		sprintf(id,"%d",this->id);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo cantidadDeAfiches de una Venta un cantidadDeAfiches en modo texto, transformandolo a entero antes de setearlo.
 * \param Venta* this: puntero de Venta
 * \param char* cantidadDeAfiches: cantidadDeAfiches del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setCantidadDeAfichesTxt(Venta* this,char* cantidadDeAfiches)
{
	int retorno = -1;
	if(this != NULL && cantidadDeAfiches != NULL)
	{
		if(esNumerica(cantidadDeAfiches,1000))
		{
			this->cantidadDeAfiches = atoi(cantidadDeAfiches);
			retorno = 0;
		}
	}
	return retorno;
}

/**
 * \brief obtiene la cantidadDeAfiches de una Venta en modo texto.
 * \param Venta* this: puntero de Venta
 * \param char* cantidadDeAfiches: cantidadDeAfiches del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getCantidadDeAfichesTxt(Venta* this,char* cantidadDeAfiches)
{
	int retorno = -1;
	if(this != NULL && cantidadDeAfiches != NULL)
	{
		sprintf(cantidadDeAfiches,"%d",this->cantidadDeAfiches);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo zonaDePago de una Venta un zonaDePago en modo texto, transformandolo a entero antes de setearlo.
 * \param Venta* this: puntero de Venta
 * \param char* zonaDePago: zonaDePago del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setZonaDePagoTxt(Venta* this,char* zonaDePago)
{
	int retorno = -1;
	if(this != NULL && zonaDePago != NULL)
	{
		if(esNumerica(zonaDePago,1000))
		{
			this->zonaDePago = atoi(zonaDePago);
			retorno = 0;
		}
	}
	return retorno;
}

/**
 * \brief obtiene la zonaDePago de una Venta en modo texto.
 * \param Venta* this: puntero de Venta
 * \param char* zonaDePago: zonaDePago del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getZonaDePagoTxt(Venta* this,char* zonaDePago)
{
	int retorno = -1;
	if(this != NULL && zonaDePago != NULL)
	{
		sprintf(zonaDePago,"%d",this->zonaDePago);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo estado de una Venta un estado en modo texto, transformandolo a entero antes de setearlo.
 * \param Venta* this: puntero de Venta
 * \param char* estado: estado del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setEstadoTxt(Venta* this,char* estado)
{
	int retorno = -1;
	if(this != NULL && estado != NULL)
	{
		if(esNumerica(estado,1000))
		{
			this->estado = atoi(estado);
			retorno = 0;
		}
	}
	return retorno;
}

/**
 * \brief obtiene la estado de una Venta en modo texto.
 * \param Venta* this: puntero de Venta
 * \param char* estado: estado del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getEstadoTxt(Venta* this,char* estado)
{
	int retorno = -1;
	if(this != NULL && estado != NULL)
	{
		sprintf(estado,"%d",this->estado);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea en el campo idCliente de una Venta un idCliente en modo texto, transformandolo a entero antes de setearlo.
 * \param Venta* this: puntero de Venta
 * \param char* idCliente: idCliente del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_setIdClienteTxt(Venta* this,char* idCliente)
{
	int retorno = -1;
	if(this != NULL && idCliente != NULL)
	{
		if(esNumerica(idCliente,1000))
		{
			this->idCliente = atoi(idCliente);
			retorno = 0;
		}
	}
	return retorno;
}

/**
 * \brief obtiene el idCliente de una Venta en modo texto.
 * \param Venta* this: puntero de Venta
 * \param char* idCliente: idCliente del tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int venta_getIdClienteTxt(Venta* this,char* idCliente)
{
	int retorno = -1;
	if(this != NULL && idCliente != NULL)
	{
		sprintf(idCliente,"%d",this->idCliente);
		retorno = 0;
	}
	return retorno;
}

/*
 * isValidId: Recibe un id y valida si el id es valido
 * int id: id a ser validado
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int isValidId(int id)
{
	int respuesta = 0;
	char bufferId[400];
	sprintf(bufferId,"%d",id);
	if(id >= 0 && esNumerica(bufferId,sizeof(bufferId)))
	{
		respuesta = 1;
	}
	return respuesta;
}

/**
* esNumerica: Verifica si la cadena ingresada es numerica
* cadena: cadena de caracteres a ser analizada
* limite: limite de la cadena
* Retorno: 1 (verdadero) si la cadena es numerica , 0 (falso) si no y -1 en caso de ERROR de parametro
*
*/
static int esNumerica(char* cadena,int limite)
{
	int retorno = 1;
	int i = 0;
	if(cadena != NULL && limite > 0)
	{
		retorno = 1;
		if(cadena[0] == '+')
		{
			i = 1;
		}
		for(;cadena[i] != '\0';i++)
		{
			if(cadena[i] < '0' || cadena[i] > '9' )
			{
				retorno = 0;
				break;
			}
		}
	}
	return retorno;
}

/*
 * isValidNombreDeArchivo: Recibe una cadena de caracteres y su limite, y pregunta si el nombre de archivo es valido
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int isValidNombreDeArchivo(char* pResultado,int limite)
{
	int respuesta = 1;
	int i;
	if(pResultado != NULL && limite > 0){
		for(i=0; i <= limite && pResultado[i] != '\0';i++){
			if((pResultado[i] < 'a' || pResultado[i] > 'z') &&
			   (pResultado[i] < 'A' || pResultado[i] > 'Z') &&
			   (pResultado[i] < '0' || pResultado[i] > '9') &&
			   pResultado[i] != '_' &&
			   pResultado[i] != '-' &&
			   pResultado[i] != '.')
			{
				respuesta = 0;
				break;
			}
		}
	}
	return respuesta;
}

/*
 * toArchivo: recibe un nombre de archivo y le concatena un tipo de archivo en caso de no tenerlo ya concatenado.
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int toArchivo(char* nombreDeArchivo)
{
	int respuesta = 0;
	int i;
	int flagIn = 0;
	if(nombreDeArchivo != NULL)
	{
		for(i=0;nombreDeArchivo[i] != '\0';i++)
		{
			if(nombreDeArchivo[i] == '.')
			{
				flagIn = 1;
				break;
			}
		}
		if(!flagIn)
		{
			strcat(nombreDeArchivo,".rar");
		}
		respuesta = 1;
	}
	return respuesta;
}

/**
 * \brief genera un id estatico
 * \param linkedList* pArrayListVenta: lista dinamica de Venta
 * \return id;
 */
static int venta_generadorDeId(LinkedList* pArrayListVenta)
{
	Venta* this;
	static int flagIn = 0;
	static int auxId;
	int idDeLaUltimaVenta;
	int len = ll_len(pArrayListVenta);
	if(pArrayListVenta != NULL)
	{
		if(!flagIn)
		{
			if(len == 0)
			{
				auxId = 1000;
			}else
			{
				this = ll_get(pArrayListVenta,len-1);
				if(!venta_getId(this,&idDeLaUltimaVenta))
				{
					auxId = idDeLaUltimaVenta;
					auxId++;
				}
			}
			flagIn = 1;
		}else
		{
			auxId++;
		}
	}
	return auxId;
}

/**
 * \brief obtiene el nombre de una zona segun el tipo de zona que se le haya pasado por parametro
 * \param int zonaDePago: zonaDePago tipo entero
 * \param char* zonaStr: nombre de la zona.
 * \return devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.;
 */
int venta_obtenerNombreDeZona(int zonaDePago,char* zonaStr)
{
	int retorno = -1;
	if(zonaStr != NULL)
	{
		switch(zonaDePago)
		{
		case 1:
			sprintf(zonaStr,"CABA");
			break;
		case 2:
			sprintf(zonaStr,"ZONA SUR");
			break;
		default:
			sprintf(zonaStr,"ZONA OESTE");
		}
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el nombre de un estado segun el tipo de estado que se le haya pasado por parametro
 * \param int estado: estado tipo entero
 * \param char* estadoStr: nombre del estado de venta.
 * \return devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.;
 */
int venta_obtenerNombreDeEstado(int estado,char* estadoStr)
{
	int retorno = -1;
	if(estadoStr != NULL)
	{
		if(!estado)
		{
			sprintf(estadoStr,"A COBRAR");
		}else
		{
			sprintf(estadoStr,"COBRADA");
		}
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el numero de una zona segun el nombre de zona que se le haya pasado por parametro
 * \param int* zonaDePago: zonaDePago tipo puntero a entero
 * \param char* zonaStr: nombre de la zona.
 * \return devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.;
 */
int venta_obtenerNumeroDeZona(int* zonaDePago,char* zonaStr)
{
	int retorno = -1;
	if(zonaStr != NULL)
	{
		if(strcmp(zonaStr,"CABA") == 0)
		{
			*zonaDePago = CABA;
		}else if(strcmp(zonaStr,"ZONA SUR") == 0)
		{
			*zonaDePago = ZONA_SUR;
		}else
		{
			*zonaDePago = ZONA_OESTE;
		}
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief obtiene el numero de un estado segun el nombre de estado que se le haya pasado por parametro
 * \param int* estado: estado tipo puntero a entero
 * \param char* estadoStr: nombre de la estado.
 * \return devuelve un 0 si esta todoOK. Devuelve -1 si hubo un error.;
 */
int venta_obtenerNumeroDeEstado(int* estado,char* estadoStr)
{
	int retorno = -1;
	if(estadoStr != NULL)
	{
		if(!strcmp(estadoStr,"A COBRAR"))
		{
			*estado = A_COBRAR;
		}else
		{
			*estado = COBRADO;
		}
		retorno = 0;
	}
	return retorno;
}
