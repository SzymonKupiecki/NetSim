#ifndef PACKAGE_H
#define PACKAGE_H
#include <set>
#include <utility>
#include "types.hpp"

class Package{
public:
    Package();
    explicit Package(ElementID id);
    Package(Package&& arg)  noexcept : id_(arg.id_) {}

    Package& operator=(Package&& other) noexcept;

    ~Package();

    ElementID get_id() const {return id_;}
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
#endif