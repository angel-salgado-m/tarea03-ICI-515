#include <FilaGG1.hh>

FilaGG1::FilaGG1(): Simulator(), cajaLibre(true)
{
	
}


// Eventos de llegada al minimarket
void LlegadaCaja::processEvent()
{
	std::stringstream ssEvLog;
	
	ssEvLog << "==> llega a la fila de la caja.\n";
	this->log(ssEvLog);
	
	if( theSim->cajaLibre ){
		theSim->cajaLibre = false;
		
		ssEvLog << "==> pasa a la caja.\n";
		this->log(ssEvLog);
		
		Event* ev = new OcuparCaja(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB);
		ev->itRescheduled = false;
		theSim->scheduleEvent(ev);	
	}
	else{
		// El evento de llegada debe ser re-planificado.
		// el nuevo tiempo es 'newTime'
		
		// (1) determinar el tiempo de postergación
		double newTime;
		newTime = time + theSim->rescheduleTime;
		
		ssEvLog << std::setprecision(6) << std::fixed;
		ssEvLog << "==> caja ocupada, replanificado para t=" << newTime << "\n";
		this->log(ssEvLog);
		
		// (2) Se crea un nuevo evento, manteniendo el mismo identificador del 
		//     evento original
		Event* ev = new LlegadaCaja(newTime, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB);
		ev->itRescheduled = true;
		
		// (3) Se planifica el nuevo evento
		theSim->scheduleEvent(ev);	
		
		// (4) El evento actual es eliminado en el ciclo de simulación
		
	}
	
}




// Modificar logica segun cantidad de abarrotes aqui.
// Más tiempos de servicio segun tipo?
// Valor randomico para la probabilidad de se tenga que escanear manualmente un abarrote de tipo A
void OcuparCaja::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->cajaLibre = false;

	
	ssEvLog << "==> Llega a la caja con tiempo:" << time <<  "\n";
	this->log(ssEvLog);
	
	theSim->scheduleEvent(new Escanear(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
}

void Escanear::processEvent()
{
	std::stringstream ssEvLog;


	// Tiempo que se va a demorar en pasar las compras
	uint32_t  Tservicio = Random::integer(1,30);

	ssEvLog << "==> escanea abarrote.\n";
	this->log(ssEvLog);
	
	// Debe replanificar los eventos que fueron pospuestos
	theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
	
}

void Salir::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->cajaLibre = true;
	
	ssEvLog << "==> Fin servicio.\n";
	this->log(ssEvLog);
	
	
	// Debe replanificar los eventos que fueron pospuestos
	theSim->rescheduleDelayedEvents();

	
}

