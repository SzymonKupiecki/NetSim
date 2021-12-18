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
        // w przypadku pierwszej inicjalizacji albo na wypadek inicjalizacji np. Package(2) bez Package(1)
        if(assigned_IDs.empty() or *assigned_IDs.begin() != 1){id_ = 1;}
        else{id_ = *assigned_IDs.end() + 1;}
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