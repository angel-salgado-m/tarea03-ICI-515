#pragma once

#include <Simulator.hh>
#include <Random.hh>

class FilaGG1 : public Simulator
{
public:
	bool servidorLibre;
	
	FilaGG1();
	
};

class EventSimConnector 
{
public:		
	static FilaGG1* theSim;
};


class Llegada : public EventSimConnector, public Event
{
public:
	Llegada(double tiempo): Event(tiempo) { 
		id = theSim->getSizeEventQueue(); 
	}
	Llegada(double tiempo, uint32_t id): Event(tiempo, id) {}
	
	virtual void processEvent();

};

class OcuparServidor : public EventSimConnector, public Event
{
public:
	OcuparServidor(double tiempo, uint32_t id): Event(tiempo, id)
	{ }
	
	virtual void processEvent();

};


class Salir : public EventSimConnector, public Event
{
public:
	Salir(double tiempo, uint32_t id): Event(tiempo, id)
	{ }
	
	virtual void processEvent();

};





