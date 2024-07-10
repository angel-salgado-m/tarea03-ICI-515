#include <FilaGG1.hh>
#include <Random.hh>
#include <checkArgs.hpp>

FilaGG1* EventSimConnector::theSim;
bool     Event::enableLog;
bool     Simulator::enableLog;

int main(int argc, char* argv[])
{
	CheckArgs args(argc, argv);
	
	if(args.getArgs().randomTest){
		Random::test(10000 /*cantidad de nros*/, "randomTest.txt" /*archivo de salida*/,  true /*salir al terminar*/);
	}
	
	std::cout << "Fila de atención simple\n";

	Simulator::enableLog = args.getArgs().enableSimulatorLogs;
	Event::enableLog     = args.getArgs().enableEventsLogs;

	numCajas = 5;  // Asegúrate de que este argumento se pase correctamente
	
	FilaGG1* GG1Sim = new FilaGG1();
	EventSimConnector::theSim = GG1Sim;
	
	GG1Sim->timeMax        = 1000000;
	GG1Sim->rescheduleTime = 1000.0;
	
	// Cargar eventos del sistema
	double rate = args.getArgs().tasaLlegada; 
	double tasaSeleccionAbarrotes = args.getArgs().tasaSeleccionAbarrotes;
	double mediaAbarrotesA = args.getArgs().mediaAbarrotesA;
	double mediaAbarrotesB = args.getArgs().mediaAbarrotesB;
	double rateFallo = args.getArgs().probabilidadFallo;
	
	double tArrival = 0.0;
	for(size_t id = 0; id < args.getArgs().totalTrabajos; id += 1) {
		std::stringstream ssEvLog;
		
		double tBetweenArrivals;
		tBetweenArrivals = Random::exponential(rate);

		tArrival += tBetweenArrivals;
		Event* ev = new Llegada(tArrival, id, tasaSeleccionAbarrotes, rateFallo, mediaAbarrotesA, mediaAbarrotesB);
		ssEvLog << "Agregando en la FEL evento id=" << ev->id << ", timeArrive=" << ev->time << '\n';
		
		GG1Sim->log(ssEvLog);
		
		GG1Sim->scheduleEvent(ev);
	}

	// Ejecutar la simulación
	GG1Sim->run();

	// Mostrar resultados
	GG1Sim->showStats();
	
	delete GG1Sim;
	
	return EXIT_SUCCESS;
}
