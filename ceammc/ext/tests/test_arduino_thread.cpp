#include "../hw/arduino/arduino.h"
#include "../hw/arduino/arduino_connection_storage.h"
#include "../hw/arduino/arduino_connection_tracker.h"
#include "../hw/arduino/arduino_thread.h"
#include "ceammc_platform.h"
#include "serial/serial.h"

#include "test_base.h"

#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <thread>

using namespace ceammc::hw;
using namespace ceammc::platform;

inline void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#define CHECK_ARDUINO_CONNECTION()                                              \
    {                                                                           \
        PortList l = ArduinoThread::findAllArduinos();                          \
        size_t n = l.size();                                                    \
        if (!n) {                                                               \
            std::cerr << "please connect Arduino board to pass all tests...\n"; \
            return;                                                             \
        } else {                                                                \
            std::cerr << "Arduino found:\n";                                    \
            for (size_t i = 0; i < n; i++) {                                    \
                std::cerr << "\t" << l[i].port << "\n";                         \
            }                                                                   \
        }                                                                       \
    }

template <class Fn>
class ThreadRunner {
    pthread_t t_;
    Fn fn_;

public:
    ThreadRunner(Fn f)
        : t_(0)
        , fn_(f)
    {
        pthread_create(&t_, 0, &ThreadRunner::thread, this);
    }

    ~ThreadRunner()
    {
        pthread_join(t_, 0);
    }

    static void* thread(void* v)
    {
        ThreadRunner* this_ = (ThreadRunner*)v;
        this_->fn_();
        return 0;
    }
};

#define THREAD_RUN(fn) \
    ThreadRunner<BOOST_TYPEOF(fn)> f(fn);

TEST_CASE("hw::ArduinoThread", "[hw]")
{
    SECTION("tracker")
    {
        Arduino a;
        serial::PortInfo pi;
        pi.port = "port1";
        pi.hardware_id = "USB VID:PID=2341:0043 SNR=754363434303517142C0";

        REQUIRE_FALSE(a.isConnected());
        REQUIRE(ConnectionStorage::instance().size() == 0);

        {
            ConnectionTracker t(&a, pi);
            REQUIRE(a.isConnected());
            REQUIRE(ConnectionStorage::instance().size() == 1);
            REQUIRE(ConnectionStorage::instance().used("port1"));
            REQUIRE(a.usbSerial() == "754363434303517142C0");
            REQUIRE(a.vendorId() == 0x2341);
            REQUIRE(a.productId() == 0x43);
        }

        REQUIRE_FALSE(a.isConnected());
        REQUIRE(ConnectionStorage::instance().size() == 0);
    }

    SECTION("parserArduinoId")
    {
        int v = 0, p = 0;
        std::string str;
        REQUIRE_FALSE(ArduinoThread::parseArduinoId("asd", v, p, str));
        REQUIRE(v == 0);
        REQUIRE(p == 0);
        REQUIRE(str.empty());

        REQUIRE(ArduinoThread::parseArduinoId("USB VID:PID=2341:0043 SNR=754363434303517142C0", v, p, str));
        REQUIRE(v == 0x2341);
        REQUIRE(p == 0x43);
        REQUIRE(str == "754363434303517142C0");
    }

    SECTION("base")
    {
        CHECK_ARDUINO_CONNECTION();

        PortList lst = ArduinoThread::findAllArduinos();
        REQUIRE(lst.size() > 0);

        std::string port0 = lst[0].port;
        REQUIRE(ArduinoThread::findDeviceByPort(lst, port0).port == port0);
        REQUIRE(ArduinoThread::findDeviceByPort(lst, "not exists").port == "");

        std::cerr << lst[0].hardware_id << "\n";

        int vid = 0, pid = 0;
        std::string serial;
        ArduinoThread::parseArduinoId(lst[0].hardware_id, vid, pid, serial);

        REQUIRE(ArduinoThread::findDeviceByVendorId(lst, vid).port == port0);
        REQUIRE(ArduinoThread::findDeviceByVendorId(lst, -100).port.empty());

        REQUIRE(ArduinoThread::findDeviceBySerialNo(lst, serial).port == port0);
        REQUIRE(ArduinoThread::findDeviceBySerialNo(lst, "???").port.empty());

        REQUIRE(ArduinoThread::findDeviceById(lst, vid, pid).port == port0);
        REQUIRE(ArduinoThread::findDeviceById(lst, vid, -1).port.empty());
        REQUIRE(ArduinoThread::findDeviceById(lst, -1, pid).port.empty());
        REQUIRE(ArduinoThread::findDeviceById(lst, -1, -1).port.empty());

        // test on empty lists
        REQUIRE(ArduinoThread::findDeviceByPort(PortList(), "123").port.empty());
        REQUIRE(ArduinoThread::findDeviceByVendorId(PortList(), vid).port.empty());
        REQUIRE(ArduinoThread::findDeviceBySerialNo(PortList(), "123").port.empty());
        REQUIRE(ArduinoThread::findDeviceById(PortList(), vid, pid).port.empty());
    }

    SECTION("non-connected")
    {
        SECTION("reconnect")
        {
            Arduino a;
            a.setReconnect(true);

            SECTION("thread wait for port")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoPort, &a, "unknown_port"));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for serial")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoUsbSerial, &a, "SOME_SERIAL"));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for vendor")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoVendorId, &a, 123));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for vendor and product")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoId, &a, 123, 125));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for free arduino")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForFreeArduino, &a));

                sleep_ms(100);
                a.stop();
            }
        }

        SECTION("no reconnect")
        {
            Arduino a;
            a.setReconnect(false);

            SECTION("thread wait for port")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoPort, &a, "unknown_port"));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for serial")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoUsbSerial, &a, "SOME_SERIAL"));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for vendor")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoVendorId, &a, 123));
                sleep_ms(100);
                a.stop();
            }

            SECTION("thread wait for vendor and product")
            {
                THREAD_RUN(boost::bind(ArduinoThread::waitForArduinoId, &a, 123, 125));
                sleep_ms(100);
                a.stop();
            }
        }
    }

    SECTION("search order")
    {
        SECTION("by port")
        {
            Arduino a("port");
            a.setReconnect(false);

            THREAD_RUN(boost::bind(ArduinoThread::waitForArduino, &a));
            sleep_ms(100);
            a.stop();
        }

        SECTION("by serial")
        {
            Arduino a("", 9600, "SERIAL");
            a.setReconnect(false);

            THREAD_RUN(boost::bind(ArduinoThread::waitForArduino, &a));
            sleep_ms(100);
            a.stop();
        }

        SECTION("by vendor")
        {
            Arduino a("", 9600, "", 1000);
            a.setReconnect(false);

            THREAD_RUN(boost::bind(ArduinoThread::waitForArduino, &a));
            sleep_ms(100);
            a.stop();
        }

        SECTION("by vendor and product")
        {
            Arduino a("", 9600, "", 1000, 2000);
            a.setReconnect(false);

            THREAD_RUN(boost::bind(ArduinoThread::waitForArduino, &a));
            sleep_ms(100);
            a.stop();
        }

        SECTION("wait for free")
        {
            Arduino a;
            a.setReconnect(false);

            THREAD_RUN(boost::bind(ArduinoThread::waitForArduino, &a));
            sleep_ms(100);
            a.stop();
        }
    }

    SECTION("storage")
    {
        ConnectionStorage& s1 = ConnectionStorage::instance();
        ConnectionStorage& s2 = ConnectionStorage::instance();
        REQUIRE(&s1 == &s2);
        REQUIRE(s1.size() == 0);
        s1.add("port1");
        REQUIRE(s1.size() == 1);
        REQUIRE(s1.used("port1"));
        s2.add("port2");
        REQUIRE(s2.size() == 2);
        REQUIRE(s2.used("port2"));
        s1.remove("port1");
        REQUIRE(s2.used("port2"));
        REQUIRE_FALSE(s2.used("port1"));
        REQUIRE(s2.size() == 1);
        s1.remove("port2");
    }
}
