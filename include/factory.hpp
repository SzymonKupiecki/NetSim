#ifndef FACTORY_H
#define FACTORY_H
#include "package.hpp"
#include <list>
#include <algorithm>

template<typename Node>
class NodeCollection{
    using const_iterator = typename std::list<Node>::const_iterator;
    using iterator = typename std::list<Node>::iterator;
public:
    NodeCollection() = default;

    [[nodiscard]] typename NodeCollection<Node>::const_iterator cbegin() const {return nodes_.cbegin();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator cend() const {return nodes_.cend();}
    [[nodiscard]] typename NodeCollection<Node>::iterator begin() {return nodes_.begin();}
    [[nodiscard]] typename NodeCollection<Node>::iterator end() {return nodes_.end();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator begin() const {return nodes_.cbegin();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator end() const {return nodes_.cend();}

    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(), nodes_.end(), [id](Node node){return node.get_id == id;});}
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const{return std::find_if(nodes_.cbegin(), nodes_.end(), [id](Node node){return node.get_id == id;});}

    void remove_by_id(ElementID id);

    void add(Node&& node);

protected:
    std::list<Node> nodes_;
};
#endif