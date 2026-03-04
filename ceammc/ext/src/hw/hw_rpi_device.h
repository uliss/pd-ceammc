/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef HW_RPI_DEVICE_H
#define HW_RPI_DEVICE_H

#include "rust_dispatched_object.h"

namespace ceammc {

template <typename HardwareHandle>
class HwRpiDevice : public RustDispatchedObject<BaseObject> {
public:
    using FreeDeviceFn = void (*)(HardwareHandle*);
    using Device = std::unique_ptr<HardwareHandle, FreeDeviceFn>;

    static const auto CONNECT_PRIORITY = 128;
    static const auto DEVICE_CONNECTED_PRIORITY = 160;

public:
    HwRpiDevice(FreeDeviceFn fn, const PdArgs& args)
        : RustDispatchedObject<BaseObject>(args)
        , hw_(nullptr, fn)
        , free_fn_(fn)
    {

        connect_ = new BoolProperty("@connect", false);
        connect_->info().setPriority(CONNECT_PRIORITY);
        connect_->setSuccessFn([this](Property*) {
            connect(connect_->value());
        });
        addProperty(connect_);
    }

    virtual bool check_connected(bool print_error)
    {
        if (!hw_ && print_error)
            OBJ_ERR << "device is not connected";

        return hw_.get();
    }

    virtual void connect(bool state)
    {
        if (state) {
            if (hw_) {
                OBJ_DBG << "already connected";
                return;
            }

            hw_ = std::move(createDevice());

            if (!hw_)
                OBJ_ERR << "can't connect to the device";
        } else {
            hw_.reset();
        }
    }

    virtual Device createDevice() = 0;

    HardwareHandle* device() { return hw_.get(); }
    const HardwareHandle* device() const { return hw_.get(); }
    FreeDeviceFn freeDeviceFn() const { return free_fn_; }

    Device nullDevice() const
    {
        return Device(nullptr, free_fn_);
    }

private:
    BoolProperty* connect_ { nullptr };
    Device hw_;
    FreeDeviceFn free_fn_ { nullptr };
};

} // namespace ceammc

#endif // HW_RPI_DEVICE_H
