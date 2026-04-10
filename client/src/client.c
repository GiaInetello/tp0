#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config, "IP"); //te trae el valor de la ip
	puerto = config_get_string_value(config, "PUERTO"); //el puerto
	valor = config_get_string_value(config, "CLAVE"); //el valor clave

	// Loggeamos el valor de config
	log_info(logger, "IP: %s", ip); //IMPORTANTE EL %s para agregar la variable
	log_info(logger, "Puerto: %s", puerto);
	log_info(logger, "Clave: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO); //true para que lo muestre por pantalla

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config"); //creamos el archivo config
	if(nuevo_config==NULL){
		//no se puede crear el config, hay que terminar el programa
		abort();
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	while(leido && strcmp(leido, "") != 0 ){ //si leido no es NULL ni es un caracter vacio
		log_info(logger, "COnsola: %s", leido);  //lo logea
		free(leido); //libera la memoria
		leido = readline("> "); //pide otra linea
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido= readline("> ");

	while (leido && strcmp(leido, "") !=0)
	{
		// aca agrego el string al buffer del paquete
		agregar_a_paquete(paquete, leido, strlen(leido)+1); //strlen mide el tamaño del dato
		free(leido);
		leido = readline("> ");
	}
	free(leido);

	enviar_paquete(paquete, conexion); //enviamos el paquete

	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger); //libera el log para que no haya un memory leak
	config_destroy(config); //destruye el archivo config
	liberar_conexion(conexion);
}
