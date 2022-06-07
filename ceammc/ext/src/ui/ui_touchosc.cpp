/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "ui_touchosc.h"
#include "ceammc_factory.h"

#include "httplib.h"

#include <chrono>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

using MutexLock = std::lock_guard<std::mutex>;

#define TOUCH_OSC_LOG_PREFIX "[touchosc] "
constexpr int TOUCH_OSC_HTTP_PORT = 9658;

class Logger : public NotifiedObject {
    std::list<std::pair<NotifyEventType, std::string>> messages_;
    std::mutex mtx_;

public:
    Logger()
    {
        Dispatcher::instance().subscribe(this, id());
    }

    ~Logger()
    {
        Dispatcher::instance().unsubscribe(this);
    }

    SubscriberId id() const { return reinterpret_cast<SubscriberId>(this); }

    bool notify(NotifyEventType code)
    {
        MutexLock lock(mtx_);
        while (!messages_.empty()) {
            auto& msg = messages_.front();

            switch (msg.first) {
            case NOTIFY_LOG_ERROR:
                LIB_ERR << TOUCH_OSC_LOG_PREFIX << msg.second;
                break;
            case NOTIFY_LOG_DEBUG:
                LIB_DBG << TOUCH_OSC_LOG_PREFIX << msg.second;
                break;
            default:
                break;
            }

            messages_.pop_front();
        }

        return true;
    }

    void error(const std::string& msg)
    {
        {
            MutexLock lock(mtx_);
            messages_.push_back({ NOTIFY_LOG_ERROR, msg });
        }

        Dispatcher::instance().send({ id(), NOTIFY_LOG_ERROR });
    }

    void debug(const std::string& msg)
    {
        {
            MutexLock lock(mtx_);
            messages_.push_back({ NOTIFY_LOG_DEBUG, msg });
        }

        Dispatcher::instance().send({ id(), NOTIFY_LOG_DEBUG });
    }
};

class TouchOscHttpServer {
    httplib::Server http_;
    std::future<bool> rc_;
    Logger log_;

    TouchOscHttpServer()
    {
        using namespace httplib;

        http_.set_exception_handler([this](const Request& req, Response& res, std::exception& e) {
            log_.error(e.what());
        });

        http_.Get("/", [this](const Request&, Response& res) {
            log_.debug("GET /");
            res.set_content("Hello World!", "text/plain");

            //            res.set_content_provider(
            //                "text/plain", // Content type
            //                [&](size_t offset, DataSink& sink) {
            //                    if (/* there is still data */) {
            //                        std::vector<char> data;
            //                        // prepare data...
            //                        sink.write(data.data(), data.size());
            //                    } else {
            //                        sink.done(); // No more data
            //                    }
            //                    return true; // return 'false' if you want to cancel the process.
            //                });
        });
    }

    ~TouchOscHttpServer()
    {
        http_.stop();
    }

public:
    static TouchOscHttpServer& instance()
    {
        static TouchOscHttpServer srv;
        return srv;
    }

    void start(bool value)
    {
        if (value) {
            if (rc_.valid()) {
                auto st = rc_.wait_for(std::chrono::seconds(0));

                switch (st) {
                case std::future_status::ready:
                case std::future_status::deferred:
                    rc_.get();
                    break;
                default:
                    log_.debug("is still running, wait...");
                    return;
                }
            }

            rc_ = std::async(std::launch::async, [this]() -> bool {
                log_.debug("server started");
                return http_.listen("0.0.0.0", TOUCH_OSC_HTTP_PORT);
            });
        } else {
            log_.debug("server stopped");
            http_.stop();
        }
    }
};

UiTouchOsc::UiTouchOsc(const PdArgs& args)
    : BaseObject(args)
    , server_(nullptr)
{
    server_ = new BoolProperty("@server", false);
    server_->setSuccessFn([this](Property*) {
        TouchOscHttpServer::instance().start(server_->value());
    });
    addProperty(server_);
}

UiTouchOsc::~UiTouchOsc()
{
}

void UiTouchOsc::m_auto(t_symbol* s, const AtomListView& lv)
{
}

void setup_ui_touchosc()
{
    ObjectFactory<UiTouchOsc> obj("ui.touchosc");
    obj.addMethod("auto", &UiTouchOsc::m_auto);
}
