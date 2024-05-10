#include <FilaGG1.hh>
#include <global.hh>

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
		double abarrotesA = Random::integer(0,29);
		double abarrotesB = Random::integer(0,29);
		
		Event* ev = new OcuparCaja(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB, abarrotesA, abarrotesB);
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




// Cliente llega a la caja
void OcuparCaja::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->cajaLibre = false;

	
	ssEvLog << "==> Llega a la caja con tiempo:" << time <<  "\n";

	this->log(ssEvLog);
	
	if (abarrotesA==0){
		theSim->scheduleEvent(new EscanearB(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB, abarrotesA, abarrotesB));
	}else{
		theSim->scheduleEvent(new EscanearA(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB, abarrotesA, abarrotesB));
	}

	// theSim->scheduleEvent(new Escanear(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
}

void EscanearA::processEvent()
{
	std::stringstream ssEvLog;


	// Tiempo que se va a demorar en pasar las compras
	uint32_t  Tservicio = Random::integer(1,10);

	ssEvLog << "==> escanea abarrote de tipo A.\n";
	this->log(ssEvLog);
	while (abarrotesA>0)
	{
		// cambiar nombre, tu cachai
		double abarrote = Random::integer(0,100);
		if (abarrote<rateFallo)
		{
			
			abarrotesA--;
			total_a++;
			Tservicio+=Random::integer(1,5);
			ssEvLog << "==> Se escanea abarrote. +" << Tservicio << " segundos\n";

		}else{
			abarrotesA--;
			total_a++;
			Tservicio+=Random::integer(1,30);
			ssEvLog << "==> Fallo en el abarrote. +" << Tservicio << " segundos\n";
			this->log(ssEvLog);
		}
	}

	// Debe replanificar los eventos que fueron pospuestos
	if (abarrotesB==0){
		theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
	}else{
		theSim->scheduleEvent(new EscanearB(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB, abarrotesA, abarrotesB));
	}
}

void EscanearB::processEvent()
{
	std::stringstream ssEvLog;

	uint32_t  Tservicio = Random::integer(1,10);

	ssEvLog << "==> escanea abarrote de tipo B.\n";
	this->log(ssEvLog);

	while (abarrotesB>0)
	{
		abarrotesB--;
		total_b++;
		Tservicio+=Random::integer(1,5);
	}
	theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
}

void Salir::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->cajaLibre = true;
	
	ssEvLog << "==> Fin servicio.\n";
	this->log(ssEvLog);
	
	e++;
	// Debe replanificar los eventos que fueron pospuestos
	theSim->rescheduleDelayedEvents();

	
}

