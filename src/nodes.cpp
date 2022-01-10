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

void Ramp::deliver_goods(Time t) {
    if((t - last_delivery_time) == di_ramp){ //sprawdza czy w danej turze jest dostawa
        Package new_package; //tworzy nowy produkt
        Ramp::push_package(std::move(new_package)); //przekazuje do bufora (funkcjonalnosc package sender)

        last_delivery_time = t; //nowy czas dostawy
    }
}

void Worker::do_work(Time t){
    if(sending_bufor_.has_value()) {
        if((t - t_) == pd_){send_package();}
    }
    else{
        push_package(q_->pop());
        t_ = t;
    }
}

