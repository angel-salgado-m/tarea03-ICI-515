#pragma once

#include <iostream>    
#include <iomanip>
#include <queue>     
#include <vector>      
#include <sstream>
#include <string>
#include <sstream>


class Simulator;


// Execution event in a discrete event driven simulation.

class Event
{
public:
	// Construct sets time of event.
	Event(double t);
	Event(double t, uint32_t id);

	virtual ~Event() {}

	virtual void processEvent() = 0;
	
	void log(std::stringstream& tss);

	double time;
	uint32_t id;
	bool     itRescheduled;
	
	static bool enableLog;
	
	

};


struct EventComparator {
	bool operator() (const Event* left, const Event* right) const
	{
		return left->time > right->time;
	}
};

// Framework for discrete event-driven simulations.
class Simulator
{
public:
	Simulator();
	void run();
	
	void scheduleEvent(Event* newEvent);
	void rescheduleDelayedEvents();
	
	void log(std::stringstream& oss);
	
	
	uint32_t getSizeEventQueue();
	
	double time;
	double timeMax;
	
	double rescheduleTime;
	double biasDeltaTime;
	
	static bool enableLog;

protected:
	std::priority_queue<Event*, std::vector<Event *>, EventComparator> eventQueue;
};
