#pragma once

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

extern char *optarg;
extern int optind, opterr, optopt;

class CheckArgs {
private:
	// 1) Modificar esta sección
	const std::string optString = "j:r:t:seh";
	
	const struct option optStringLong[7] = {
		{"jobs"   , required_argument, nullptr, 'j'},
		{"rate", required_argument, nullptr, 'r'},
		{"test", required_argument, nullptr, 't'},
		{"slogs", no_argument, nullptr, 's'},
		{"elogs", no_argument, nullptr, 'e'},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, no_argument, nullptr, 0}
	};

	const std::string opciones = "--jobs <total jobs> --rate <rate> [--test][--simlogs][--eventslogs][--help]";

	const std::string descripcion  = "Descripción:\n"
	                                "\t--jobs     cantidad total de trabajos a simular (integer).\n"
						 			"\t--rate     tasa de llegada de trabajos (cada da 'rate' unidades de tiempo llega un elemento al sistema).\n"
						 			"\t--test     genera archivo de pruebas de nros aleatorios y termina.\n"
									"\t--slogs    habilita logs del simulador en pantalla.\n"
									"\t--elogs    habilita logs de los eventos en pantalla.\n"
						 			"\t-h   Muestra esta salida y termina.\n";

	typedef struct args_t {
		uint32_t  totalTrabajos;
		double    tasaLlegada;
		bool      randomTest;
		bool      enableSimulatorLogs;
		bool      enableEventsLogs;
	} args_t;

	// 2) Modificar constructor
	// 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
	// 4) Recuerde que para compilar nuevamente, y por tratarse
	//    de un archivo header, debe hacer primero un make clean

	args_t  parametros;

	int argc;
	char **argv;


public:
	CheckArgs(int _argc, char **_argv);
	~CheckArgs();
	void   loadArgs();
	args_t getArgs();

private:
	void printUsage();

};

CheckArgs::CheckArgs(int _argc, char **_argv)
{
	parametros.totalTrabajos = 0;
	parametros.tasaLlegada   = 0;
	parametros.randomTest    = false;
	parametros.enableSimulatorLogs    = false;
	parametros.enableEventsLogs = false;

	argc = _argc;
	argv = _argv;


	loadArgs();

}

CheckArgs::~CheckArgs()
{

}

void CheckArgs::loadArgs()
{
	int opcion;
	int option_index;
	while ((opcion = getopt_long (argc, argv, optString.c_str(),  optStringLong, &option_index)) != -1) {
		switch (opcion) {
		case 'j':
			parametros.totalTrabajos = std::atoi(optarg);
			break;
		case 'r':
			parametros.tasaLlegada = std::atof(optarg);
			break;
		case 's':
			parametros.enableSimulatorLogs = true;
			break;
		case 'e':
			parametros.enableEventsLogs = true;
			break;
		case 't':
			parametros.randomTest = true;
			break;
		case 'h':
		default:
			printUsage();
			exit(EXIT_FAILURE);
		}
	}

	if ( parametros.totalTrabajos == 0 || parametros.tasaLlegada == 0  ) {
		printUsage();
		exit(EXIT_FAILURE);
	}

}

CheckArgs::args_t CheckArgs::getArgs()
{
	return(parametros);
}

void CheckArgs::printUsage()
{
	std::cout << "Uso: " <<
	          argv[0] << " " << opciones << "\n" << descripcion << std::endl;
}

