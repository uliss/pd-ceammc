#include "arduino_thread.h"
#include "arduino.h"
#include "arduino_connection_tracker.h"
#include "ceammc_platform.h"
#include "serial/serial.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind/bind.hpp>
#include <pthread.h>

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace ceammc::platform;
using namespace boost::placeholders;

#ifndef NDEBUG
#define DEBUG
#endif

namespace ceammc {
namespace hw {

    constexpr const int DEFAULT_TIMEOUT = 10;
    static std::chrono::milliseconds RECONNECT_TIME_MS(1000);

    struct usb_info {
        int vid;
        int pid;
    };

    class ThreadTracker {
        Arduino* a_;

    public:
        ThreadTracker(Arduino* a)
            : a_(a)
        {
#ifdef DEBUG
            std::cerr << "[arduino_thread] " << pthread_self() << " started\n";
#endif
        }

        ~ThreadTracker()
        {
#ifdef DEBUG
            std::cerr << "[arduino_thread] " << pthread_self() << " finished\n";
#endif
            a_->threadFinished();
        }
    };

    static bool hasVendorAndProduct(const serial::PortInfo& info, int vendorId, int productId)
    {
        int vid = 0, pid = 0;
        std::string str;
        if (!ArduinoThread::parseArduinoId(info.hardware_id, vid, pid, str))
            return false;

        return (vid == vendorId && pid == productId);
    }

    static bool hasVendor(const serial::PortInfo& info, int vendorId)
    {
        int vid = 0, pid = 0;
        std::string str;
        if (!ArduinoThread::parseArduinoId(info.hardware_id, vid, pid, str))
            return false;

        return vid == vendorId;
    }

    static bool hasSerial(const serial::PortInfo& info, const std::string& serialNo)
    {
        int vid = 0, pid = 0;
        std::string str;
        if (!ArduinoThread::parseArduinoId(info.hardware_id, vid, pid, str))
            return false;

        return str == serialNo;
    }

    serial::PortInfo ArduinoThread::findDeviceById(const PortList& lst, int vendorId, int productId)
    {
        auto it = std::find_if(lst.begin(), lst.end(),
            boost::bind(hasVendorAndProduct, _1, vendorId, productId));

        return it == lst.end() ? serial::PortInfo() : *it;
    }

    serial::PortInfo ArduinoThread::findDeviceByVendorId(const PortList& lst, int id)
    {
        auto it = std::find_if(lst.begin(), lst.end(),
            boost::bind(hasVendor, _1, id));

        return it == lst.end() ? serial::PortInfo() : *it;
    }

    serial::PortInfo ArduinoThread::findDeviceBySerialNo(const PortList& lst, const std::string& serialNo)
    {
        auto it = std::find_if(lst.begin(), lst.end(),
            boost::bind(hasSerial, _1, serialNo));

        return it == lst.end() ? serial::PortInfo() : *it;
    }

    serial::PortInfo ArduinoThread::findDeviceByPort(const PortList& lst, const std::string& port)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (lst[i].port == port)
                return lst[i];
        }

        return serial::PortInfo();
    }

    PortList ArduinoThread::findAllArduinos()
    {
        PortList res;
        PortList lst = serial::list_ports();

        for (size_t i = 0; i < lst.size(); i++) {
            if (maybeArduino(lst[i]))
                res.push_back(lst[i]);
        }

        return res;
    }

    PortList ArduinoThread::findAllDevices()
    {
        return serial::list_ports();
    }

    serial::PortInfo ArduinoThread::findFreeArduino(const PortList& lst)
    {
        PortList l = lst;

        for (size_t i = 0; i < l.size(); i++) {
            // skip connected devices
            if (ConnectionTracker::isConnected(l[i].port))
                continue;

            return l[i];
        }

        return serial::PortInfo();
    }

    template <class T, class F>
    serial::PortInfo arduinoWait(Arduino* arduino, T pred, F lsfn, const std::string& descr = "")
    {
        assert(arduino);

        auto dev = pred(lsfn());

        while (dev.port.empty()) {
            if (arduino->shouldStopThread())
                break;

            if (!arduino->reconnect())
                break;

            std::this_thread::sleep_for(RECONNECT_TIME_MS);
            arduino->pushDebug("waiting for Arduino: " + descr);
            dev = pred(lsfn());
        }

        return dev;
    }

    serial::PortInfo ArduinoThread::waitForArduinoPort(Arduino* arduino, const std::string& port)
    {
        std::string descr("port=");
        descr += port;

        return arduinoWait(arduino,
            boost::bind(findDeviceByPort, _1, port),
            &serial::list_ports,
            descr);
    }

    serial::PortInfo ArduinoThread::waitForArduinoUsbSerial(Arduino* arduino, const std::string& id)
    {
        std::string descr("serial=");
        descr += id;

        return arduinoWait(arduino,
            boost::bind(findDeviceBySerialNo, _1, id),
            &serial::list_ports,
            descr);
    }

    serial::PortInfo ArduinoThread::waitForArduinoVendorId(Arduino* arduino, int id)
    {
        std::ostringstream s;
        s << "vendor_id=" << id;

        return arduinoWait(arduino,
            boost::bind(findDeviceByVendorId, _1, id),
            &serial::list_ports,
            s.str());
    }

    serial::PortInfo ArduinoThread::waitForArduinoId(Arduino* arduino, int vendorId, int productId)
    {
        std::ostringstream s;
        s << "vendor_id=" << vendorId << " and product_id=" << productId;

        return arduinoWait(arduino,
            boost::bind(findDeviceById, _1, vendorId, productId),
            &serial::list_ports,
            s.str());
    }

    serial::PortInfo ArduinoThread::waitForFreeArduino(Arduino* arduino)
    {
        return arduinoWait(arduino,
            boost::bind(findFreeArduino, _1),
            findAllArduinos, "wait for free arduino");
    }

    serial::PortInfo ArduinoThread::waitForArduino(Arduino* arduino)
    {
        std::string port = arduino->port();
        std::string serial = arduino->usbSerial();
        int vid = arduino->vendorId();
        int pid = arduino->productId();

        // if port specified
        if (!port.empty()) {
            serial::PortInfo res = waitForArduinoPort(arduino, port);

            if (res.port.empty())
                arduino->pushError("can't connect to device: " + port);

            return res;
        }
        // if usb serial specified
        else if (!serial.empty()) {
            serial::PortInfo res = waitForArduinoUsbSerial(arduino, serial);

            if (res.port.empty())
                arduino->pushError("can't connect to device with serial: " + serial);

            return res;
        }
        // vid or pid specified
        else if (vid > 0) {
            if (pid > 0)
                return waitForArduinoId(arduino, vid, pid);
            else
                return waitForArduinoVendorId(arduino, vid);
        }
        // wait for free arduino
        else {
            serial::PortInfo res = waitForFreeArduino(arduino);

            if (res.port.empty())
                arduino->pushError("no available arduino devices");

            return res;
        }
    }

    bool ArduinoThread::parseArduinoId(const std::string& str, int& vid, int& pid, std::string& serialNo)
    {
        // format is fixed: "USB VID:PID=%04x:%04x SNR=%s"
        unsigned int vid_, pid_;
        char buf[100];
        if (sscanf(str.c_str(), "USB VID:PID=%04x:%04x SNR=%99s", &vid_, &pid_, buf) != 3)
            return false;

        vid = vid_;
        pid = pid_;
        serialNo = buf;
        return true;
    }

    void* ArduinoThread::worker(void* v)
    {
        using namespace serial;

        Arduino* arduino = static_cast<Arduino*>(v);
        ThreadTracker logger(arduino);

        try {
            // reconnection loop
            do {
                Serial serial;

                // connect immidiately or wait
                serial::PortInfo port_info = waitForArduino(arduino);

                // check if already connected
                if (ConnectionTracker::isConnected(port_info.port)) {
                    arduino->pushError("already connected to device: " + port_info.port);
                    return 0;
                }

                int baudRate = arduino->baudRate();
                serial.setBaudrate(baudRate);
                serial.setPort(port_info.port);
                serial.open();

                // can't open
                if (!serial.isOpen()) {
#ifdef DEBUG
                    std::cerr << "[arduino_thread] can't connect to device: "
                              << port_info.port << " at baudrate " << baudRate << "\n";

#endif
                    if (arduino->reconnect()) {
#ifdef DEBUG
                        std::cerr << "[arduino_thread] reconnecting...\n";
#endif
                        std::this_thread::sleep_for(RECONNECT_TIME_MS);
                        // try again
                        continue;
                    } else
                        return 0;
                }

#ifdef DEBUG
                // connection is ok
                std::cerr << "[arduino_thread] connected to device: "
                          << port_info.port << " with baudrate: " << baudRate << "\n";
#endif

                serial.setDTR(false);
                serial.setStopbits(stopbits_one);
                serial.setTimeout(Timeout::max(), DEFAULT_TIMEOUT, 0, DEFAULT_TIMEOUT, 0);

                // register connection in constructor
                // unregister connection when destructor is called
                // RAII idiom
                ConnectionTracker tracker(arduino, port_info);

                // main read/write cycle
                try {
                    while (!arduino->shouldStopThread()) {
                        read(arduino, serial);
                        write(arduino, serial);
                    }
                } catch (std::exception& e) {
                    arduino->pushError("exception: " + std::string(e.what()));

                    // reconnect on error
                    if (arduino->reconnect()) {
#ifdef DEBUG
                        std::cerr << "[arduino_thread] reconnecting...\n";
#endif
                        std::this_thread::sleep_for(RECONNECT_TIME_MS);
                        continue;
                    } else
                        return 0;
                }

                // normal return
                return 0;

            } while (true);

        } catch (std::exception& e) {
#ifdef DEBUG
            std::cerr << "[arduino_thread] exception: " << e.what() << "\n";
#endif
            return 0;
        }
#if defined(__APPLE__) && defined(__arm64__)
        catch (...) {
            return 0;
        }
#endif

        return 0;
    }

    void ArduinoThread::read(Arduino* arduino, serial::Serial& port)
    {
        uint8_t chr = 0;

        // read next byte from serial port
        if (port.read(&chr, 1) != 1)
            return;

        arduino->pushInputData(chr);
    }

    void ArduinoThread::write(Arduino* arduino, serial::Serial& port)
    {
        if (!arduino->hasOutputData())
            return;

        size_t n_bytes = port.write(arduino->outputData());
        arduino->popWrittenData(n_bytes);
    }

    bool ArduinoThread::maybeArduino(const serial::PortInfo& p)
    {
        static usb_info const info[] = {
            // CP2102 USB to UART Bridge Controller
            {
                0x10c4,
                /*(Silicon Laboratories, Inc.)*/
                0xea60,
            }
        };

        using namespace boost::algorithm;

        if (contains(to_lower_copy(p.description), "arduino"))
            return true;

        int vid = 0, pid = 0;
        std::string serial;
        if (!parseArduinoId(p.hardware_id, vid, pid, serial))
            return false;

        for (const usb_info& i : info) {
            if (i.vid == vid && i.pid == pid)
                return true;
        }

        return false;
    }
}
}
