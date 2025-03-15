#ifndef RUST_DISPATCHED_OBJECT_H
#define RUST_DISPATCHED_OBJECT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"

#define CEAMMC_OBJECT_ADD_METHOD(obj, cls, method) obj.addMethod(#method, &cls::m_##method);

namespace ceammc {

template <class T>
class RustDispatchedObject : public DispatchedObject<T> {
public:
    RustDispatchedObject(const PdArgs& args)
        : DispatchedObject<T>(args)
    {
    }

protected:
    ceammc_hw_notify_cb on_notify() const
    {
        return {
            this->subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); }
        };
    }

    ceammc_hw_msg_cb on_message()
    {
        return { static_cast<void*>(this),
            [](void* user, ceammc_hw_msg_level level, const char* msg) {
                auto obj = static_cast<typeof(this)>(user);
                switch (level) {
                case ceammc_hw_msg_level::Debug:
                    Debug(obj) << msg;
                    break;
                case ceammc_hw_msg_level::Info:
                    Post(obj) << msg;
                    break;
                default:
                    Error(obj) << msg;
                    break;
                }
            } };
    }
};

}

#endif // RUST_DISPATCHED_OBJECT_H
