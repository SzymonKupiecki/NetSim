#ifndef NODES_H
#define NODES_H

#include <map>
#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include "helpers.hpp"
#include "package.hpp"

class IPackageReceiver{
};

class ReceiverPreferences{
// na diagramie UML wszystkie pola i metody maja byc publiczne, ale postanowilem ze pola zrobie prywatne

public:
    using preferences_t = std::map<IPackageReceiver*, double>; // nie wiem po co te aliasy
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator* pg = &probability_generator): probability_value_(pg){} //sprawdzic czy w konstruktorze nie ma tez byc preferencji

    [[nodiscard]] typename ReceiverPreferences::const_iterator cbegin() const {return preferences_.cbegin(); }
    [[nodiscard]] typename ReceiverPreferences::const_iterator cend() const {return preferences_.cend(); }
    [[nodiscard]] typename ReceiverPreferences::const_iterator begin() const {return preferences_.begin(); }
    [[nodiscard]] typename ReceiverPreferences::const_iterator end() const {return preferences_.end(); }

    void add_receiver(IPackageReceiver* r); //TODO: testy
    void remove_receiver(IPackageReceiver* r); //TODO: testy
    IPackageReceiver* choose_receiver(); //TODO: testy
    const preferences_t& get_preferences(){ return preferences_; }; //TODO: testy

protected: //TODO: czy zostawic prywatne
    preferences_t preferences_; //sluzy do przechowywania odbiorcow i przawdopodobienstwa wybrania odbiorcy
    ProbabilityGenerator* probability_value_;  //"uchwyt" do funkcji zdefiniowanej w helpers do wyboru liczby losowej
};


class PackageSender{
public:
    PackageSender(PackageSender&&) = default; //nie wiem po co

    PackageSender() = default;

//    void send_package(); //wysyla przetworzona wartosc do odbiorcy //TODO: implementacja/poprawnosc
    void push_package(Package&& package){ sending_bufor_ = std::move(package); }; //sluzy do wstawiania wartosci do bufora //TODO: test
    const std::optional<Package>& get_sending_buffer(){ return sending_bufor_; };

    ReceiverPreferences receiver_preferences_;

protected:
    std::optional<Package> sending_bufor_; // pole bufora
};


class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di);

    ~Ramp(); //TODO: test
    void deliver_goods(Time t); //TODO: test, zamienic na modulo
    [[nodiscard]] const TimeOffset& get_delivery_interval() const {return di_ramp; };
    [[nodiscard]] const ElementID& get_id() const {return ramp_id_; };

    //TODO: implementacja przetworzenia

private:
    Time last_delivery_time;
    TimeOffset di_ramp;

    ElementID ramp_id_;
    static std::set<ElementID> ramp_assigned_IDs;
    static std::set<ElementID> ramp_freed_IDs;
};
#endif