#ifndef NODES_H
#define NODES_H

#include <map>
#include <vector>
#include <memory>
#include "helpers.hpp"

class IPackageReceiver{

};

class ReciverPreferences{
    using preferences_t = std::map<IPackageReceiver*, double>; // nie wiem po co te aliasy
    using const_iterator = preferences_t::const_iterator;


private:
    preferences_t preferences; //sluzy do przechowywania odbiorcow i przawdopodobienstwa wybrania odbiorcy
    ProbabilityGenerator probability_value = probability_generator;  //"uchwyt" do funkcji zdefiniowanej w helpers do wyboru liczby losowej
};

#endif