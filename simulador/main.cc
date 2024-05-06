/**********************************************************************************************
 *
 * Ejemplo de implementación de fila G/G/1/inf
 *
 **********************************************************************************************/


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
	
	FilaGG1*  GG1Sim = new FilaGG1();
	EventSimConnector::theSim = GG1Sim;
	
	GG1Sim->timeMax        = 1000000;
	GG1Sim->rescheduleTime = 1000.0;
	
	// Cargar eventos del sistema
	double rate = args.getArgs().tasaLlegada; 
	
	double tArrival = 0.0;
	for(size_t id = 0; id < args.getArgs().totalTrabajos; id += 1) {
		std::stringstream ssEvLog;
		
		double tBetweenArrivals;
		tBetweenArrivals = Random::exponential(rate);

		tArrival += tBetweenArrivals;
		Event* ev = new Llegada(tArrival, id);
		ssEvLog << "Agregando en la FEL evento id=" << ev->id << ", timeArrive=" << ev->time << '\n';
		
		GG1Sim->log(ssEvLog);
		
		GG1Sim->scheduleEvent(ev);
	}

	// Ejecutar la simulación
	GG1Sim->run();
	
	delete GG1Sim;
	
	return EXIT_SUCCESS;
}
