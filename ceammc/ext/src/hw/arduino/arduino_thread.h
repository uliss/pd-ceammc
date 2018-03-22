#ifndef ARDUINO_THREAD_H
#define ARDUINO_THREAD_H

#include <string>
#include <vector>

namespace serial {
class Serial;
class PortInfo;
}

namespace ceammc {
namespace hw {

    typedef std::vector<serial::PortInfo> PortList;

    class Arduino;

    class ArduinoThread {
    public:
        static void* worker(void* v);
        static void read(Arduino* arduino, serial::Serial& port);
        static void write(Arduino* arduino, serial::Serial& port);

        static bool maybeArduino(const serial::PortInfo& p);

        static PortList findAllArduinos();
        static PortList findAllDevices();
        static serial::PortInfo findDeviceByPort(const PortList& lst, const std::string& port);
        static serial::PortInfo findDeviceById(const PortList& lst, int vendorId, int productId);
        static serial::PortInfo findDeviceByVendorId(const PortList& lst, int id);
        static serial::PortInfo findDeviceBySerialNo(const PortList& lst, const std::string& serialNo);
        static serial::PortInfo findFreeArduino(const PortList& lst);

        static serial::PortInfo waitForArduinoPort(Arduino *arduino, const std::string& port);
        static serial::PortInfo waitForArduinoUsbSerial(Arduino* arduino, const std::string& id);
        static serial::PortInfo waitForArduinoVendorId(Arduino *arduino, int id);
        static serial::PortInfo waitForArduinoId(Arduino* arduino, int vendorId, int productId);
        static serial::PortInfo waitForFreeArduino(Arduino *arduino);

        static serial::PortInfo waitForArduino(Arduino *arduino);

        static bool parseArduinoId(const std::string& str, int& vid, int& pid, std::string& serialNo);
    };

    class ArduinoConnected {
        std::string name_;

    public:
        ArduinoConnected(const std::string& name);
        ~ArduinoConnected();

    public:
        static bool connect(const std::string& name);
        static bool disconnect(const std::string& name);
    };
}
}

#endif // ARDUINO_THREAD_H
