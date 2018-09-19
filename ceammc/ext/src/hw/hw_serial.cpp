/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "hw_serial.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "serial/serial.h"

#include <array>
#include <boost/optional.hpp>
#include <deque>
#include <fcntl.h>
#include <mutex>

namespace ceammc {

static const int DEFAULT_TIMEOUT = 5;

enum SerialTaskError {
    NO_ERROR = 0,
    ERR_PORT_NOT_FOUND = 1,
    ERR_PORT_CONNECTION,
    ERR_EXCEPTION,
    ERR_TRANSFER,
    ERR_WRITE
};

class SerialTask : public thread::Task {
    t_symbol* port_;
    int baud_rate_;
    int to_pd_;
    typedef std::vector<uint8_t> SerialQueue;
    typedef std::lock_guard<std::mutex> Lock;
    SerialQueue from_pd_;
    std::mutex mutex_;

public:
    SerialTask(t_symbol* port = &s_, int rate = 57600)
        : port_(port)
        , baud_rate_(rate)
        , to_pd_(-1)
    {
    }

    boost::optional<serial::PortInfo> findPort() const
    {
        if (port_ == &s_) {
            auto p = serial::list_ports();
            if (p.empty())
                return boost::none;

            return p.back();
        } else {
            for (auto& p : serial::list_ports()) {
                if (p.port == port_->s_name)
                    return p;
            }

            return boost::none;
        }
    }

    void pushByte(uint8_t b)
    {
        Lock lock(mutex_);
        from_pd_.push_back(b);
    }

    void pushBytes(const AtomList& l)
    {
        Lock lock(mutex_);
        for (auto& a : l)
            from_pd_.push_back(a.asFloat());
    }

    int run() override
    {
        using namespace serial;

        try {
            serial::Serial serial;
            auto pinfo = findPort();

            if (!pinfo) {
                std::string msg("can't find port: ");
                msg += port_->s_name;
                writeError(msg.c_str());
                return ERR_PORT_NOT_FOUND;
            }

            serial.setBaudrate(baud_rate_);
            serial.setPort(pinfo->port);
            serial.open();

            if (!serial.isOpen()) {
                std::string msg("can't connect to ");
                msg += port_->s_name;
                writeError(msg.c_str());
                return ERR_PORT_CONNECTION;
            }

            // update port name
            // this needed when empty port name specified
            {
                Lock lock(mutex_);
                port_ = gensym(pinfo->port.c_str());
                writeCommand(TASK_UPDATE);

                std::string msg("connected to ");
                msg += port_->s_name;
                writeDebug(msg.c_str());
            }

            serial.setDTR(false);
            serial.setStopbits(stopbits_one);
            serial.setTimeout(Timeout::max(), DEFAULT_TIMEOUT, 0, DEFAULT_TIMEOUT, 0);

            // main read/write cycle
            try {
                while (!stopRequested()) {
                    // read one byte from serial port
                    uint8_t chr;
                    if (serial.read(&chr, 1) == 1) {
                        // write to PureData fd
                        if (write(to_pd_, &chr, 1) == -1) {
                            writeError("write error");
                            return ERR_WRITE;
                        }
                    }

                    // write to serial
                    {
                        Lock lock(mutex_);
                        size_t n = from_pd_.size();

                        if (n > 0) {
                            serial.write(from_pd_.data(), n);
                            from_pd_.clear();
                        }
                    }
                }
            } catch (std::exception& e) {
                writeError(e.what());
                return ERR_TRANSFER;
            }
        } catch (std::exception& e) {
            writeError(e.what());
            return ERR_EXCEPTION;
        }

        return 0;
    }

    t_symbol* port()
    {
        Lock lock(mutex_);
        return port_;
    }

    void setPort(t_symbol* name)
    {
        Lock lock(mutex_);
        port_ = name;
    }

    void setBaudRate(int r)
    {
        baud_rate_ = r;
    }

    void setOutFd(int fd)
    {
        to_pd_ = fd;
    }
};

SerialPort::SerialPort(const PdArgs& args)
    : ThreadExternal(args, new SerialTask())
    , port_(nullptr)
    , baud_rate_(nullptr)
    , serial_out_(this, &SerialPort::handleSerialOutput)
{
    createOutlet();

    port_ = new SymbolProperty("@port", positionalSymbolArgument(1, &s_), false);
    createProperty(port_);

    baud_rate_ = new IntEnumProperty("@rate", positionalFloatArgument(0, 57600));

    baud_rate_->appendEnum(110);
    baud_rate_->appendEnum(300);
    baud_rate_->appendEnum(600);
    baud_rate_->appendEnum(1200);
    baud_rate_->appendEnum(2400);
    baud_rate_->appendEnum(4800);
    baud_rate_->appendEnum(9600);
    baud_rate_->appendEnum(14400);
    baud_rate_->appendEnum(19200);
    baud_rate_->appendEnum(38400);
    baud_rate_->appendEnum(56000);
    baud_rate_->appendEnum(57600);
    baud_rate_->appendEnum(115200);
    baud_rate_->appendEnum(128000);
    baud_rate_->appendEnum(256000);

    createProperty(baud_rate_);

    createCbProperty("@devices", &SerialPort::propDevices);

    setNonBlocking(serial_out_.fd[0]);
}

void SerialPort::onFloat(t_float f)
{
    serial()->pushByte(static_cast<uint8_t>(f));
}

void SerialPort::onList(const AtomList& l)
{
    serial()->pushBytes(l);
}

void SerialPort::onThreadDone(int rc)
{
}

bool SerialPort::onThreadCommand(int code)
{
    if (code == TASK_UPDATE) {
        port_->setValue(serial()->port());
        return true;
    }

    return ThreadExternal::onThreadCommand(code);
}

void SerialPort::m_open(t_symbol* s, const AtomList& l)
{
    if (isRunning()) {
        OBJ_ERR << "already connected";
        return;
    }

    // using numeric index
    if (l.isFloat()) {
        int idx = atomlistToValue<int>(l, 0);
        auto ports = serial::list_ports();
        if (idx >= 0 || idx < ports.size()) {
            port_->setValue(gensym(ports[idx].port.c_str()));
        }
    }

    serial()->setPort(port_->value());
    serial()->setBaudRate(baud_rate_->value());
    serial()->setOutFd(serial_out_.fd[1]);
    start();
}

void SerialPort::m_close(t_symbol* s, const AtomList&)
{
    if (!isRunning()) {
        OBJ_ERR << "not connected";
        return;
    }

    quit();
}

AtomList SerialPort::propDevices() const
{
    AtomList res;
    auto ports = serial::list_ports();

    res.reserve(ports.size());
    for (auto& p : ports)
        res.append(gensym(p.port.c_str()));

    return res;
}

void SerialPort::handleSerialOutput(int fd)
{
    std::array<uint8_t, 64> buf;
    ssize_t n = 0;
    while ((n = read(fd, buf.data(), buf.size())) > 0) {
        for (ssize_t i = 0; i < n; i++)
            floatTo(0, buf[i]);
    }
}

SerialTask* SerialPort::serial()
{
    return (SerialTask*)task_.get();
}
}

void hw_setup_serial()
{
    using namespace ceammc;
    ObjectFactory<SerialPort> obj("hw.serial");
    obj.addMethod("open", &SerialPort::m_open);
    obj.addMethod("close", &SerialPort::m_close);
}
