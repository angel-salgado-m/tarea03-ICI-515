#pragma once

#include <Simulator.hh>
#include <Random.hh>

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

	Llegada(double tiempo, uint32_t id, int cantAbarrotesA, int cantAbarrotesB) : Event(tiempo, id, cantAbarrotesA, cantAbarrotesB) {}
	
	virtual void processEvent();

};

class LlegadaCaja : public EventSimConnector, public Event
{
public:
	LlegadaCaja(double tiempo): Event(tiempo) { 
		id = theSim->getSizeEventQueue(); 
	}

	LlegadaCaja(double tiempo, uint32_t id, int cantAbarrotesA, int cantAbarrotesB) : Event(tiempo, id, cantAbarrotesA, cantAbarrotesB) {}
	
	virtual void processEvent();

};

// Evento de ocupacion de caja
class OcuparCaja : public EventSimConnector, public Event
{
public:
	OcuparCaja(double tiempo, uint32_t id, int cantAbarrotesA, int cantAbarrotesB): Event(tiempo, id, cantAbarrotesA, cantAbarrotesB)
	{ }
	
	virtual void processEvent();

};

// Evento de escaneo de abarrotes
class Escanear : public EventSimConnector, public Event
{
public:
	Escanear(double tiempo, uint32_t id, int cantAbarrotesA, int cantAbarrotesB): Event(tiempo, id, cantAbarrotesA, cantAbarrotesB)
	{ }
	
	virtual void processEvent();

};


// Liberacion de la caja
class Salir : public EventSimConnector, public Event
{
public:
	Salir(double tiempo, uint32_t id, int cantAbarrotesA, int cantAbarrotesB  ): Event(tiempo, id, cantAbarrotesA, cantAbarrotesB)
	{ }
	
	virtual void processEvent();

};





