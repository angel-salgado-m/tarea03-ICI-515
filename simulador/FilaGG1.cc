#include <FilaGG1.hh>
#include <global.hh>

FilaGG1::FilaGG1() : Simulator() {
    for (int i = 0; i < numCajas; ++i) {
        cajas.push_back({true, std::queue<Event*>()});
    }
}

void Llegada::processEvent()
{
	std::stringstream ssEvLog;

	ssEvLog << "==> llega al minimarket.\n";

	double tiempoSeleccionAbarrotes = Random::exponential(tasaSeleccionAbarrotes);

	if(tiempoSeleccionAbarrotes < 0){
		tiempoSeleccionAbarrotes = -tiempoSeleccionAbarrotes;
	}

	ssEvLog << "==> id: "<< this->id << " se toma en seleccionar " << tiempoSeleccionAbarrotes << " segundos." << "\n";

	abarrotesA = static_cast<int>(round(Random::normal(mediaAbarrotesA, 10)));
	if (abarrotesA < 0){
		abarrotesA = -abarrotesA;
	}

	ssEvLog << "Lleva: " << abarrotesA << " abarrotes de tipo A.\n";

	abarrotesB = static_cast<int>(round(Random::normal(mediaAbarrotesB, 10)));
	if (abarrotesB < 0){
		abarrotesB = -abarrotesB;
	}
	ssEvLog << "\t" << abarrotesB << " abarrotes de tipo B.\n\n";
	this->log(ssEvLog);

	theSim->scheduleEvent(new LlegadaCaja(time + tiempoSeleccionAbarrotes, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB));
}

void LlegadaCaja::processEvent() {
    std::stringstream ssEvLog;
    
    ssEvLog << "==> llega a la fila de la caja.\n";
    this->log(ssEvLog);
    
    bool atendido = false;
    for (int i = 0; i < numCajas; ++i) {
        if (cajas[i].libre) {
            cajas[i].libre = false;
            ssEvLog << "==> pasa a la caja " << i << ".\n";
            this->log(ssEvLog);
            
            Event* ev = new OcuparCaja(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB, i);
            ev->itRescheduled = false;
            theSim->scheduleEvent(ev);    
            atendido = true;
            break;
        }
    }

    if (!atendido) {
        // Si no hay cajas libres, se reprograma el evento
        double newTime = time + theSim->rescheduleTime;
        ssEvLog << std::setprecision(6) << std::fixed;
        ssEvLog << "==> todas las cajas ocupadas, replanificado para t=" << newTime << "\n";
        this->log(ssEvLog);
        
        Event* ev = new LlegadaCaja(newTime, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB);
        ev->itRescheduled = true;
        theSim->scheduleEvent(ev);    
    }
}

void OcuparCaja::processEvent() {
    std::stringstream ssEvLog;
    ssEvLog << "==> Llega a la caja " << cajaIndex << " con tiempo:" << time <<  "\n";
    this->log(ssEvLog);

    cajas[cajaIndex].libre = false;

    if (abarrotesA == 0) {
        theSim->scheduleEvent(new EscanearB(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB, cajaIndex));
    } else {
        theSim->scheduleEvent(new EscanearA(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB, cajaIndex));
    }
}

void EscanearA::processEvent() {
    std::stringstream ssEvLog;
    uint32_t Tservicio = Random::integer(1,10);

    if (abarrotesA > 0) {
        double fallo = Random::integer(0, 100);
        if (fallo < rateFallo) {
            total_a++;
            Tservicio += Random::integer(10, 30);  
            ssEvLog << "==> Fallo en el abarrote. +" << Tservicio << " segundos\n";
        } else {
            total_a++;
            ssEvLog << "==> Se escanea abarrote. +" << Tservicio << " segundos\n";
        }
        this->log(ssEvLog);
        theSim->scheduleEvent(new EscanearA(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA - 1, abarrotesB, cajaIndex));
    } else if (abarrotesA == 0 && abarrotesB > 0) {
        theSim->scheduleEvent(new EscanearB(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB, cajaIndex));
    } else if (abarrotesA == 0 && abarrotesB == 0) {
        theSim->scheduleEvent(new Salir(time, id, tasaSeleccionAbarrotes, rateFallo, cajaIndex));
    }
}

void EscanearB::processEvent() {
    std::stringstream ssEvLog;
    uint32_t Tservicio = Random::integer(10, 30);

    if (abarrotesB > 0) {
        total_b++;
        theSim->scheduleEvent(new EscanearB(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB - 1, cajaIndex));
        ssEvLog << "==> Escanea abarrote de tipo B. +" << Tservicio << " segundos\n";
        this->log(ssEvLog);
    } else if (abarrotesB == 0 && abarrotesA == 0) {
        theSim->scheduleEvent(new Salir(time + Tservicio, id, tasaSeleccionAbarrotes, rateFallo, cajaIndex));
    }
}

void Salir::processEvent() {
    std::stringstream ssEvLog;

    cajas[cajaIndex].libre = true;
    ssEvLog << "==> Fin servicio en la caja " << cajaIndex << ".\n";
    this->log(ssEvLog);

    e++;
    theSim->rescheduleDelayedEvents();
}
