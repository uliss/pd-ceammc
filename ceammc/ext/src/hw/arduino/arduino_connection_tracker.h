#ifndef ARDUINO_CONNECTION_TRACKER_H
#define ARDUINO_CONNECTION_TRACKER_H

#include <string>

namespace serial {
class PortInfo;
}

namespace ceammc {
namespace hw {

    class Arduino;

    /**
     * @brief The ConnectionTracker class - RAII idiom
     */
    class ConnectionTracker {
        Arduino* a_;

    public:
        ConnectionTracker(Arduino* a, const serial::PortInfo& info);
        ~ConnectionTracker();

        static bool isConnected(const std::string& port);
    };
}
}

#endif // ARDUINO_CONNECTION_TRACKER_H
