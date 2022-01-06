#include "factory.hpp"

template<typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto ptr = collection.find_by_id(id);
}