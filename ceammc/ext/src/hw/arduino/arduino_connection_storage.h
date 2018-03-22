#ifndef ARDUINO_CONNECTION_STORAGE_H
#define ARDUINO_CONNECTION_STORAGE_H

#include <set>
#include <string>

#include <pthread.h>

namespace ceammc {
namespace hw {

    class ConnectionStorage {
        std::set<std::string> connections_;
        ConnectionStorage();
        ~ConnectionStorage();
        ConnectionStorage(const ConnectionStorage&);
        void operator=(const ConnectionStorage&);
        static pthread_mutex_t mutex_;
        static ConnectionStorage* instance_;
        static bool init_;
        static bool initMutex();

    public:
        /**
         * Returns singleton instance
         */
        static ConnectionStorage& instance();

        /**
         * Check if already connected to serial port
         */
        bool used(const std::string& port) const;

        /**
         * Add connection
         */
        void add(const std::string& port);

        /**
         * Remove connection
         */
        void remove(const std::string& port);

        /**
         * Returns number of connections
         */
        size_t size() const;
    };
}
}

#endif // ARDUINO_CONNECTION_STORAGE_H
