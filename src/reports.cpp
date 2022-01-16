#include "reports.hpp"
#include "simulation.hpp"



bool IntervalReportNotifier::should_generate_report(Time t){
    if( ((t-1) % time_offset_) == 0){
        return true;
    }
    return false;
}

bool SpecificTurnsReportNotifie::should_generate_report(Time t){
    if(turns_.count(t)){
        return true;
    }
    return false;
}

void generate_structure_report(Factory &f, std::ostream &os) {
    os << '\n';
    os << "== LOADING RAMPS ==" << '\n';
    for (auto it = f.ramp_begin(); it != f.ramp_end(); it++) {
        os << '\n' << "LOADING RAMP #" << std::to_string(it->get_id()) << '\n';
        os << ' ' << ' ' << "Delivery interval: " << std::to_string(it->get_delivery_interval()) << '\n';
        os << ' ' << ' ' << "Receivers:" << '\n';
        std::vector<IPackageReceiver*> vec;
        ElementID count = 1;
        while(vec.size() != it->receiver_preferences_.get_preferences().size())
        {
            for(auto i : it->receiver_preferences_.get_preferences()){
                if(i.first->get_id() == count){
                    vec.push_back(i.first);
                    count++;
                }
            }
        }
        for (auto i : vec) {
            if (i->get_receiver_type() == ReceiverType::WORKER){
                os << ' ' << ' ' << ' ' << ' ' << "worker #" << std::to_string(i->get_id()) << '\n';
            }
            else if (i->get_receiver_type() == ReceiverType::STOREHOUSE){
                os << ' ' << ' ' << ' ' << ' ' << "storehouse  #" << std::to_string(i->get_id()) << '\n';
            }
        }
    }

    os << '\n' << '\n' << "== WORKERS ==" << '\n';
    for (auto it = f.worker_begin(); it != f.worker_end(); it++) {
        os << '\n' << "WORKER #" << std::to_string(it->get_id()) << '\n';
        os << ' ' << ' ' << "Processing time: " << std::to_string(it->get_processing_duration()) << '\n';
        os << ' ' << ' ' << "Queue type: ";
        if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO){os << "FIFO" << '\n';}
        else if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO){os << "LIFO" << '\n';}
        os << ' ' << ' ' << "Receivers:" << '\n';
        std::vector<IPackageReceiver*> vec;
        ElementID count = 1;
        while(vec.size() != it->receiver_preferences_.get_preferences().size())
        {
            for(auto i : it->receiver_preferences_.get_preferences()){
                if(i.first->get_id() == count){
                    vec.push_back(i.first);
                    count++;
                }
            }
        }
        for (auto i : vec) {
            if (i->get_receiver_type() == ReceiverType::WORKER){
                os << ' ' << ' ' << ' ' << ' ' << "worker #" << std::to_string(i->get_id()) << '\n';
            }
            else if (i->get_receiver_type() == ReceiverType::STOREHOUSE){
                os << ' ' << ' ' << ' ' << ' ' << "storehouse #" << std::to_string(i->get_id()) << '\n';
            }
        }
    }

    os << '\n' << '\n';
    if (f.storehouse_begin() != f.storehouse_end()) {
        os << "== STOREHOUSES ==" << '\n';
        for (auto it = f.storehouse_begin(); it != f.storehouse_end(); it++) {
            os << '\n' << "STOREHOUSE #" << std::to_string(it->get_id()) << '\n';
        }
    }
    os << '\n';
}

void generate_simulation_turn_report(Factory &f, std::ostream &os, Time t) {
    os << "=== [ Turn: " << std::to_string(t) << " ] ===" << '\n';
    os << '\n';
    os << "== WORKERS ==" << '\n';
    for (auto it = f.worker_begin(); it != f.worker_end(); it++) {
        os << '\n' << "WORKER #" << std::to_string(it->get_id()) << '\n';
        if (it->get_processing_buffer() != std::nullopt) {
            os << "  PBuffer: #" << it->get_processing_buffer()->get_id() << " ";
            os << "(pt = " << it->get_package_processing_start_time() << ")" << '\n';
        } else
            os << "  PBuffer: (empty)" << '\n';
        if (!(it->get_queue()->empty())) {
            os << "  Queue: ";
            for (const auto &el: *it->get_queue()) {
                if (it->get_queue()->size() == 1)
                    os << "#" << el.get_id();
                else
                    os << ", #" << el.get_id();
            }
            os << "\n";
        } else
            os << "  Queue: (empty)" << '\n';
        if (it->get_sending_buffer() != std::nullopt) {
            os << "  SBuffer: #" << it->get_sending_buffer()->get_id() << "\n";
        } else
            os << "  SBuffer: (empty)" << '\n';
    }
    os << '\n' << '\n';

    os << "== STOREHOUSES ==" << '\n';
    for (auto it = f.storehouse_begin(); it != f.storehouse_end(); it++) {
        os << '\n' << "STOREHOUSE #" << std::to_string(it->get_id()) << '\n';
        if (!(it->get_queue()->empty())) {
            os << "  Stock: ";
            for (const auto &el: *it->get_queue()) {
                if (it->get_queue()->size() == 1)
                    os << "#" << el.get_id() << '\n';
                else
                    os << ", #" << el.get_id() << '\n';
            }
        } else
            os << "  Stock: (empty)" << '\n';
    }
    os << '\n';
}