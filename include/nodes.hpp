#ifndef NODES_H
#define NODES_H

#include <map>
#include <vector>
#include <memory>
#include "helpers.hpp"

class IPackageReceiver{

};

class ReciverPreferences{
// na diagramie UML wszystkie pola i metody maja byc publiczne
public:

    using preferences_t = std::map<IPackageReceiver*, double>; // nie wiem po co te aliasy
    using const_iterator = preferences_t::const_iterator;

    ReciverPreferences( ProbabilityGenerator pg){probability_value = pg; } //sprawdzic czy w konstruktorze nie ma tez byc preferencji

    void add_receiver(IPackageReceiver* r); //TODO: implementacja
    void remove_receiver(IPackageReceiver* r); //TODO: przy implementacji uwzglednic uwagi szymona
    IPackageReceiver* choose_receiver(); //TODO: implementacja
    const preferences_t& get_preferences(); //TODO: implementacja

    preferences_t preferences; //sluzy do przechowywania odbiorcow i przawdopodobienstwa wybrania odbiorcy
    ProbabilityGenerator probability_value;  //"uchwyt" do funkcji zdefiniowanej w helpers do wyboru liczby losowej
};

#endif