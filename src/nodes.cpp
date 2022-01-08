#include "nodes.hpp"

void ReciverPreferences::add_receiver(IPackageReceiver* r){
    // funkcja na razie ma nie zajmowac sie odbiorcami z konkretnym prawdopodobienstwem,
    // tak samo w remove_receiver:
    double prawdopodobienstwo = 1/( double(preferences.size()) + 1.0 );

    for(auto &x: preferences){
        x.second = prawdopodobienstwo;
    }

    preferences.insert(std::pair<IPackageReceiver*, double>(r, prawdopodobienstwo));
}

void ReciverPreferences::remove_receiver(IPackageReceiver* r){
    //funkcja dziala rowniez gdy r nie ma w mapie preferences
    if(preferences.find(r) != preferences.end()){
        preferences.erase(r);

        // funkcja na razie ma nie zajmowac sie odbiorcami z konkretnym prawdopodobienstwem, wiec:
        double prawdopodobienstwo = 1/( double(preferences.size()));

        for(auto &x: preferences){
            x.second = prawdopodobienstwo;
        }
    }
}

IPackageReceiver* ReciverPreferences::choose_receiver(){
    double random = probability_value(); //warosc od 0 do 1
    double dystrybuanta_1 = 0;
    double dystrybuanta_2 = 0;

    for(auto reciver: preferences){
        dystrybuanta_1 = dystrybuanta_2;
        dystrybuanta_2 += reciver.second;

        if(dystrybuanta_1 < random and random < dystrybuanta_2){
            return reciver.first;
        }
    }
    return nullptr; //cos poszlo nie tak
}

