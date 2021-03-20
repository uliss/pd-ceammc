#include "arduino.h"
#include "arduino_thread.h"
#include "ceammc_thread.h"
#include "serial/serial.h"

#include <cerrno>
#include <cstring>

#include <algorithm>
#include <iostream>

#define ARDUINO_THREAD_CHECK(status, msg)                               \
    {                                                                   \
        if (status != 0) {                                              \
            std::cerr << "[arduino] " << msg << ", status = " << status \
                      << ", str: " << strerror(status) << "\n";         \
        }                                                               \
    }

using namespace ceammc::thread;

namespace ceammc {
namespace hw {

    Arduino::Arduino(const std::string& port,
        int baudRate,
        const std::string& usbSerial,
        int vendorId,
        int productId)

        : port_(port)
        , usb_serial_(usbSerial)
        , is_connected_(false)
        , should_stop_(false)
        , is_running_(false)
        , reconnect_(true)
        , baud_rate_(baudRate)
        , vendor_id_(vendorId)
        , product_id_(productId)
    {
        int status = 0;

        status = pthread_mutex_init(&mutex_, 0);
        ARDUINO_THREAD_CHECK(status, "can't init mutex");
    }

    Arduino::~Arduino()
    {
        stop();

        int err = pthread_mutex_destroy(&mutex_);
        ARDUINO_THREAD_CHECK(err, "can't destroy mutex");
    }

    std::string Arduino::usbSerial() const
    {
        return usb_serial_;
    }

    std::string Arduino::port() const
    {
        return port_;
    }

    int Arduino::baudRate() const
    {
        return baud_rate_;
    }

    int Arduino::vendorId() const
    {
        return vendor_id_;
    }

    int Arduino::productId() const
    {
        return product_id_;
    }

    bool Arduino::start()
    {
        if (is_running_) {
            std::cerr << "[arduino] thread is running...\n";
            return false;
        }

        should_stop_ = false;

        int err = pthread_create(&thread_, 0, ArduinoThread::worker, this);
        ARDUINO_THREAD_CHECK(err, "thread create failed");
        if (err)
            return false;

        is_running_ = true;
        return true;
    }

    bool Arduino::stop()
    {
        should_stop_ = true;

        if (!is_running_) {
            std::cerr << "[arduino] thread is not running...\n";
            return false;
        }

        int err = pthread_join(thread_, 0);
        ARDUINO_THREAD_CHECK(err, "thread join failed");
        if (err)
            return false;

        is_running_ = false;
        return true;
    }

    bool Arduino::isConnected() const
    {
        Lock lock(mutex_);
        return is_connected_;
    }

    bool Arduino::shouldStopThread() const
    {
        Lock lock(mutex_);
        return should_stop_;
    }

    bool Arduino::reconnect() const
    {
        Lock lock(mutex_);
        return reconnect_;
    }

    void Arduino::setReconnect(bool value)
    {
        Lock lock(mutex_);
        reconnect_ = value;
    }

    ArduinoQueue Arduino::inputData() const
    {
        Lock lock(mutex_);
        return input_data_;
    }

    bool Arduino::hasInputData() const
    {
        Lock lock(mutex_);
        return !input_data_.empty();
    }

    ArduinoData Arduino::outputData() const
    {
        Lock lock(mutex_);
        ArduinoData res;
        res.reserve(output_data_.size());
        std::copy(output_data_.begin(), output_data_.end(), std::back_inserter(res));
        return res;
    }

    bool Arduino::hasOutputData() const
    {
        Lock lock(mutex_);
        return !output_data_.empty();
    }

    void Arduino::popWrittenData(size_t n)
    {
        Lock lock(mutex_);

        size_t total = std::min(n, output_data_.size());

        for (size_t i = 0; i < total; i++) {
            output_data_.pop_front();
        }
    }

    size_t Arduino::write(const std::string& v)
    {
        Lock lock(mutex_);
        std::copy(v.begin(), v.end(), std::back_inserter(output_data_));
        return v.size();
    }

    size_t Arduino::write(const ArduinoData& data)
    {
        Lock lock(mutex_);
        std::copy(data.begin(), data.end(), std::back_inserter(output_data_));
        return data.size();
    }

    size_t Arduino::write(uint8_t byte)
    {
        Lock lock(mutex_);
        output_data_.push_back(byte);
        return 1;
    }

    bool Arduino::hasMessages() const
    {
        Lock lock(mutex_);
        return !messages_.empty();
    }

    Messages Arduino::popMessages()
    {
        Lock lock(mutex_);

        Messages res(messages_);
        messages_.clear();
        return res;
    }

    void Arduino::pushInputData(uint8_t byte)
    {
        Lock lock(mutex_);
        input_data_.push_back(byte);
    }

    size_t Arduino::read(ArduinoData& data)
    {
        Lock lock(mutex_);

        size_t n = input_data_.size();
        if (n < 1)
            return 0;

        std::copy(input_data_.begin(), input_data_.end(), std::back_inserter(data));

        // remove elements
        for (size_t i = 0; i < n; i++)
            input_data_.pop_front();

        return n;
    }

    void Arduino::setPort(const std::string& port)
    {
        Lock lock(mutex_);
        port_ = port;
    }

    void Arduino::setUsbSerial(const std::string& usbSerial)
    {
        Lock lock(mutex_);
        usb_serial_ = usbSerial;
    }

    void Arduino::setVendorId(int vid)
    {
        Lock lock(mutex_);
        vendor_id_ = vid;
    }

    void Arduino::setProductId(int pid)
    {
        Lock lock(mutex_);
        product_id_ = pid;
    }

    void Arduino::setConnected(bool v)
    {
        Lock lock(mutex_);
        is_connected_ = v;
    }

    void Arduino::pushError(const std::string& msg)
    {
        pushMessage(MSG_ERROR, msg);
    }

    void Arduino::pushDebug(const std::string& msg)
    {
        pushMessage(MSG_DEBUG, msg);
    }

    void Arduino::pushMessage(MessageType t, const std::string& msg)
    {
        Lock lock(mutex_);
        messages_.push_back(std::make_pair(t, msg));
    }

    std::vector<std::string> Arduino::deviceList()
    {
        PortList lst = ArduinoThread::findAllArduinos();
        std::vector<std::string> res;

        for (auto& pi : lst) {
            res.push_back(pi.port);
        }

        return res;
    }

    std::vector<std::string> Arduino::allDevices()
    {
        PortList lst = ArduinoThread::findAllDevices();
        std::vector<std::string> res;

        for (auto& pi : lst) {
            res.push_back(pi.port);
        }

        return res;
    }

    void Arduino::threadFinished()
    {
        Lock lock(mutex_);
        is_running_ = false;
    }

} // namespace hw
} // namespace ceammc
