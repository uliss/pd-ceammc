#include "arduino_connection_tracker.h"
#include "arduino.h"
#include "arduino_connection_storage.h"
#include "arduino_thread.h"
#include "serial/serial.h"

#include <iostream>

ceammc::hw::ConnectionTracker::ConnectionTracker(ceammc::hw::Arduino* a, const serial::PortInfo& info)
    : a_(a)
{
    a_->setPort(info.port);

    // add connection to storage
    ConnectionStorage::instance().add(info.port);

    // fill arduino info
    int vid = 0, pid = 0;
    std::string sn;

    if (ArduinoThread::parseArduinoId(info.hardware_id, vid, pid, sn)) {
        a_->setUsbSerial(sn);
        a_->setVendorId(vid);
        a_->setProductId(pid);
    } else {
        std::cerr << "[arduino_thread] can't parse hardware id: "
                  << info.hardware_id << "\n";
    }

    a_->setConnected(true);
    a_->pushDebug("connected to: " + a_->port());
    a_->pushMessage(MSG_CONNECT, "");
}

ceammc::hw::ConnectionTracker::~ConnectionTracker()
{
    a_->pushDebug("disconnected from: " + a_->port());
    // remove connection from storage
    ConnectionStorage::instance().remove(a_->port());
    a_->setConnected(false);
}

bool ceammc::hw::ConnectionTracker::isConnected(const std::string& port)
{
    return ConnectionStorage::instance().used(port);
}
