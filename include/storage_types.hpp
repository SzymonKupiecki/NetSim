#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H
#include <list>
#include <utility>
#include "package.hpp"
enum class PackageQueueType{
    LIFO, FIFO
};

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    IPackageStockpile() = default;

    void push(Package&& p) {queue_.emplace_back(std::move(p));}
    [[nodiscard]] bool empty() const {return queue_.empty();}
    [[nodiscard]] std::size_t size() const {return queue_.size();}

    [[nodiscard]] typename IPackageStockpile::const_iterator cbegin() const {return queue_.cbegin();}
    [[nodiscard]] typename IPackageStockpile::const_iterator cend() const {return queue_.cend();}
    [[nodiscard]] typename IPackageStockpile::const_iterator begin() const {return queue_.cbegin();}
    [[nodiscard]] typename IPackageStockpile::const_iterator end() const {return queue_.cend();}

    virtual ~IPackageStockpile() = default;

protected:
    std::list<Package> queue_;
};

class IPackageQueue : public IPackageStockpile{
public:
    explicit IPackageQueue(PackageQueueType type) : IPackageStockpile(), queue_type_(type) {}

    PackageQueueType get_queue_type() {return queue_type_;}
    Package pop();

protected:
    PackageQueueType queue_type_;
};

class PackageQueue: public IPackageQueue{
public:
    explicit PackageQueue(PackageQueueType type) : IPackageQueue(type) {}
};

#endif