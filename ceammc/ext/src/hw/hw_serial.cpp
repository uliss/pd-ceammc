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

enum SerialTaskCommand {
    TASK_READ_FROM_SERIAL = TASK_LAST_ENUM + 1,
    TASK_UPDATE_PORT_NAME
};

class SerialTask : public thread::Task {
    t_symbol* port_;
    int baud_rate_;
    thread::Pipe* pipe_in_;
    thread::Pipe* pipe_out_;
    typedef std::lock_guard<std::mutex> Lock;
    std::mutex mutex_;

public:
    SerialTask(SerialPort* caller, t_symbol* port = &s_, int rate = 57600)
        : Task(caller)
        , port_(port)
        , baud_rate_(rate)
        , pipe_in_(nullptr)
        , pipe_out_(nullptr)
    {
    }

    void setPipeInOut(thread::Pipe* in, thread::Pipe* out)
    {
        pipe_in_ = in;
        pipe_out_ = out;
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
                writeCommand(TASK_UPDATE_PORT_NAME);

                std::string msg("connected to ");
                msg += port_->s_name;
                writeDebug(msg.c_str());
            }

            serial.setDTR(false);
            serial.setStopbits(stopbits_one);
            serial.setTimeout(Timeout::max(), DEFAULT_TIMEOUT, 0, DEFAULT_TIMEOUT, 0);

            // main read/write cycle
            try {
                std::string in_buf;
                const size_t BLOCK = 16;

                while (!stopRequested()) {
                    in_buf = serial.read(BLOCK);
                    if (!in_buf.empty()) {
                        for (size_t i = 0; i < in_buf.size(); i++)
                            pipe_out_->enqueue(in_buf[i]);

                        writeCommand(TASK_READ_FROM_SERIAL);
                    }

                    // write to serial
                    for (size_t i = 0; i < BLOCK; i++) {
                        uint8_t out_ch;
                        if (!pipe_in_->try_dequeue(out_ch))
                            break;

                        serial.write(&out_ch, 1);
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
};

SerialPort::SerialPort(const PdArgs& args)
    : ThreadExternal(args, new SerialTask(this))
    , port_(nullptr)
    , baud_rate_(nullptr)
    , pipe_in_(new thread::Pipe(256))
    , pipe_out_(new thread::Pipe(256))
{
    createOutlet();

    port_ = new SymbolProperty("@port", &s_);
    port_->setArgIndex(1);
    addProperty(port_);

    baud_rate_ = new IntEnumProperty("@rate", { 57600, 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 115200, 128000, 256000 });
    baud_rate_->setArgIndex(0);
    addProperty(baud_rate_);

    createCbProperty("@devices", &SerialPort::propDevices);

    serial()->setPipeInOut(pipe_in_.get(), pipe_out_.get());
}

SerialPort::~SerialPort()
{
    waitStop();
}

void SerialPort::onFloat(t_float f)
{
    // TODO check for connected
    pipe_in_->enqueue(static_cast<uint8_t>(f));
}

void SerialPort::onList(const AtomList& l)
{
    // TODO check for connected
    for (auto& a : l)
        pipe_in_->enqueue(a.asFloat());
}

void SerialPort::onThreadDone(int rc)
{
}

bool SerialPort::onThreadCommand(int code)
{
    switch (code) {
    case TASK_UPDATE_PORT_NAME:
        port_->setValue(serial()->port());
        return true;
    case TASK_READ_FROM_SERIAL: {
        if (!pipe_out_)
            return false;

        char ch;
        while (pipe_out_->try_dequeue(ch))
            floatTo(0, static_cast<unsigned char>(ch));

        return true;
    } break;
    }

    return ThreadExternal::onThreadCommand(code);
}

void SerialPort::m_open(t_symbol* s, const AtomListView& l)
{
    if (isRunning()) {
        OBJ_ERR << "already connected";
        return;
    }

    // using numeric index
    if (l.isFloat()) {
        int idx = l.toT<int>(0);
        auto ports = serial::list_ports();
        if (idx >= 0 || idx < ports.size()) {
            port_->setValue(gensym(ports[idx].port.c_str()));
        }
    }

    serial()->setPort(port_->value());
    serial()->setBaudRate(baud_rate_->value());
    start();
}

void SerialPort::m_close(t_symbol* s, const AtomListView&)
{
    if (!isRunning()) {
        OBJ_ERR << "not connected";
        return;
    }

    stop();
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
