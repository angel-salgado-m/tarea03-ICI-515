#include <FilaGG1.hh>
#include <global.hh>

FilaGG1::FilaGG1(): Simulator(), cajaLibre(true)
{
	
}

void Llegada::processEvent()
{
	std::stringstream ssEvLog;

	ssEvLog << "==> llega al minimarket.\n";

	double tiempoSeleccionAbarrotes = Random::normal(tasaSeleccionAbarrotes, 60);

	if(tiempoSeleccionAbarrotes < 0){
		tiempoSeleccionAbarrotes = -tiempoSeleccionAbarrotes;
	}

	ssEvLog << "==> id:"<< this->id << "se toma en seleccionar " << tiempoSeleccionAbarrotes << "segundos." << "\n";

	// Revisar signos, temporal
	abarrotesA = static_cast<int>(round(Random::normal(mediaAbarrotesA, 20)));
	if (abarrotesA < 0){
		abarrotesA = -abarrotesA;
	}
	ssEvLog << abarrotesA << " abarrotes de tipo A.\n";
	abarrotesB = static_cast<int>(round(Random::normal(mediaAbarrotesB, 20)));
	if (abarrotesB < 0){
		abarrotesB = -abarrotesB;
	}
	ssEvLog << abarrotesB << " abarrotes de tipo B.\n";
	this->log(ssEvLog);



	theSim->scheduleEvent(new LlegadaCaja(time + tiempoSeleccionAbarrotes, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));

}

// Eventos de llegada a la fila de la caja
// Solo planificacion preestablecida, modelo original no considera eventos de abandono.
void LlegadaCaja::processEvent()
{
	std::stringstream ssEvLog;
	
	ssEvLog << "==> llega a la fila de la caja.\n";
	this->log(ssEvLog);
	
	if( theSim->cajaLibre ){
		theSim->cajaLibre = false;
		ssEvLog << "==> pasa a la caja.\n";
		this->log(ssEvLog);
		
		Event* ev = new OcuparCaja(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB);
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
		Event* ev = new LlegadaCaja(newTime, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB);
		ev->itRescheduled = true;
		
		// (3) Se planifica el nuevo evento
		theSim->scheduleEvent(ev);	
		
		// (4) El evento actual es eliminado en el ciclo de simulación
		
	}
	
}




// Cliente es atendido por la caja
void OcuparCaja::processEvent()
{
	std::stringstream ssEvLog;
	
	theSim->cajaLibre = false;

	
	ssEvLog << "==> Llega a la caja con tiempo:" << time <<  "\n";

	this->log(ssEvLog);
	
	if (abarrotesA==0){
		theSim->scheduleEvent(new EscanearB(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));
	}else{
		theSim->scheduleEvent(new EscanearA(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));
	}

	// theSim->scheduleEvent(new Escanear(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB));
}

void EscanearA::processEvent()
{
	std::stringstream ssEvLog;


	// Tiempo que se va a demorar en pasar las compras
	uint32_t  Tservicio = Random::integer(1,10);

	ssEvLog << "==> escanean abarrotes de tipo A.\n";

	if (abarrotesA>0){
		total_a++;
		double fallo = Random::integer(0,100);
		if (fallo < rateFallo)
		{
			// El escaneo de tipo A toma entre 1 y 5 segundos
			Tservicio+=Random::integer(1,5);
			ssEvLog << "==> Se escanea abarrote. +" << Tservicio << " segundos\n";
		}else
		{
			// El escaneo al fallar toma el mismo tiempo que si hubiera sido de tipo B y se hace de forma manual
			Tservicio+=Random::integer(4,30);
			ssEvLog << "==> Fallo en el abarrote. +" << Tservicio << " segundos\n";
		}
		theSim->scheduleEvent(new EscanearA(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA-1, abarrotesB));
	}else{
		theSim->scheduleEvent(new EscanearB(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));
	}
	this->log(ssEvLog);
	// Se lleva a la finalizacion del servicio para replanificar
	// Si no quedan abarrotes de de tipo B, se finaliza el servicio. De lo contrario, se escanean los de tipo B
	if (abarrotesB==0){
		theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo));
	}else{
		theSim->scheduleEvent(new EscanearB(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));
	}
}

void EscanearB::processEvent()
{
	std::stringstream ssEvLog;

	// Escanear los articulos de tipo B toma entre 4 y 30 segundos
	uint32_t  Tservicio = Random::integer(4, 30); 

	ssEvLog << "==> escanea abarrote de tipo B.\n";
	this->log(ssEvLog);
	
	if (abarrotesB>0){
		theSim->scheduleEvent(new EscanearB(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB-1));
		total_b++;
	}
	
	theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo));
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

