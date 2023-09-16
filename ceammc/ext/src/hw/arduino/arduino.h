#ifndef ARDUINO_H
#define ARDUINO_H

#include <cstdint>
#include <mutex>

#include <deque>
#include <string>
#include <vector>

#include <pthread.h>

namespace ceammc {
namespace hw {

    class ArduinoThread;
    class ConnectionTracker;
    class ThreadTracker;

    enum MessageType {
        MSG_ERROR = 2,
        MSG_DEBUG = 1,
        MSG_CONNECT = 3
    };

    typedef std::deque<uint8_t> ArduinoQueue;
    typedef std::vector<uint8_t> ArduinoData;
    typedef std::pair<MessageType, std::string> Message;
    typedef std::deque<Message> Messages;

    class Arduino {
        std::string port_;
        std::string usb_serial_;
        volatile bool is_connected_;
        volatile bool should_stop_;
        volatile bool is_running_;
        volatile bool reconnect_;
        pthread_t thread_;
        mutable std::mutex mutex_;
        int baud_rate_;
        int vendor_id_;
        int product_id_;
        ArduinoQueue output_data_;
        ArduinoQueue input_data_;
        Messages messages_;

    public:
        Arduino(const std::string& port = "",
            int baudRate = 57600,
            const std::string& usbSerial = "",
            int vendorId = 0,
            int productId = 0);
        ~Arduino();

        int baudRate() const;

        std::string port() const;
        void setPort(const std::string& port);

        std::string usbSerial() const;
        void setUsbSerial(const std::string& usbSerial);

        int vendorId() const;
        void setVendorId(int vid);

        int productId() const;
        void setProductId(int pid);

        bool start();
        bool stop();

        bool isConnected() const;
        bool shouldStopThread() const;
        bool reconnect() const;
        void setReconnect(bool value);

        // input
        ArduinoQueue inputData() const;
        bool hasInputData() const;
        void pushInputData(uint8_t byte);
        size_t read(ArduinoData& data);

        // output
        ArduinoData outputData() const;
        bool hasOutputData() const;
        void popWrittenData(size_t n);
        size_t write(const std::string& v);
        size_t write(const ArduinoData& data);
        size_t write(uint8_t byte);

        // worker messages
        bool hasMessages() const;
        Messages popMessages();
        void pushError(const std::string& msg);
        void pushDebug(const std::string& msg);
        void pushMessage(MessageType t, const std::string& msg);

    public:
        friend class ArduinoThread;
        friend class ConnectionTracker;
        friend class ThreadTracker;

    public:
        static std::vector<std::string> deviceList();
        static std::vector<std::string> allDevices();

    private:
        void setConnected(bool v);
        void threadFinished();
    };

} // namespace hw
} // namespace ceammc

#endif // ARDUINO_H
