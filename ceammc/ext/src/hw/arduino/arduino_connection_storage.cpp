#include "arduino_connection_storage.h"
#include "ceammc_thread.h"

#include <iostream>

using namespace ceammc::thread;

namespace ceammc {
namespace hw {

    ConnectionStorage* ConnectionStorage::instance_ = 0;
    pthread_mutex_t ConnectionStorage::mutex_;
    bool ConnectionStorage::init_ = ConnectionStorage::initMutex();

    ConnectionStorage::ConnectionStorage() {}

    ConnectionStorage::~ConnectionStorage() {}

    bool ConnectionStorage::initMutex()
    {
        pthread_mutex_init(&mutex_, 0);
        return true;
    }

    ConnectionStorage& ConnectionStorage::instance()
    {
        if (instance_ == 0) {
            Lock lock(mutex_);
            instance_ = new ConnectionStorage();
            return *instance_;
        }

        return *instance_;
    }

    bool ConnectionStorage::used(const std::string& port) const
    {
        Lock lock(mutex_);
        return connections_.find(port) != connections_.end();
    }

    void ConnectionStorage::add(const std::string& port)
    {
        Lock lock(mutex_);
        connections_.insert(port);
        std::cerr << "[arduino_storage] connected to: " << port << "\n";
    }

    void ConnectionStorage::remove(const std::string& port)
    {
        Lock lock(mutex_);
        connections_.erase(port);
        std::cerr << "[arduino_storage] disconnected from: " << port << "\n";
    }

    size_t ConnectionStorage::size() const
    {
        Lock lock(mutex_);
        return connections_.size();
    }
}
}
