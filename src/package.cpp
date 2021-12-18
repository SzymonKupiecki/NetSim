#include "package.hpp"

// Inicjalizacja statycznych pol klasy
std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package() {
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
        //na wypadek wczesniejszej inicjalizacji np. Package(2) bez Package(1)
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
Package::Package(ElementID id){
    id_ = id;
    assigned_IDs.insert(id);
}

// Dodanie usuwanego ID do zwolnionych ID
Package::~Package() {
    freed_IDs.insert(id_);
}

// zdefiniowanie = jako przerzucenia id_
Package& Package::operator=(Package&& other) noexcept {
    id_ = other.id_;
    return *this;
}