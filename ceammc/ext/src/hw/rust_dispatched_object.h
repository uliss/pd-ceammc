#ifndef RUST_DISPATCHED_OBJECT_H
#define RUST_DISPATCHED_OBJECT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"

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

    ceammc_hw_msg_cb on_err()
    {
        return { static_cast<void*>(this), [](void* user, const char* msg) {
                    auto obj = static_cast<typeof(this)>(user);
                    Error err(obj);
                    err << msg;
                } };
    }
};

}

#endif // RUST_DISPATCHED_OBJECT_H
