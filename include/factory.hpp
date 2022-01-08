#ifndef FACTORY_H
#define FACTORY_H
#include "package.hpp"
#include "nodes.hpp"
#include <list>
#include <algorithm>
#include <map>
#include <stdexcept>

enum class NodeColour{
    UNVISITED, VISITED, VERIFIED
};

template<typename Node>
class NodeCollection{
public:
    using const_iterator = typename std::list<Node>::const_iterator;
    using iterator = typename std::list<Node>::iterator;
    NodeCollection() = default;

    [[nodiscard]] typename NodeCollection<Node>::const_iterator cbegin() const {return nodes_.cbegin();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator cend() const {return nodes_.cend();}
    [[nodiscard]] typename NodeCollection<Node>::iterator begin() {return nodes_.begin();}
    [[nodiscard]] typename NodeCollection<Node>::iterator end() {return nodes_.end();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator begin() const {return nodes_.cbegin();}
    [[nodiscard]] typename NodeCollection<Node>::const_iterator end() const {return nodes_.cend();}

    [[nodiscard]] NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(), nodes_.end(), [id](Node& node){return node.get_id() == id;});}
    [[nodiscard]] NodeCollection<Node>::const_iterator find_by_id(ElementID id) const{return std::find_if(nodes_.cbegin(), nodes_.end(), [id](const Node& node){return node.get_id() == id;});}

    void remove_by_id(ElementID id) {if(find_by_id(id) != nodes_.end()){nodes_.erase(find_by_id(id));}}

    void add(Node&& node) {nodes_.emplace_back(std::move(node));}

protected:
    std::list<Node> nodes_;
};

class Factory{
public:
    // RAMPS
    void add_ramp(Ramp&& ramp) {ramps_.add(std::move(ramp));}
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const{return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return ramps_.cbegin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const{return ramps_.cend();}
    // WORKERS
    void add_worker(Worker&& worker) {workers_.add(std::move(worker));}
    void remove_worker(ElementID id) {remove_receiver(workers_, id);}//USUWANIE POLACZEN
    [[nodiscard]] NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const{return workers_.cbegin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const{return workers_.cend();}
    // STOREHOUSES
    void add_storehouse(Storehouse&& ramp) {storehouses_.add(std::move(ramp));}
    void remove_storehouse(ElementID id) {remove_receiver(storehouses_, id);}//USUWANIE POLACZEN
    [[nodiscard]] NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const{return storehouses_.cbegin();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const{return storehouses_.cend();}
    //REST
    bool is_consistent();

private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
    bool does_receiver_has_reachable_storehouse(PackageSender*,std::map<PackageSender*,NodeColour> colour);
    template<typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};
#endif