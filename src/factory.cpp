#include <sstream>
#include "factory.hpp"

//template<typename Node>
//void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
//    auto iterator = collection.find_by_id(id);
//    if(iterator == collection.end()){return;}
//    Node *node_ptr = &(*(collection.find_by_id(id)));
//    ReceiverType object_type = node_ptr->get_receiver_type();
//    IPackageReceiver *i_package_ptr; //inaczej sie rzuca ze niby niezdefiniowany jak w srodku if go definiuje
//    if (object_type == ReceiverType::WORKER ||object_type == ReceiverType::STOREHOUSE) {
//        i_package_ptr = dynamic_cast<IPackageReceiver *>(node_ptr);
//    } else { throw; }
//    //usuwanie polaczen miedzy dostawcami
//    //i_package_ptr jednoznacznie definiuje typ obiektu
//    for (auto &worker: workers_) {
//        worker.receiver_preferences_.remove_receiver(i_package_ptr);
//    }
//    for (auto &ramp: ramps_) {
//        ramp.receiver_preferences_.remove_receiver(i_package_ptr);
//    }
//    //usuwanie samego obiektu
//    if(object_type == ReceiverType::WORKER){workers_.remove_by_id(id);}
//    else{storehouses_.remove_by_id(id);}
//}
void Factory::remove_receiver(NodeCollection<Worker>& collection, ElementID id) {
    auto iterator = collection.find_by_id(id);
    if(iterator == collection.end()){return;}
    Worker *node_ptr = &(*(iterator));
    ReceiverType object_type = node_ptr->get_receiver_type();
    IPackageReceiver *i_package_ptr; //inaczej sie rzuca ze niby niezdefiniowany jak w srodku if go definiuje
    if (object_type == ReceiverType::WORKER ||object_type == ReceiverType::STOREHOUSE) {
        i_package_ptr = dynamic_cast<IPackageReceiver *>(node_ptr);
    } else { throw; }
    //usuwanie polaczen miedzy dostawcami
    //i_package_ptr jednoznacznie definiuje typ obiektu
    for (auto &worker: workers_) {
        worker.receiver_preferences_.remove_receiver(i_package_ptr);
    }
    for (auto &ramp: ramps_) {
        ramp.receiver_preferences_.remove_receiver(i_package_ptr);
    }
    //usuwanie samego obiektu
    if(object_type == ReceiverType::WORKER){workers_.remove_by_id(id);}
    else{storehouses_.remove_by_id(id);}
}
void Factory::remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {
    auto iterator = collection.find_by_id(id);
    if(iterator == collection.end()){return;}
    Storehouse *node_ptr = &(*(collection.find_by_id(id)));
    ReceiverType object_type = node_ptr->get_receiver_type();
    IPackageReceiver *i_package_ptr; //inaczej sie rzuca ze niby niezdefiniowany jak w srodku if go definiuje
    if (object_type == ReceiverType::WORKER ||object_type == ReceiverType::STOREHOUSE) {
        i_package_ptr = dynamic_cast<IPackageReceiver *>(node_ptr);
    } else { throw; }
    //usuwanie polaczen miedzy dostawcami
    //i_package_ptr jednoznacznie definiuje typ obiektu
    for (auto &worker: workers_) {
        worker.receiver_preferences_.remove_receiver(i_package_ptr);
    }
    for (auto &ramp: ramps_) {
        ramp.receiver_preferences_.remove_receiver(i_package_ptr);
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
    catch (NoReceiverException& error_message){
        return false;
    }
}

bool Factory::does_receiver_has_reachable_storehouse(PackageSender* sender,std::map<PackageSender*,NodeColour> colour){
    if(colour[sender] == NodeColour::VERIFIED){return true;}
    //oznacz jako odwiedzony
    colour[sender] = NodeColour::VISITED;
    if(sender->receiver_preferences_.get_preferences().empty()){throw NoReceiverException();}
    bool does_sender_has_at_least_one_receiver_other_than_himself = false;
    for(const auto& [receiver, trash]: sender->receiver_preferences_.get_preferences()){
        if(receiver->get_receiver_type() == ReceiverType::STOREHOUSE){
            does_sender_has_at_least_one_receiver_other_than_himself = true;
        }
        else{
            auto receiver_ptr = dynamic_cast<IPackageReceiver*>(receiver);
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
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
    else{throw NoReceiverException();}

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
        worker.do_work(t);
    }
}

ParsedLineData parse_line(std::string line){
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while(std::getline(token_stream, token, delimiter)){
        tokens.push_back(token);
    }

    //detecting type specifier
    ElementType t;
    if(tokens.empty()){throw WrongLineError();}
    if(tokens[0] == "LOADING_RAMP"){t = ElementType::RAMP;}
    else if(tokens[0] == "WORKER"){t = ElementType::WORKER;}
    else if(tokens[0] == "STOREHOUSE"){t = ElementType::STOREHOUSE;}
    else if(tokens[0] == "LINK"){t = ElementType::LINK;}
    else{throw WrongLineError();}
    tokens.erase(tokens.begin());

    std::map<std::string, std::string> map;
    for(auto& pair: tokens){
        std::vector<std::string> bufor;
        std::istringstream stream_pair(pair);
        std::string key;
        while(std::getline(stream_pair, key, '=')){
            bufor.push_back(key);
        }
        map[bufor[0]] = bufor[1];
    }

    return {t, map};
}

Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;
    std::vector<ParsedLineData> nodes_and_links;
    while(std::getline(is, line)){
        try{
            nodes_and_links.push_back(parse_line(line));
        }
        catch(WrongLineError& error_message){
            continue;
        }
    }
    //ADDING NODES AND EXTRACTING LINKS
    std::vector<ParsedLineData> links;
    for(auto& elem: nodes_and_links){
        switch (elem.get_type()) {
            case ElementType::RAMP: {
                if(not is_node_or_link_correct(elem)){break;}
                auto id = (unsigned int)std::stoi(elem.get_map()["id"]);
                auto it = (unsigned int)std::stoi(elem.get_map()["delivery-interval"]);
                factory.add_ramp(Ramp(id, it));
                break;
            }
            case ElementType::WORKER: {
                if(not is_node_or_link_correct(elem)){break;}
                auto map = elem.get_map();
                auto id = (unsigned int)std::stoi(map["id"]);
                auto pt = (unsigned int)std::stoi(map["processing-time"]);
                if (map.size() == 2) {
                    factory.add_worker(Worker(id, pt));
                }
                else{//map.size() == 3
                    PackageQueueType t;
                    if(map["queue-type"] == "FIFO"){t = PackageQueueType::FIFO;}
                    else{t = PackageQueueType::LIFO;}
                    factory.add_worker(Worker(id, pt, std::make_unique<IPackageQueue>(t)));
                }
                break;
            }
            case ElementType::STOREHOUSE: {
                if (not is_node_or_link_correct(elem)) { break; }
                auto map = elem.get_map();
                auto id = (unsigned int) std::stoi(map["id"]);
                factory.add_storehouse(Storehouse(id));
                break;
            }
            case ElementType::LINK: {
                if (is_node_or_link_correct(elem)) {
                    links.push_back(elem);
                }
                break;
            }
        }
    }
    //MAKING LINKS BETWEEN NODES
    for(auto& link: links){
        auto map = link.get_map();
        std::istringstream src(map["src"]);
        std::istringstream dest(map["dest"]);
        std::pair<std::string, unsigned int > src_id, dest_id;
        std::string part;
        while(std::getline(src,part,'-')){
            if(part == "ramp" || part == "worker"){
                src_id.first = part;
            }
            else{
                src_id.second = (unsigned int)std::stoi(part);
            }
        }
        while(std::getline(dest,part,'-')){
            if(part == "worker" || part == "store"){
                dest_id.first = part;
            }
            else{
                dest_id.second = (unsigned int)std::stoi(part);
            }
        }
        if(src_id.first == "ramp" && dest_id.first == "worker"){
            auto source_it = factory.find_ramp_by_id(src_id.second);
            auto dest_it = factory.find_worker_by_id(dest_id.second);
            source_it->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver*>(&(*dest_it)));
        }
        if(src_id.first == "ramp" && dest_id.first == "store"){
            auto source_it = factory.find_ramp_by_id(src_id.second);
            auto dest_it = factory.find_storehouse_by_id(dest_id.second);
            source_it->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver*>(&(*dest_it)));
        }
        if(src_id.first == "worker" && dest_id.first == "worker"){
            auto source_it = factory.find_worker_by_id(src_id.second);
            auto dest_it = factory.find_worker_by_id(dest_id.second);
            source_it->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver*>(&(*dest_it)));
        }
        if(src_id.first == "worker" && dest_id.first == "store"){
            auto source_it = factory.find_worker_by_id(src_id.second);
            auto dest_it = factory.find_storehouse_by_id(dest_id.second);
            source_it->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver*>(&(*dest_it)));
        }

    }
    return factory;
}

bool is_node_or_link_correct(ParsedLineData candidate){
    auto map = candidate.get_map();
    switch (candidate.get_type()) {
        case ElementType::RAMP: {
            if(map.size() != 2){return false;}
            bool have_id = false;
            bool have_interval = false;
            for (auto&[key, value]: map) {
                if (key != "id" && key != "delivery-interval") { return false; }
                if(key == "id"){have_id = true;}
                if(key == "delivery-interval"){have_interval = true;}
                try {
                    ElementID v = (unsigned int) std::stoi(value);
                    if (v < UINT_MAX) { v++; } //#unused variable vibes
                }
                catch (std::invalid_argument &error_message) {
                    return false;
                }
            }
            if(have_interval && have_id){return true;}
            return false;
        }
        case ElementType::WORKER: {
            bool have_id = false;
            bool have_time = false;
            bool have_type = false;
            for (auto&[key, value]: map) {
                if (key != "id" && key != "processing-time" && key != "queue-type") { return false; }
                if (key == "id") { have_id = true; }
                if (key == "processing-time") { have_time = true; }
                if (key == "queue-type") { have_type = true; }
                if (key == "id" || key == "processing-time") {
                    try {
                        ElementID v = (unsigned int) std::stoi(value);
                        if (v < UINT_MAX) { v++; } //#unused variable vibes
                    }
                    catch (std::invalid_argument &error_message) {
                        return false;
                    }
                } else {
                    //key == queue-type
                    if (value != "LIFO" && value != "FIFO") { return false; }
                }
            }
            if (have_type) {
                if (have_time && have_id && map.size() == 3) { return true; }
            } else {
                if (have_time && have_id && map.size() == 2) { return true; }
            }
            return false;
        }
        case ElementType::STOREHOUSE: {
            if(map.size() != 1){return false;}
            for (auto&[key, value]: map) {
                if (key != "id") { return false; }
                try {
                    auto v = (unsigned int) std::stoi(value);
                    if (v < UINT_MAX) { v++; } //#unused variable vibes
                }
                catch (std::invalid_argument &error_message) {
                    return false;
                }
            }
            return true;
        }
        case ElementType::LINK: {
            bool have_src = false;
            bool have_dest = false;
            if (map.size() != 2) { return false; }
            for (auto&[key, value]: map) {
                if (key != "src" && key != "dest") { return false; }
                if (key == "src") { have_src = true; }
                if (key == "dest") { have_dest = true; }
                std::string value_decomposition;
                std::istringstream sth(value);
                bool first_segment_correct = false;
                while (std::getline(sth, value_decomposition, '-')) {
                    if (not first_segment_correct) {
                        if (value_decomposition != "ramp" && value_decomposition != "worker" &&
                            value_decomposition != "store"){
                            first_segment_correct = true;
                        }
                    } else {
                        try {
                            auto v = (unsigned int) std::stoi(value_decomposition);
                            if (v < UINT_MAX) { v++; } //#unused variable vibes
                        }
                        catch (std::invalid_argument &error_message) {
                            return false;
                        }
                    }
                }
            }
            if (have_dest && have_src) { return true; }
            return false;
        }
    }
    throw;
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    if (factory.ramp_begin() != factory.ramp_end()) {
        os << "; == LOADING_RAMPS ==" << '\n' << '\n';
        for (auto it = factory.ramp_begin(); it != factory.ramp_end(); it++) {
            os << "LOADING_RAMP " << "id=" << std::to_string(it->get_id()) << " delivery-interval="
               << std::to_string(it->get_delivery_interval()) << '\n';
        }
    }
    os << '\n';
    if (factory.worker_begin() != factory.worker_end()) {
        os << "; == WORKERS ==" << '\n' << '\n';
        for (auto it = factory.worker_begin(); it != factory.worker_end(); it++) {
            os << "WORKER " << "id=" << std::to_string(it->get_id()) << " processing-time="
            << std::to_string(it->get_processing_duration()) << " queue-type=";
            if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO){os << "FIFO";}
            else {os << "LIFO";}
            os << '\n';
        }
    }
    os << '\n';
    if (factory.storehouse_begin() != factory.storehouse_end()) {
        os << "; == STOREHOUSES ==" << '\n' << '\n';
        for (auto it = factory.storehouse_begin(); it != factory.storehouse_end(); it++) {
            os << "STOREHOUSE " << "id=" << std::to_string(it->get_id()) << '\n';
        }
    }
    os << '\n';
    os << "; == LINKS ==" << '\n' << '\n';
    if (factory.ramp_begin() != factory.ramp_end()) {
        for (auto it = factory.ramp_begin(); it != factory.ramp_end(); it++) {
            auto receivers = it->receiver_preferences_.get_preferences();
            for(auto& [receiver, probality]: receivers){
                os << "LINK " << "src=ramp-" << it->get_id() << " dest=";
                if(receiver->get_receiver_type() == ReceiverType::WORKER) {os << "worker-" << receiver->get_id();}
                else {os << "store-" << receiver->get_id();}
                os << "\n";
            }
            os << "\n";
        }
    }
    if (factory.worker_begin() != factory.worker_end()) {
        for (auto it = factory.worker_begin(); it != factory.worker_end(); it++) {
            auto receivers = it->receiver_preferences_.get_preferences();
            for(auto& [receiver, probality]: receivers){
                os << "LINK " << "src=worker-" << it->get_id() << " dest=";
                if(receiver->get_receiver_type() == ReceiverType::WORKER){os << "worker-" << receiver->get_id();}
                else {os << "store-" << receiver->get_id();}
                os << "\n";
            }
            os << '\n';
        }
    }
}
