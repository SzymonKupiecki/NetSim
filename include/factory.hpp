#ifndef FACTORY_H
#define FACTORY_H
#include "package.hpp"
#include "nodes.hpp"
#include <list>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <sstream>
#include <limits>

enum class NodeColour{
    UNVISITED, VISITED, VERIFIED
};

template<typename Node>
class NodeCollection{
public:
    using const_iterator = typename std::list<Node>::const_iterator;
    using iterator = typename std::list<Node>::iterator;

    [[nodiscard]] const_iterator cbegin() const {return nodes_.cbegin();}
    [[nodiscard]] const_iterator cend() const {return nodes_.cend();}
    iterator begin() {return nodes_.begin();}
    iterator end() {return nodes_.end();}
    [[nodiscard]] const_iterator begin() const {return nodes_.cbegin();}
    [[nodiscard]] const_iterator end() const {return nodes_.cend();}

    iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(), nodes_.end(), [id](Node& node){return node.get_id() == id;});}
    [[nodiscard]] const_iterator find_by_id(ElementID id) const{return std::find_if(nodes_.cbegin(), nodes_.end(), [id](const Node& node) {return node.get_id() == id;});}

    void remove_by_id(ElementID id) {if(find_by_id(id) != nodes_.end()){nodes_.erase(find_by_id(id));}}

    void add(Node& node) {nodes_.push_back(std::move(node));}

protected:
    std::list<Node> nodes_;
};

class Factory{
public:
    // RAMPS
    void add_ramp(Ramp&& ramp) {ramps_.add(ramp);}
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const{return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return ramps_.cbegin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const{return ramps_.cend();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_begin() const{return ramps_.begin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_end() const{return ramps_.end();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_begin() {return ramps_.begin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_end() {return ramps_.end();}
    // WORKERS
    void add_worker(Worker&& worker) {workers_.add(worker);}
    void remove_worker(ElementID id) {remove_receiver(workers_, id);}//USUWANIE POLACZEN
    [[nodiscard]] NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const{return workers_.cbegin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const{return workers_.cend();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_begin() const{return workers_.begin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_end() const{return workers_.end();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_begin() {return workers_.begin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_end() {return workers_.end();}
    // STOREHOUSES
    void add_storehouse(Storehouse&& storehouse) {storehouses_.add(storehouse);}
    void remove_storehouse(ElementID id) {remove_receiver(storehouses_, id);}//USUWANIE POLACZEN
    [[nodiscard]] NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const{return storehouses_.cbegin();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const{return storehouses_.cend();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_begin() const{return storehouses_.begin();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_end() const{return storehouses_.end();}
    [[nodiscard]] NodeCollection<Storehouse>::iterator storehouse_begin() {return storehouses_.begin();}
    [[nodiscard]] NodeCollection<Storehouse>::iterator storehouse_end() {return storehouses_.end();}
    //REST
    bool is_consistent();

    void do_deliveres(Time t);

    void do_package_passing();

    void do_work(Time t);


private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
    bool does_receiver_has_reachable_storehouse(PackageSender*,std::map<PackageSender*,NodeColour> colour);
//    template<typename Node>
//    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    void remove_receiver(NodeCollection<Worker>& collection, ElementID id);
    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id);
};

class NoReceiverException: public std::logic_error{
public:
    NoReceiverException(): std::logic_error("Sender doesnt have receivers or have only himself") {}
};

enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ElementType element_type_;
    std::map<std::string, std::string> parameters_;

    ParsedLineData() = delete;

    ParsedLineData(ElementType t, std::map<std::string, std::string> m): element_type_(t), parameters_(std::move(m)) {}

    std::map<std::string, std::string> get_map() {return parameters_;}

    ElementType get_type() {return element_type_;}

//    explicit ParsedLineData(ElementType t): element_type_(t) {}
//
//    explicit ParsedLineData(std::map<std::string, std::string> m): parameters_(std::move(m)) {}
};

ParsedLineData parse_line(std::string line);

Factory load_factory_structure(std::istream& is);

class WrongLineError: public std::logic_error{
public:
    WrongLineError(): std::logic_error("This line doesnt contain correct data") {}
};

bool is_node_or_link_correct(ParsedLineData candidate);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif