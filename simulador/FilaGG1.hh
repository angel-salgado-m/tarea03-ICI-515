#pragma once

#include <Simulator.hh>
#include <Random.hh>
#include "global.hh"
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

	Llegada(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB) {}
	
	virtual void processEvent();

};

class LlegadaCaja : public EventSimConnector, public Event
{
public:
	LlegadaCaja(double tiempo): Event(tiempo) { 
		id = theSim->getSizeEventQueue(); 
	}

	LlegadaCaja(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB) {}
	
	virtual void processEvent();

};

// Evento de ocupacion de caja
class OcuparCaja : public EventSimConnector, public Event
{
public:
	OcuparCaja(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB,double abarrotesA,double abarrotesB) : Event(time, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB, abarrotesA, abarrotesB)
	{ }
	
	virtual void processEvent();

};

// Evento de escaneo de abarrotes
class EscanearA : public EventSimConnector, public Event
{
public:
	EscanearA(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB)
	{ }
	
	virtual void processEvent();

};

class EscanearB : public EventSimConnector, public Event
{
public:
	EscanearB(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB)
	{ }
	
	virtual void processEvent();

};
// Liberacion de la caja
class Salir : public EventSimConnector, public Event
{
public:
	Salir(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double tiempoAbarrotesA, double tiempoAbarrotesB) : Event(tiempo, id, tasaSeleccionAbarrotes, rateFallo, tiempoAbarrotesA, tiempoAbarrotesB)
	{ }
	
	virtual void processEvent();

};





