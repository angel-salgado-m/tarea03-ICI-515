#pragma once

#include <Simulator.hh>
#include <Random.hh>
#include <global.hh>
class FilaGG1 : public Simulator
{
public:
	bool cajaLibre;
	
	FilaGG1();
	
};

class EventSimConnector 
{
public:		
	static FilaGG1* theSim;
};


// Eventos de llegada al minimarket
class Llegada : public EventSimConnector, public Event
{
public:
	Llegada(double tiempo): Event(tiempo) { 
		id = theSim->getSizeEventQueue(); 
	}
	Llegada(double tiempo, uint32_t id): Event(tiempo, id) {}

	Llegada(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double mediaAbarrotesA, double mediaAbarrotesB ) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, mediaAbarrotesA, mediaAbarrotesB) {}
	
	virtual void processEvent();

};

// LLegada a la fila de la caja, la cantidad de abarrotes ya fueron seleccionados.
class LlegadaCaja : public EventSimConnector, public Event
{
public:
	LlegadaCaja(double tiempo): Event(tiempo) { 
		id = theSim->getSizeEventQueue(); 
	}

	LlegadaCaja(double time, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, int abarrotesA, int abarrotesB) : Event(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB) 
	{ }
	
	virtual void processEvent();

};

// Evento de ocupacion de caja
class OcuparCaja : public EventSimConnector, public Event
{
public:
	OcuparCaja(double time, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, int abarrotesA, int abarrotesB) : Event(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB)
	{ }
	
	virtual void processEvent();

};

// Evento de escaneo de abarrotes
class EscanearA : public EventSimConnector, public Event
{
public:
	EscanearA(double time, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, int abarrotesA, int abarrotesB) : Event(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB)
	{ }
	
	virtual void processEvent();

};

class EscanearB : public EventSimConnector, public Event
{
public:
	EscanearB(double time, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, int abarrotesA, int abarrotesB) : Event(time, id, tasaSeleccionAbarrotes, rateFallo, abarrotesA, abarrotesB)
	{ }
	
	virtual void processEvent();

};
// Liberacion de la caja
class Salir : public EventSimConnector, public Event
{
public:
	Salir(double time, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo) : Event(time, id, tasaSeleccionAbarrotes, rateFallo)
	{ }
	
	virtual void processEvent();

};





