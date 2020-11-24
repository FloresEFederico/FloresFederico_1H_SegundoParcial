
#ifndef CLIENTES_H_
#define CLIENTES_H_
#include "LinkedList.h"
#define NOMBRE_LEN 56
#define CUIT_LEN 32
typedef struct
{
	int id;
	char nombre[NOMBRE_LEN];
	char apellido[NOMBRE_LEN];
	char cuit[CUIT_LEN];

}Cliente;

Cliente* cliente_new();
Cliente* cliente_newParametros(int id,char* nombreStr,char* apellidoStr,char* cuitStr);
void cliente_delete(Cliente* this);

int cliente_setId(Cliente* this,int id);
int cliente_getId(Cliente* this,int* id);

int cliente_setNombre(Cliente* this,char* nombre);
int cliente_getNombre(Cliente* this,char* nombre);

int cliente_setApellido(Cliente* this,char* apellido);
int cliente_getApellido(Cliente* this,char* apellido);

int cliente_setCuit(Cliente* this,char* cuit);
int cliente_getCuit(Cliente* this,char* cuit);


int cliente_altaCliente(LinkedList* pArrayListCliente);
int cliente_imprimirCliente(void* this);
int cliente_setIdTxt(Cliente* this,char* id);
int cliente_getIdTxt(Cliente* this,char* id);
int cliente_buscarPorId(LinkedList* pArrayListCliente,int idABuscar);
int cliente_buscarPorCuit(LinkedList* pArrayListCliente,char* cuitABuscar);

#endif /* CLIENTES_H_ */
