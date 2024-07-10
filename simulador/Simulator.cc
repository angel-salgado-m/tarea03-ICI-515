#include <Simulator.hh>
#include <global.hh>

Event::Event(double t) : time(t), id(0), itRescheduled(false)
{ }

Event::Event(double t, uint32_t id) : time(t), id(id), itRescheduled(false)
{ }

Event::Event(double t, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo) : time(t), id(id), itRescheduled(false), tasaSeleccionAbarrotes(tasaSeleccionAbarrotes), rateFallo(rateFallo)
{ }

Event::Event(double t, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, double mediaAbarrotesA, double mediaAbarrotesB) : time(t), id(id), itRescheduled(false), tasaSeleccionAbarrotes(tasaSeleccionAbarrotes), rateFallo(rateFallo), mediaAbarrotesA(mediaAbarrotesA), mediaAbarrotesB(mediaAbarrotesB)
{ }

Event::Event(double tiempo, uint32_t id, double tasaSeleccionAbarrotes, double rateFallo, int abarrotesA, int abarrotesB) : time(tiempo), id(id), itRescheduled(false), tasaSeleccionAbarrotes(tasaSeleccionAbarrotes), rateFallo(rateFallo), abarrotesA(abarrotesA), abarrotesB(abarrotesB)
{ }

void Event::log(std::stringstream& tss)
{
	if(enableLog){
		std::stringstream header;
	
		header << std::setprecision(6) << std::fixed;
		header << this->time << "\t";
		header << "id: " << this->id   << "\t";
	
		std::cout << header.str();
		std::cout << tss.str();
	}
	tss.str("");
}

Simulator::Simulator() : time(0), timeMax(0), eventQueue(), biasDeltaTime(1e-6)
{ }

void Simulator::run ()
{
	while(time <= timeMax && !eventQueue.empty() ){
		Event* nextEvent = eventQueue.top();
		eventQueue.pop();
		time = nextEvent->time;
		nextEvent->processEvent();
		delete nextEvent;
	}
}

void Simulator::scheduleEvent(Event* newEvent)
{
	eventQueue.push(newEvent);
}

void Simulator::rescheduleDelayedEvents()
{
	std::priority_queue<Event*, std::vector<Event *>, EventComparator> eventQueueTMP;
	std::stringstream ssLog;
	
	double biasTime = 0.0;
	
	ssLog << "Reprogramando FEL\n";
	
	while( !eventQueue.empty() ){
		Event* currentEvent = eventQueue.top();
		
		ssLog << std::setprecision(6) << std::fixed;
		ssLog << "reprogramando id=" << currentEvent->id << ", time=" << currentEvent->time;
		ssLog << "\t itRescheduled=" << currentEvent->itRescheduled << "\n";
		
		if(currentEvent->itRescheduled){
			currentEvent->time = this->time + biasTime;
			currentEvent->itRescheduled = false;
			ssLog << "\ttime-new=" << currentEvent->time << "\n";
						
			biasTime += this->biasDeltaTime;
		}
		eventQueueTMP.push(currentEvent);
		eventQueue.pop();
	}
	eventQueue = eventQueueTMP;
	this->log(ssLog);
}

uint32_t Simulator::getSizeEventQueue()
{
	return(eventQueue.size());
}

void Simulator::showStats()
{
	std::cout << "Simulación finalizada\n";
	std::cout << "Tiempo de servicio: " << time << " = " << time/60 << " minutos.\n";
	std::cout << "Cantidad total de abarrotes de tipo A vendidos: " << total_a << "\n";
	std::cout << "Cantidad total de abarrotes de tipo B vendidos: " << total_b << "\n";
	std::cout << "Cantidad total de trabajos realizados: " << e << "\n";
}

void Simulator::log(std::stringstream& oss)
{
	if(enableLog){
		std::cout << oss.str();
	}
	oss.str("");
}
