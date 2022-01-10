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

//void PackageSender::send_package(){
//    IPackageReceiver* chosen_reciver = receiver_preferences_.choose_receiver(); //wybiera odbiorce
//
//    Package package_to_send = std::move(sending_bufor_.value()); //z tego co czytalem std::move nie dziala poprawnie dla std::optional
//    sending_bufor_.reset();
//
////    chosen_reciver->receive_package(std::move(package_to_send));
//}


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