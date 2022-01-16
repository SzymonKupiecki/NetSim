#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    // funkcja na razie ma nie zajmowac sie odbiorcami z konkretnym prawdopodobienstwem,
    // tak samo w remove_receiver:
    preferences_.insert(std::make_pair(r,0));
    auto length = double(std::size(preferences_));
    for(auto &x: preferences_){
        x.second = 1/length;
    }

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
    double random = (probability_value_)(); //warosc od 0 do 1
    std::vector<double> steps{0};
    double sth;
    for(auto& reciver: preferences_){
        sth += reciver.second;
        steps.push_back(sth);
    }
    for(std::size_t i=0; i<steps.size(); i++){
        if(steps[i] < random && random <= steps[i+1]) {
            std::size_t sth2 = 0;
            for (auto &receiver: preferences_) {
                if (sth2 == i) { return receiver.first; }
                else { sth2++; }
            }
        }
    }
    return nullptr; //cos poszlo nie tak
}

void PackageSender::send_package(){
    if(sending_bufor_.has_value()) {
        IPackageReceiver *chosen_reciver = receiver_preferences_.choose_receiver(); //wybiera odbiorce

        Package package_to_send = std::move(
                sending_bufor_.value()); //z tego co czytalem std::move nie dziala poprawnie dla std::optional
        sending_bufor_.reset();

        chosen_reciver->receive_package(std::move(package_to_send));
    }
}

void Ramp::deliver_goods(Time t) {
    if( (t-1) % di_ramp == 0){ //sprawdza czy w danej turze jest dostawa
        Package new_package; //tworzy nowy produkt
        Ramp::push_package(std::move(new_package)); //przekazuje do bufora (funkcjonalnosc package sender)

    }
}

void Worker::do_work(Time t){
    if(sending_bufor_.has_value()) {
        if((t - t_) == pd_){send_package();}

    } else if(get_queue()->empty()){
        //TODO:chyba po prostu nic nie robi
    }else{
        push_package(q_->pop());
        t_ = t;
    }
}
