#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utn.h"
#include "LinkedList.h"
#include "clientes.h"
#include <ctype.h>

static int isValidId(int id);
static int esNumerica(char* cadena,int limite);
static int isValidCuit(char* pResultado,int limite);
static int isValidNombre(char* pResultado,int limite);
static int toNombre(char text[],int len);
static int cliente_generadorDeId(LinkedList* pArrayListCliente);




/**
 * \brief genera espacio para un cliente
 * \return espacio en memoria si todoOK, de lo contrario devuelve NULL.
 */
Cliente* cliente_new()
{
	return (Cliente*)malloc(sizeof(Cliente));
}

/**
 * \brief agrega a un cliente los valores en modo texto pasados por parametros.
 * \param char* id: id
 * \param char* nombreStr: nombre
 * \param char* apellidoStr: apellido
 * \param char* cuitStr: cuit.
 * \return el puntero a Cliente si salio el seteo de datos estuvo bien, sino devuelve un NULL
 */
Cliente* cliente_newParametros(int id,char* nombreStr,char* apellidoStr,char* cuitStr)
{
	Cliente* this = cliente_new();
	if(this != NULL && id >= 0 && nombreStr != NULL && apellidoStr != NULL && cuitStr != NULL)
	{
		if(!cliente_setId(this,id) &&
		   !cliente_setNombre(this,nombreStr) &&
		   !cliente_setApellido(this,apellidoStr) &&
		   !cliente_setCuit(this,cuitStr))
		{
			return this;
		}else
		{
			cliente_delete(this);
			this = NULL;
		}
	}
	return NULL;
}
/**
 * \brief libera la memoria de un puntero a Cliente
 * \param Cliente* this: puntero de Cliente
 */
void cliente_delete(Cliente* this)
{
	if(this != NULL)
	{
		free(this);
	}
}

/**
 * \brief carga a un Cliente el id pasado por parametro validando si el id es correcto.
 * \param Cliente* this: puntero de Cliente
 * \param int id: Id entero
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_setId(Cliente* this,int id)
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
 * \brief obtiene el Id de un Cliente.
 * \param Cliente* this: puntero de Cliente
 * \param int* id: puntero Id
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_getId(Cliente* this,int* id)
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
 * \brief setea un nombre al campo de nombre de un Cliente validando si el valor pasado por parametro es correcto.
 * \param Cliente* this: puntero de Cliente
 * \param char* nombre: nombre tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_setNombre(Cliente* this,char* nombre)
{
	int retorno = -1;
	if(this != NULL && nombre != NULL)
	{
		if(isValidNombre(nombre,NOMBRE_LEN) && toNombre(nombre,NOMBRE_LEN))
		{
			strncpy(this->nombre,nombre,NOMBRE_LEN);
			retorno = 0;
		}
	}
	return retorno;
}
/**
 * \brief obtiene el nombre de un Cliente.
 * \param Cliente* this: puntero de Cliente
 * \param char* nombre: nombre tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_getNombre(Cliente* this,char* nombre)
{
	int retorno = -1;
	if(this != NULL && nombre != NULL)
	{
		strncpy(nombre,this->nombre,NOMBRE_LEN);
		retorno = 0;
	}
	return retorno;
}


/**
 * \brief setea un apellido al campo de apellido de un Cliente validando si el valor pasado por parametro es correcto.
 * \param Cliente* this: puntero de Cliente
 * \param char* apellido: apellido tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_setApellido(Cliente* this,char* apellido)
{
	int retorno = -1;
	if(this != NULL && apellido != NULL)
	{
		if(isValidNombre(apellido,NOMBRE_LEN) && toNombre(apellido,NOMBRE_LEN))
		{
			strncpy(this->apellido,apellido,NOMBRE_LEN);
			retorno = 0;
		}
	}
	return retorno;
}
/**
 * \brief obtiene el apellido de un Cliente.
 * \param Cliente* this: puntero de Cliente
 * \param char* apellido: apellido tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_getApellido(Cliente* this,char* apellido)
{
	int retorno = -1;
	if(this != NULL && apellido != NULL)
	{
		strncpy(apellido,this->apellido,NOMBRE_LEN);
		retorno = 0;
	}
	return retorno;
}

/**
 * \brief setea un cuit al campo de cuit de un Cliente validando si el valor pasado por parametro es correcto.
 * \param Cliente* this: puntero de Cliente
 * \param char* cuit: cuit tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_setCuit(Cliente* this,char* cuit)
{
	int retorno = -1;
	if(this != NULL && cuit != NULL)
	{
		if(isValidCuit(cuit,CUIT_LEN))
		{
			strncpy(this->cuit,cuit,CUIT_LEN);
			retorno = 0;
		}
	}
	return retorno;
}
/**
 * \brief obtiene el cuit de un Cliente.
 * \param Cliente* this: puntero de Cliente
 * \param char* cuit: cuit tipo char*
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_getCuit(Cliente* this,char* cuit)
{
	int retorno = -1;
	if(this != NULL && cuit != NULL)
	{
		strncpy(cuit,this->cuit,CUIT_LEN);
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
 * isValidNombre: Recibe una cadena de caracteres y su limite, y pregunta si el nombre es valido
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int isValidNombre(char* pResultado,int limite)
{
	int respuesta = 1;
	int i;
	if(pResultado != NULL && limite > 0){
		for(i=0; i <= limite && pResultado[i] != '\0';i++){
			if((pResultado[i] < 'a' || pResultado[i] > 'z') &&
			   (pResultado[i] < 'A' || pResultado[i] > 'Z'))
			{
				respuesta = 0;
				break;
			}
		}
	}
	return respuesta;
}

/*
 * isValidCuit: Recibe una cadena de caracteres y su limite, y pregunta si el cuit es valido
 * pResultado: cadena de caracteres
 * limite: tamaño del texto
 * Retorno: devuelve un 1 si esta todoOK. Devuelve 0 si hubo un error.
 *
 */
static int isValidCuit(char* pResultado,int limite)
{
	int respuesta = 1;
	int i;
	if(pResultado != NULL && limite > 0){
		for(i=0; i <= limite && pResultado[i] != '\0';i++){
			if((pResultado[i] < '0' || pResultado[i] > '9') &&
			   (pResultado[i] != '-'))
			{
				respuesta = 0;
				break;
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

/**
 * \brief Realiza el alta de un Cliente, pidiendole valores y luego utilizando la funcion cliente_newParametros para guardarlos en una lista dinamica
 * \param LinkedList* pArrayListCliente: puntero de LinkedList
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_altaCliente(LinkedList* pArrayListCliente)
{
	int retorno = -1;
	Cliente* this;
	int auxId;
	char auxNombre[NOMBRE_LEN];
	char auxApellido[NOMBRE_LEN];
	char auxCuit[CUIT_LEN];
	int existeCuit;
	if(pArrayListCliente != NULL)
	{
		if(!utn_getNombre(auxNombre,NOMBRE_LEN,"Ingrese nombre: ","\nNombre Invalido!\n",3) &&
		   !utn_getNombre(auxApellido,NOMBRE_LEN,"\nIngrese Apellido: ","\nApellido invalido!\n",3) &&
		   !utn_getCuit(auxCuit,CUIT_LEN,"\nIngrese CUIT(XX-XXXXXXXX-X): ","\nCUIT invalido!\n",3))
		{
			existeCuit = cliente_buscarPorCuit(pArrayListCliente,auxCuit);
			if(existeCuit == -1)
			{
				auxId = cliente_generadorDeId(pArrayListCliente);
				this = cliente_newParametros(auxId,auxNombre,auxApellido,auxCuit);
				if(this != NULL)
				{
					ll_add(pArrayListCliente,this);
					this = NULL;
					printf("\nSe ha realizado el alta exitosamente!\n");
					retorno = 0;
				}else
				{
					printf("\nHubo un error en el ingreso de los datos!\n");
				}
			}else
			{
				printf("\nEl Cuit ingresado ya existe!\n");
			}
		}
	}
	return retorno;
}

/**
 * \brief busca si el id pasado por parametro pertenece a un cliente de la lista. En caso de encontrarlo, retorna el indice de ese Cliente.
 * \param LinkedList* pArrayListCliente: puntero de LinkedList
 * \param int idABuscar: id a ser buscado
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_buscarPorId(LinkedList* pArrayListCliente,int idABuscar)
{
	int indiceDelId = -1;
	Cliente* pElement;
	int i;
	int auxId;
	if(pArrayListCliente != NULL)
	{
		for(i=0;i<ll_len(pArrayListCliente);i++)
		{
			pElement = ll_get(pArrayListCliente,i);
			if(!cliente_getId(pElement,&auxId) && auxId == idABuscar)
			{
				indiceDelId = i;
				break;
			}
		}
	}
	return indiceDelId;
}

/**
 * \brief busca si el cuit pasado por parametro pertenece a un cliente de la lista. En caso de encontrarlo, retorna el indice de ese Cliente.
 * \param LinkedList* pArrayListCliente: puntero de LinkedList
 * \param char* cuitABuscar: cuit a ser buscado
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_buscarPorCuit(LinkedList* pArrayListCliente,char* cuitABuscar)
{
	int indiceDelCuit = -1;
	Cliente* pElement;
	int i;
	char auxCuit[CUIT_LEN];
	if(pArrayListCliente != NULL)
	{
		for(i=0;i<ll_len(pArrayListCliente);i++)
		{
			pElement = ll_get(pArrayListCliente,i);
			if(!cliente_getCuit(pElement,auxCuit) && !strncmp(auxCuit,cuitABuscar,CUIT_LEN))
			{
				indiceDelCuit = i;
				break;
			}
		}
	}
	return indiceDelCuit;
}



/**
 * \brief  imprime los datos de un Cliente
 * \param void* this: puntero generico
 */
int cliente_imprimirCliente(void* this)
{
	Cliente* pElement = (Cliente*)this;
	int auxId;
	char auxNombre[NOMBRE_LEN];
	char auxApellido[NOMBRE_LEN];
	char auxCuit[CUIT_LEN];
	int retorno = -1;
	if(!cliente_getId(pElement,&auxId) &&
	   !cliente_getNombre(pElement,auxNombre) &&
       !cliente_getApellido(pElement,auxApellido) &&
	   !cliente_getCuit(pElement,auxCuit))
	{
		printf("Id: %04d  Apellido,Nombre: %s,%-11s\nCUIT: %s\n\n",auxId,auxApellido,auxNombre,auxCuit);
		retorno = 0;
	}
	return retorno;
}


/**
 * \brief obtiene un id de tipo char de un Cliente pasado por referencia.
 * \param Cliente* this: puntero de Cliente
 * \param char* id: id del tipo char donde sera guardado el id
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_getIdTxt(Cliente* this,char* id)
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
 * \brief setea a un Cliente un id de tipo char pasado por parametro, pasandolo a entero.
 * \param Cliente* this: puntero de Cliente
 * \param char* id: id del tipo char
 * \return (-1) ERROR/ (0) Ok
 */
int cliente_setIdTxt(Cliente* this,char* id)
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
 * \brief elimina todos los Clientes de la lista dinamica
 * \param linkedList* pArrayListCliente: lista dinamica de Cliente
 * \param int len: longitud de la lista.
 * \return 0 si todoOK / -1 ERROR;
 */
int cliente_deleteAllArray(LinkedList* pArrayListCliente,int len)
{
	int retorno = -1;
	Cliente* this;
	int i;
	if(pArrayListCliente != NULL && len > 0)
	{
		for(i=len-1;i>=0;i--)
		{
			this = (Cliente*)ll_get(pArrayListCliente,i);
			if(this != NULL)
			{
				cliente_delete(this);
				this = NULL;
				ll_remove(pArrayListCliente,i);
			}
		}
	}
	return retorno;
}

/**
 * \brief genera un id estatico
 * \param linkedList* pArrayListCliente: lista dinamica de Cliente
 * \return id;
 */
static int cliente_generadorDeId(LinkedList* pArrayListCliente)
{
	Cliente* this;
	static int flagIn = 0;
	static int auxId;
	int idDelUltimoCliente;
	int len = ll_len(pArrayListCliente);
	if(pArrayListCliente != NULL)
	{
		if(!flagIn)
		{
			if(len == 0)
			{
				auxId = 100;
			}else
			{
				this = ll_get(pArrayListCliente,len-1);
				if(!cliente_getId(this,&idDelUltimoCliente))
				{
					auxId = idDelUltimoCliente;
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


