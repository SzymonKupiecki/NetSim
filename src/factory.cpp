#include "factory.hpp"

template<typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    Node *node_ptr = &(*(collection.find_by_id(id)));
    ReceiverType object_type = node_ptr->get_receiver_type();
    IPackageReceiver *i_package_ptr; //inaczej sie rzuca ze niby niezdefiniowany jak w srodku if go definiuje
    if (object_type == ReceiverType::WORKER ||object_type == ReceiverType::STOREHOUSE) {
        i_package_ptr = dynamic_cast<IPackageReceiver *>(node_ptr);
    } else { throw; }
    //usuwanie polaczen miedzy dostawcami
    //i_package_ptr jednoznacznie definiuje typ obiektu
    for (auto &worker: workers_) {
        worker.remove_receiver(i_package_ptr);
    }
    for (auto &ramp: ramps_) {
        ramp.remove_receiver(i_package_ptr);
    }
    //usuwanie samego obiektu
    if(object_type == ReceiverType::WORKER){workers_.remove_by_id(id);}
    else{storehouses_.remove_by_id(id);}
}

bool Factory::is_consistent() {
    std::map<PackageSender*,NodeColour> colour;
    //Wszytkie rampy
    for(auto& ramp: ramps_){
        colour[dynamic_cast<PackageSender*>(&ramp)] = NodeColour::UNVISITED;
    }
    //Wszyscy workerzy
    for(auto& worker: workers_){
        colour[dynamic_cast<PackageSender*>(&worker)] = NodeColour::UNVISITED;
    }
    //przechodzenie przez wszytskie rampy do sterehousow
    try{
        for(auto& ramp: ramps_){
            does_receiver_has_reachable_storehouse(dynamic_cast<PackageSender*>(&ramp), colour);
        }
        return true;
    }
    catch (std::string& error_message){
        return false;
    }
}

bool Factory::does_receiver_has_reachable_storehouse(PackageSender* sender,std::map<PackageSender*,NodeColour> colour){
    if(colour[sender] == NodeColour::VERIFIED){return true;}
    //oznacz jako odwiedzony
    colour[sender] = NodeColour::VISITED;
    if(sender->get_preferences().empty()){throw(std::logic_error("Sender doesnt have receivers others than himself"));}
    bool does_sender_has_at_least_one_receiver_other_than_himself = false;
    for(const auto& [receiver, trash]: sender->get_preferences()){
        if(receiver->get_receiver_type() == ReceiverType::STOREHOUSE){
            does_sender_has_at_least_one_receiver_other_than_himself = true;
        }
        else{
            auto receiver_ptr = dynamic_cast<IPackageReceiver*>(receiver);
            auto worker_ptr = static_cast<Worker*>(receiver_ptr);//NORMALNIE POWINNO DZIALAC Z DYNAMIC TYLKO Z PELNA IMPLEMENTACJA
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender){continue;}
            does_sender_has_at_least_one_receiver_other_than_himself = true;
            if(colour[sendrecv_ptr] == NodeColour::UNVISITED){
                does_receiver_has_reachable_storehouse(sendrecv_ptr,colour);
            }
        }
    }
    colour[sender] = NodeColour::VERIFIED;
    if(does_sender_has_at_least_one_receiver_other_than_himself){return true;}
    else{throw(std::logic_error("Sender doesnt have receivers other than himself"));}

}

void Factory::do_deliveres(Time t) {
    for(auto& ramp: ramps_){
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for(auto& ramp: ramps_){
        ramp.send_package();
    }

    for(auto& worker: workers_){
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for(auto& worker: workers_){
        worker.do_work();
    }
}