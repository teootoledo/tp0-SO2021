/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

	t_log* logger;

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_config* config;

	logger = iniciar_logger();

	config = leer_config();

	//Asignar valor de config a la variable valor
	if(config_has_property(config, "CLAVE")){
		valor = config_get_string_value(config, "CLAVE");
		log_info(logger, valor);
		} else log_error(logger,"No existe el campo CLAVE");


	//crear conexion
	if(config_has_property(config,"IP") && config_has_property(config,"PUERTO")){
		ip = config_get_string_value(config, "IP");
		puerto = config_get_string_value(config, "PUERTO");
		log_info(logger, ip);
		log_info(logger, puerto);
	} else {
		log_error(logger, "IP y PUERTO no configurados.");
		return -1;
	}
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexión establecida.");

	//enviar CLAVE al servirdor
	enviar_mensaje(valor,conexion);
	log_info(logger, "Enviado el mensaje CLAVE.");

	//leer_consola(logger);

	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);
	log_info(logger, "Logger creado.");
	return logger;
}

t_config* leer_config(void)
{
	t_config* config = config_create("./tp0.config");
	log_info(logger, "Retorno config.");
	return config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa
	while (strcmp((leido = readline(">")), "") != 0){
		log_info(logger, leido);
	}
	log_info(logger, "Fin de lectura.");
	free(leido);
	log_info(logger, "Liberada la memoria de \"leido\".");
}

void paquete(int conexion)
{
	//Ahora toca lo divertido!
	char* leido;

	t_paquete* paquete = crear_paquete();

	while (strcmp((leido = readline(">")), "") != 0){
		agregar_a_paquete(paquete, leido, (sizeof(leido) + 1));
		log_info(logger, leido);
	}
	log_info(logger, "Fin de lectura.");
	free(leido);
	log_info(logger, "Liberada la memoria de \"leido\".");

	enviar_paquete(paquete, conexion);
	log_info(logger, "Paquete enviado.");

	eliminar_paquete(paquete);
	log_info(logger, "Paquete liberado.");

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
