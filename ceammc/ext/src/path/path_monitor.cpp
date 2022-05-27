/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "path_monitor.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#define DMON_IMPL
#include "dmon.h"

#include <string>
#include <unordered_set>

using PathSet = std::unordered_set<std::string>;

using WatchMap = std::unordered_map<PathMonitor*, dmon_watch_id>;

struct MutexLock {
    std::mutex& m_;

public:
    MutexLock(std::mutex& m)
        : m_(m)
    {
        m_.lock();
    }

    ~MutexLock()
    {
        m_.unlock();
    }
};

class DMonitor {
    int refs_ { 0 };
    WatchMap watch_map_;
    std::mutex mtx_;

    DMonitor()
    {
        dmon_init();
    }

    ~DMonitor()
    {
        dmon_deinit();
    }

    void init()
    {
        if (++refs_ == 1) {
            std::cerr << "dmon_init()\n";
        }
    }

    void deinit()
    {
        if (refs_ <= 0)
            return;

        refs_--;
        std::cerr << "dmon_deinit()\n";
    }

public:
    static DMonitor& instance()
    {
        static DMonitor i_;
        return i_;
    }

    void addRef(PathMonitor* obj, t_symbol* s, SubscriberId id)
    {
        init();

        MutexLock l(mtx_);

        auto it = watch_map_.find(obj);
        if (it == watch_map_.end()) {
            auto mid = dmon_watch(s->s_name, watch_callback, 0, this);
            watch_map_.insert({ obj, mid });
        } else {
            dmon_unwatch(it->second);
            it->second = dmon_watch(s->s_name, watch_callback, 0, this);
        }
    }

    void removeRef(PathMonitor* obj)
    {
        MutexLock l(mtx_);
        auto it = watch_map_.find(obj);
        if (it == watch_map_.end())
            return;

        dmon_unwatch(it->second);
        watch_map_.erase(it);
        deinit();
    }

public:
    static void watch_callback(dmon_watch_id watch_id, dmon_action action, const char* rootdir,
        const char* filepath, const char* oldfilepath, void* user)
    {
        DMonitor* mon = static_cast<DMonitor*>(user);

        MutexLock l(mon->mtx_);
        for (auto& w : mon->watch_map_) {
            if (w.second.id == watch_id.id) {
                SubscriberId id = reinterpret_cast<SubscriberId>(w.first);
                w.first->setPath(filepath);
                Dispatcher::instance().send({ id, (NotifyEventType)action });
                return;
            }
        }
    }
};

PathMonitor::PathMonitor(const PdArgs& args)
    : NotifiedObject(args)
    , path_(nullptr)
{
    createOutlet();

    path_ = new SymbolProperty("@paths", &s_);
    addProperty(path_);
    path_->setArgIndex(0);
    path_->setSuccessFn([this](Property*) {
        t_symbol* path = path_->value();
        if (path == &s_)
            return;

        SubscriberId id = reinterpret_cast<SubscriberId>(this);
        DMonitor::instance().addRef(this, path, id);
    });

    SubscriberId id = reinterpret_cast<SubscriberId>(this);
    Dispatcher::instance().subscribe(this, id);
}

PathMonitor::~PathMonitor()
{
    DMonitor::instance().removeRef(this);
    Dispatcher::instance().unsubscribe(this);
}

bool PathMonitor::notify(NotifyEventType code)
{
    OBJ_DBG << "code: " << code;
    dmon_action act = static_cast<dmon_action>(code);

    MutexLock l(mtx_);

    switch (act) {
    case DMON_ACTION_CREATE:
        OBJ_DBG << "file created: " << path_info_;
        anyTo(0, gensym("create"), gensym(path_info_.c_str()));
        break;
    case DMON_ACTION_DELETE:
        OBJ_DBG << "file removed: " << path_info_;
        anyTo(0, gensym("remove"), gensym(path_info_.c_str()));
        break;
    case DMON_ACTION_MODIFY:
        OBJ_DBG << "file changed: " << path_info_;
        anyTo(0, gensym("update"), gensym(path_info_.c_str()));
        break;
    case DMON_ACTION_MOVE:
        OBJ_DBG << "file moved: " << path_info_;
        anyTo(0, gensym("move"), gensym(path_info_.c_str()));
        break;
    default:
        break;
    }

    return true;
}

void PathMonitor::setPath(const char* path)
{
    MutexLock lock(mtx_);
    path_info_ = path;
}

void setup_path_monitor()
{
    ObjectFactory<PathMonitor> obj("path.monitor");
}
