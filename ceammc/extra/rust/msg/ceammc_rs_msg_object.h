#ifndef CEAMMC_RS_MSG_OBJECT_H_
#define CEAMMC_RS_MSG_OBJECT_H_

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_rs_msg.hpp"

namespace ceammc {

template <class T>
class RustMessageObject : public DispatchedObject<T> {
public:
    RustMessageObject(const PdArgs& args)
        : DispatchedObject<T>(args)
    {
    }

protected:
    ceammc_msg_notify on_notify() const
    {
        return {
            this->subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); }
        };
    }

    ceammc_msg_notify_int on_notify_int() const
    {
        return {
            this->subscriberId(), [](size_t id, std::int32_t value) { Dispatcher::instance().send({ id, value }); }
        };
    }

    ceammc_msg_cb on_message()
    {
        return {
            static_cast<void*>(this),
            [](void* user, ceammc_msg_level level, const char* msg) {
                auto obj = static_cast<typeof(this)>(user);
                switch (level) {
                case ceammc_msg_level::Debug:
                    Debug(obj) << msg;
                    break;
                case ceammc_msg_level::Post:
                    Post(obj) << msg;
                    break;
                case ceammc_msg_level::Log:
                    Log(obj) << msg;
                    break;
                default:
                    Error(obj) << msg;
                    break;
                }
            },
        };
    }
};
} // namespace ceammc

#endif // CEAMMC_RS_MSG_OBJECT_H_
