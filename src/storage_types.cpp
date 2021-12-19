#include "storage_types.hpp"

Package PackageQueue::pop() {
    switch(queue_type_){
        case PackageQueueType::FIFO: {
            Package p = std::move(queue_.front());
            queue_.pop_front();
            return p;
        }
        case PackageQueueType::LIFO: {
            Package p = std::move(queue_.back());
            queue_.pop_back();
            return p;
        }
    }
    throw; // inaczej komilator sie rzuca ze tu returna nie ma a gupi nie wie ze tu sie dojsc nie da XD
}