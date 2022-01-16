#ifndef SIMULATION_H
#define SIMULATION_H
#include <utility>

#include "factory.hpp"


void simulate(Factory& f, TimeOffset d, const std::function<void (Factory&, Time)>& rf);


#endif