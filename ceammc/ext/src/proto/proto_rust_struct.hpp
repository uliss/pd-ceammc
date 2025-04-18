/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef PROTO_RUST_STRUCT_HPP
#define PROTO_RUST_STRUCT_HPP

#include "proto_rust.hpp"

#include <cstdint>
#include <functional>
#include <iostream>

namespace ceammc {
namespace proto {

    template <typename T, typename InitParams, typename ResultCallback>
    class ProtoService {
    public:
        using service_handle_new_fn = T*(
            InitParams,
            ceammc_callback_msg,
            ceammc_callback_msg,
            ceammc_callback_msg,
            ceammc_callback_msg,
            ResultCallback,
            ceammc_callback_notify);

        using service_handle_free_fn = void(T*);
        using service_handle_proc_fn = bool(T*);
        using service_handle_progress_fn = void(std::uint8_t);

    private:
        T* handle_;
        std::function<void(T*)> cb_free_;
        std::function<bool(T*)> cb_proc_;
        std::function<void(int)> cb_progress_;
        std::function<void(const char*msg)> cb_err_, cb_post_, cb_debug_, cb_log_;

    public:
        ProtoService(const InitParams& init_params,
            service_handle_new_fn cb_new,
            service_handle_free_fn cb_free,
            service_handle_proc_fn cb_proc,
            ResultCallback cb_result,
            ceammc_callback_notify cb_notify //
            )
            : handle_(nullptr)
            , cb_free_(cb_free)
            , cb_proc_(cb_proc)
        {
            handle_ = cb_new( //
                init_params,
                { this, onError },
                { this, onPost },
                { this, onDebug },
                { this, onLog },
                cb_result,
                cb_notify);
        }

        ~ProtoService()
        {
            if (handle_ && this->cb_free_)
                this->cb_free_(handle_);
        }

        bool processResults()
        {
            if (handle_ && cb_proc_) {
                return cb_proc_(handle_);
            } else
                return false;
        }

        T* handle() { return handle_; }

        void setErrorCallback(std::function<void(const char*)> cb)
        {
            cb_err_ = cb;
        }

        void setPostCallback(std::function<void(const char*)> cb)
        {
            cb_post_ = cb;
        }

        void setDebugCallback(std::function<void(const char*)> cb)
        {
            cb_debug_ = cb;
        }

        void setLogCallback(std::function<void(const char*)> cb)
        {
            cb_log_ = cb;
        }

        void setProgressCallback(std::function<void(int)> cb)
        {
            cb_progress_ = cb;
        }

    public:
        static void onError(void* user, const char* msg)
        {
            auto this_ = static_cast<ProtoService*>(user);
            if (this_ && this_->cb_err_)
                this_->cb_err_(msg);
            else
                std::cerr << msg << std::endl;
        }

        static void onPost(void* user, const char* msg)
        {
            auto this_ = static_cast<ProtoService*>(user);
            if (this_ && this_->cb_post_)
                this_->cb_post_(msg);
        }

        static void onDebug(void* user, const char* msg)
        {
            auto this_ = static_cast<ProtoService*>(user);
            if (this_ && this_->cb_debug_)
                this_->cb_debug_(msg);
        }

        static void onLog(void* user, const char* msg)
        {
            auto this_ = static_cast<ProtoService*>(user);
            if (this_ && this_->cb_log_)
                this_->cb_log_(msg);
        }
    };

} // namespace proto
} // namespace ceammc

#endif // PROTO_RUST_STRUCT_HPP
