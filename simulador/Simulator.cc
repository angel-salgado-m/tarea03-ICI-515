#include <Simulator.hh>


Event::Event(double t) : time(t), id(0), itRescheduled(false)
{

}


Event::Event(double t, uint32_t id) : time(t), id(id), itRescheduled(false)
{ 
	
}


void Event::log(std::stringstream& tss)
{
	if(enableLog){
		std::stringstream header;
	
		header << std::setprecision(6) << std::fixed;
		header << this->time << "\t";
		header << this->id   << "\t";
	
		std::cout << header.str();
		std::cout << tss.str();
		
	}
	tss.str("");
	
}


Simulator::Simulator() : time(0), timeMax(0), eventQueue(), biasDeltaTime(1e-6)
{
	
}

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


/*
	Reprograma todos los eventos que fueron replanificados 
	para el futuro. Por ejemplo, puden ser eventos de llegada que no pueden 
	ser atendidos debido a que el servidor está ocupado.
*/
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
		ssLog << ", itRescheduled=" << currentEvent->itRescheduled << "\n";
		
	
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


void Simulator::log(std::stringstream& oss)
{
	if(enableLog){
		std::cout << oss.str();
	}
	
	oss.str("");
	
}









