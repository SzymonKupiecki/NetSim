#ifndef NODES_H
#define NODES_H

#include <map>
#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include "helpers.hpp"
#include "package.hpp"
#include "storage_types.hpp"

enum class ReceiverType{
    WORKER, STOREHOUSE
};

class IPackageReceiver{
    public:
    virtual void receive_package(Package&& p) = 0;
    virtual  ElementID get_id() const = 0;
//#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
//    virtual ReceiverType get_receiver_type() = 0;
//#endif
};

class ReceiverPreferences{
// na diagramie UML wszystkie pola i metody maja byc publiczne, ale postanowilem ze pola zrobie prywatne

public:
    using preferences_t = std::map<IPackageReceiver*, double>;
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

    void send_package(); //wysyla przetworzona wartosc do odbiorcy //TODO: implementacja/poprawnosc
    void push_package(Package&& package){ sending_bufor_ = std::move(package); }; //sluzy do wstawiania wartosci do bufora //TODO: test
    const std::optional<Package>& get_sending_buffer(){ return sending_bufor_; };

    ReceiverPreferences receiver_preferences_;

protected:
    std::optional<Package> sending_bufor_; // pole bufora
};


class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di): PackageSender(), ramp_id_(id), di_ramp(di) {}


    ~Ramp() = default;
    void deliver_goods(Time t);
    [[nodiscard]] const TimeOffset& get_delivery_interval() const {return di_ramp; };
    [[nodiscard]] const ElementID& get_id() const {return ramp_id_; };



private:
    ElementID ramp_id_;
    TimeOffset di_ramp;
};
class Worker: public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q = std::make_unique<IPackageQueue>(PackageQueueType::LIFO)): PackageSender(), IPackageReceiver(), id_(id), pd_(pd), q_(std::move(q)), t_(0) {}

    void do_work(Time t);
    void receive_package(Package&& p) override{q_->push(std::move(p));}
//    ReceiverType get_receiver_type() override {return ReceiverType::WORKER;}
    ElementID get_id() const override {return id_;}
    TimeOffset get_processing_duration() const{return pd_;};
    Time get_package_processing_start_time() const{return t_;};

protected:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time t_=0;
};

class Storehouse: public IPackageReceiver, public IPackageStockpile{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<IPackageStockpile>()): id_(id), d_(std::move(d)) {}


    ElementID get_id() const override {return id_;}

//    ReceiverType get_receiver_type() override{return ReceiverType::STOREHOUSE;}

    void receive_package(Package&& p) override{push(std::move(p));}
protected:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};
#endif
