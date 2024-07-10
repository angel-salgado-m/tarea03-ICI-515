#pragma once

#include <queue>
#include <vector>

// Declaración anticipada de la clase Event
class Event;

// Estructura para representar una caja
struct Caja {
    bool libre;
    std::queue<Event*> fila;
};

// Variables globales
extern std::vector<Caja> cajas;
extern int numCajas;

extern int total_a;  // Declaración del primer contador global.
extern int total_b;  // Declaración del segundo contador global.
extern int e;
