#ifndef NODES_H
#define NODES_H

#include <map>
#include <utility>
#include <vector>
#include <memory>
#include "helpers.hpp"

class IPackageReceiver{

};

class ReciverPreferences{
// na diagramie UML wszystkie pola i metody maja byc publiczne, ale postanowilem ze pola zrobie prywatne

public:
    using preferences_t = std::map<IPackageReceiver*, double>; // nie wiem po co te aliasy
    using const_iterator = preferences_t::const_iterator;

    ReciverPreferences(ProbabilityGenerator pg){probability_value = std::move(pg); } //sprawdzic czy w konstruktorze nie ma tez byc preferencji

    [[nodiscard]] typename ReciverPreferences::const_iterator cbegin() const {return preferences.cbegin(); }
    [[nodiscard]] typename ReciverPreferences::const_iterator cend() const {return preferences.cend(); }
    [[nodiscard]] typename ReciverPreferences::const_iterator begin() const {return preferences.begin(); }
    [[nodiscard]] typename ReciverPreferences::const_iterator end() const {return preferences.end(); }

    void add_receiver(IPackageReceiver* r); //TODO: testy
    void remove_receiver(IPackageReceiver* r); //TODO: testy
    IPackageReceiver* choose_receiver(); //TODO: testy
    const preferences_t& get_preferences(){ return preferences; }; //TODO: testy

private: //TODO: czy zostawic prywatne
    preferences_t preferences; //sluzy do przechowywania odbiorcow i przawdopodobienstwa wybrania odbiorcy
    ProbabilityGenerator probability_value;  //"uchwyt" do funkcji zdefiniowanej w helpers do wyboru liczby losowej
};

#endif