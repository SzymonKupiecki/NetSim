#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    // funkcja na razie ma nie zajmowac sie odbiorcami z konkretnym prawdopodobienstwem,
    // tak samo w remove_receiver:
    double prawdopodobienstwo = 1/(double(preferences_.size()) + 1.0 );

    for(auto &x: preferences_){
        x.second = prawdopodobienstwo;
    }

    preferences_.insert(std::pair<IPackageReceiver*, double>(r, prawdopodobienstwo));
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    //funkcja dziala rowniez gdy r nie ma w mapie preferences_
    if(preferences_.find(r) != preferences_.end()){
        preferences_.erase(r);

        // funkcja na razie ma nie zajmowac sie odbiorcami z konkretnym prawdopodobienstwem, wiec:
        double prawdopodobienstwo = 1/( double(preferences_.size()));

        for(auto &x: preferences_){
            x.second = prawdopodobienstwo;
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver(){
    double random = (*probability_value_)(); //warosc od 0 do 1 //TODO: sprawdzic funkcjonalnosc
    double dystrybuanta_1 = 0;
    double dystrybuanta_2 = 0;

    for(auto reciver: preferences_){
        dystrybuanta_1 = dystrybuanta_2;
        dystrybuanta_2 += reciver.second;

        if(dystrybuanta_1 < random and random < dystrybuanta_2){
            return reciver.first;
        }
    }
    return nullptr; //cos poszlo nie tak
}

void PackageSender::send_package(){
    IPackageReceiver* chosen_reciver = receiver_preferences_.choose_receiver(); //wybiera odbiorce

    Package package_to_send = std::move(sending_bufor_.value()); //z tego co czytalem std::move nie dziala poprawnie dla std::optional
    sending_bufor_.reset();

    chosen_reciver->receive_package(std::move(package_to_send));
}

// Inicjalizacja statycznych pol klasy
std::set<ElementID> ramp_assigned_IDs;
std::set<ElementID> ramp_freed_IDs;

Ramp::Ramp(ElementID id, TimeOffset di){
    //praktycznie przepisane z package
    if(not ramp_freed_IDs.empty()){
        ramp_id_ = *ramp_freed_IDs.begin();
        ramp_freed_IDs.erase(ramp_freed_IDs.begin());
    }
    else{
        auto last_element = ramp_assigned_IDs.end();
        if(not ramp_assigned_IDs.empty()){last_element--;} //wskaznik na ostatni element
        // w przypadku pierwszej inicjalizacji
        if(ramp_assigned_IDs.empty()){ramp_id_ = 1;}
            //na wypadek wczesniejszej inicjalizacji np. Package(2) bez Package(1)
        else if(ramp_assigned_IDs.size() != *last_element){
            for(ElementID i = 1; i < *last_element; i++){
                if(ramp_assigned_IDs.count(i) == 0){
                    ramp_id_ = i;
                    break;
                }
            }
        }
        else{
            ramp_id_ = *last_element + 1;
        }
        ramp_assigned_IDs.insert(ramp_id_); // set nie przechowuje duplikatow
    }
    di_ramp = di; //czas miedzy dostawami
}

Ramp::~Ramp() {
    ramp_freed_IDs.insert(ramp_id_);
}
void Ramp::deliver_goods(Time t) {
    if((t - last_delivery_time) == di_ramp){ //sprawdza czy w danej turze jest dostawa
        Package new_package; //tworzy nowy produkt
        Ramp::push_package(std::move(new_package)); //przekazuje do bufora (funkcjonalnosc package sender)

        last_delivery_time = t; //nowy czas dostawy
    }
}
// Inicjalizacja statycznych pol klasy
std::set<ElementID> Worker::assigned_IDs;
std::set<ElementID> Worker::freed_IDs;

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q = std::make_unique<PackageQueue>(PackageQueueType::FIFO)){
    id_ = id;
    pd_ = pd;
    q_ = q;
    //jesli sa jakies zwolnione ID
    if(not freed_IDs.empty()){
        id_ = *freed_IDs.begin(); // set jest posortowany
        freed_IDs.erase(freed_IDs.begin()); //usuwanie zajetego ID
    }
        // jesli nie
    else{
        auto last_element = assigned_IDs.end();
        if(not assigned_IDs.empty()){last_element--;} //wskaznik na ostatni element
        // w przypadku pierwszej inicjalizacji
        if(assigned_IDs.empty()){id_ = 1;}
            //na wypadek wczesniejszej inicjalizacji np. Worker(2) bez Worker(1)
        else if(assigned_IDs.size() != *last_element){
            for(ElementID i = 1; i < *last_element; i++){
                if(assigned_IDs.count(i) == 0){
                    id_ = i;
                    break;
                }
            }
        }
        else{
            id_ = *last_element + 1;
        }
        assigned_IDs.insert(id_); // set nie przechowuje duplikatow
    }
}
Worker::Worker(ElementID id){
    id_ = id;
    assigned_IDs.insert(id);
}
// Dodanie usuwanego ID do zwolnionych ID
Worker::~Worker() {
    freed_IDs.insert(id_);
}
void Worker::do_work(Time t){
    if(sending_bufor_.has_value()) {
        if(t - t_ = pd_){
            sending_bufor.send_package();
        }
    }
    else{
        sending_bufor.push_package(q_.pop());
        t_ = t;
    }
}
void Worker::receive_package(Package&& p){
    q_.push(std::move(p));
}
void Worker::get_id() {return id_;};

std::set<ElementID> Storehouse::assigned_IDs;
std::set<ElementID> Storehouse::freed_IDs;

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d){
    id_ = id;
    d_ = d;
    //jesli sa jakies zwolnione ID
    if(not freed_IDs.empty()){
        id_ = *freed_IDs.begin(); // set jest posortowany
        freed_IDs.erase(freed_IDs.begin()); //usuwanie zajetego ID
    }
        // jesli nie
    else{
        auto last_element = assigned_IDs.end();
        if(not assigned_IDs.empty()){last_element--;} //wskaznik na ostatni element
        // w przypadku pierwszej inicjalizacji
        if(assigned_IDs.empty()){id_ = 1;}
            //na wypadek wczesniejszej inicjalizacji np. Worker(2) bez Worker(1)
        else if(assigned_IDs.size() != *last_element){
            for(ElementID i = 1; i < *last_element; i++){
                if(assigned_IDs.count(i) == 0){
                    id_ = i;
                    break;
                }
            }
        }
        else{
            id_ = *last_element + 1;
        }
        assigned_IDs.insert(id_); // set nie przechowuje duplikatow
    }
}
Storehouse::Storehouse(ElementID id){
    id_ = id;
    assigned_IDs.insert(id);
}
// Dodanie usuwanego ID do zwolnionych ID
Storehouse::~Storehouse() {
    freed_IDs.insert(id_);
}
void Storehouse::receive_package(Package&& p){
    queue_.push(std::move(p));
}

void Storehouse::get_id(){
    return id_;
}
