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
#include "ceammc_base64.h"
#include "ceammc_canvas.h"
#include "ceammc_format.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"

#include "fmt/format.h"
#include "httplib.h"

#include <chrono>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

#include "ceammc_ui.h"
#include "ui_bang.h"
#include "ui_knob.h"
#include "ui_slider.h"
#include "ui_toggle.h"

#include "touchosc/layout.h"
#include "touchosc/push_button.h"

using MutexLock = std::lock_guard<std::mutex>;

#define TOUCH_OSC_LOG_PREFIX "[touchosc] "
constexpr int TOUCH_OSC_HTTP_PORT = 9658;

namespace ceammc {

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
    std::string content_;
    std::string filename_;
    std::mutex content_mtx_;

    TouchOscHttpServer()
        : filename_("PdUI.touchosc")
    {
        using namespace httplib;

        http_.set_exception_handler([this](const Request& req, Response& res, std::exception& e) {
            log_.error(e.what());
        });

        http_.Get("/", [this](const Request&, Response& res) {
            log_.debug("GET /");

            std::string xml;
            std::string fname;

            {
                // make a thread local copy
                std::lock_guard<std::mutex> lock(content_mtx_);
                xml = content_;
                fname = filename_;
            }

            res.set_content(xml, "application/touchosc");
            res.set_header("Content-Disposition",
                fmt::format(R"(attachment; filename="PdUI.touchosc")", fname));
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

    void setContent(const std::string& str)
    {
        std::lock_guard<std::mutex> lock(content_mtx_);
        content_ = str;
    }

    void setFilename(const std::string& name)
    {
        filename_ = fmt::format("{}.touchosc", name);
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
    , layout_(new touchosc::Layout())
{
    server_ = new BoolProperty("@server", false);
    server_->setSuccessFn([this](Property*) {
        TouchOscHttpServer::instance().start(server_->value());
        auto root = rootCanvas();
        if (root) {
            auto name = canvas_info_name(root);
            TouchOscHttpServer::instance().setFilename(name->s_name);
            TouchOscHttpServer::instance().setContent(xml_content_);
        }
    });
    addProperty(server_);
}

UiTouchOsc::~UiTouchOsc()
{
}

void UiTouchOsc::m_auto(t_symbol* s, const AtomListView& lv)
{
    auto cnv = canvas();
    if (!cnv) {
        return;
    }

    const auto cbng = &UIObjectFactory<UIBang>::pd_ui_class->c_class;
    const auto ctgl = &UIObjectFactory<UIToggle>::pd_ui_class->c_class;
    const auto cknob = &UIObjectFactory<UIKnob>::pd_ui_class->c_class;
    const auto csl = &UIObjectFactory<UISlider>::pd_ui_class->c_class;

    enum UIType {
        UI_BANG,
        UI_TOGGLE,
        UI_KNOB,
        UI_SLIDER
    };

    std::vector<std::pair<UIType, UIObject*>> res;

    for (auto x = cnv->gl_list; x != nullptr; x = x->g_next) {
        auto cls = x->g_pd;
        auto pdobj = pd_checkobject(&cls);
        // skip non-patchable objects
        if (!pdobj)
            continue;

        if (cbng == cls)
            res.emplace_back(UI_BANG, (UIObject*)x);
        else if (ctgl == cls)
            res.emplace_back(UI_TOGGLE, (UIObject*)x);
        else if (cknob == cls)
            res.emplace_back(UI_KNOB, (UIObject*)x);
        else if (csl == cls)
            res.emplace_back(UI_SLIDER, (UIObject*)x);
    }

    using namespace touchosc;

    const int scene = 1;

    layout_->clear();
    layout_->append(TabPagePtr(new TabPage(fmt::format("{}", scene))));
    auto& page = layout_->tabs().back();

    int btn_cnt = 1;

    for (auto ux : res) {
        switch (ux.first) {
        case UI_BANG: {
            METHOD_DBG(s) << "bang found";
            auto bng = static_cast<UIBang*>(ux.second);
            auto name = fmt::format("btn{}", btn_cnt);
            auto osc = fmt::format("/{}/btn{}", page->name(), btn_cnt);

            ControlPtr ctl { new PushButton(name) };
            ctl->osc().setPath(osc);
            ctl->setPos(bng->x(), bng->y());
            ctl->setSize(bng->width(), bng->height());
            ctl->setColor(
                bng->prop_color_background.red,
                bng->prop_color_background.green,
                bng->prop_color_background.blue);

            page->append(std::move(ctl));
            btn_cnt++;
        } break;
        }
    }

    page->layout();

    xml_content_ = to_string(*layout_);

    LIB_DBG << xml_content_;
}

void UiTouchOsc::m_save(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "file name expected";
        return;
    }

    const auto spath = to_string(lv);
    const auto path = spath.c_str();

    if (isAbsolutePath(path)) {
        if (platform::path_exists(path)) {
            if (!platform::remove(path)) {
                METHOD_ERR(s) << "can't overwrite " << path;
                return;
            }
        }

        if (layout_) {
            if (!layout_->saveToFile(path)) {
                METHOD_ERR(s) << "can't save to: " << path;
                return;
            } else
                METHOD_LOG(s) << "faved to " << path;
        }
    } else {
        auto full_path = platform::make_abs_filepath_with_canvas(rootCanvas(), spath);

        if (platform::path_exists(full_path.c_str())) {
            if (!platform::remove(full_path.c_str())) {
                METHOD_ERR(s) << "can't overwrite " << full_path;
                return;
            }
        }

        if (layout_) {
            if (!layout_->saveToFile(full_path)) {
                METHOD_ERR(s) << "can't save to: " << full_path;
                return;
            } else
                METHOD_LOG(s) << "faved to " << full_path;
        }
    }
}
}

void setup_ui_touchosc()
{
    using namespace ceammc;
    ObjectFactory<UiTouchOsc> obj("ui.touchosc");
    obj.addMethod("auto", &UiTouchOsc::m_auto);
    obj.addMethod("save", &UiTouchOsc::m_save);
}
