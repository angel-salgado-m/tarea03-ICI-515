#include <FilaGG1.hh>

FilaGG1::FilaGG1(): Simulator(), servidorLibre(true)
{
	
}

void Llegada::processEvent()
{
	std::stringstream ssEvLog;
	
	ssEvLog << "==> llega  a la fila.\n";
	this->log(ssEvLog);
	
	if( theSim->servidorLibre ){
		theSim->servidorLibre = false;
		
		ssEvLog << "==> pasa al servidor.\n";
		this->log(ssEvLog);
		
		Event* ev = new OcuparServidor(time, id);
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
		ssEvLog << "==> servidor ocupado, replanificado para t=" << newTime << "\n";
		this->log(ssEvLog);
		
		// (2) Se crea un nuevo evento, manteniendo el mismo identificador del 
		//     evento original
		Event* ev = new Llegada(newTime, id);
		ev->itRescheduled = true;
		
		// (3) Se planifica el nuevo evento
		theSim->scheduleEvent(ev);	
		
		// (4) El evento actual es eliminado en el ciclo de simulación
		
	}
	
}


void OcuparServidor::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->servidorLibre = false;
	// Tiempo que se va a demorar en pasar las compras
	uint32_t  Tservicio = Random::integer(1,30);
	
	ssEvLog << "==> empieza a ocupar servidor. Tiempo de servicio:" << Tservicio << "\n";
	this->log(ssEvLog);
	
	theSim->scheduleEvent(new Salir(time + Tservicio, id));
}




void Salir::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->servidorLibre = true;
	
	ssEvLog << "==> Fin servicio.\n";
	this->log(ssEvLog);
	
	
	// Debe replanificar los eventos que fueron pospuestos
	theSim->rescheduleDelayedEvents();

	
}

