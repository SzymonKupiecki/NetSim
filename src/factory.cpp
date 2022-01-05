#include "factory.hpp"


template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    std::remove(find_by_id(id));//przeniesc do hpp
}

