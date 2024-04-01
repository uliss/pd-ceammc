#include "net_rust.hpp"

#include <functional>
#include <iostream>

namespace ceammc {
namespace net {

    template <typename T, typename Result>
    class NetService {
    public:
        using service_handle_new_fn = T*(ceammc_callback_msg,
            ceammc_callback_msg,
            ceammc_callback_msg,
            ceammc_callback_msg,
            ceammc_callback_progress,
            ceammc_http_client_result_cb,
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
        std::function<void(const Result&)> cb_res_;

    public:
        NetService(service_handle_new_fn cb_new,
            service_handle_free_fn cb_free,
            service_handle_proc_fn cb_proc,
            ceammc_callback_notify cb_notify //
            )
            : handle_(nullptr)
            , cb_free_(cb_free)
            , cb_proc_(cb_proc)
        {
            handle_ = cb_new({ this, onError },
                { this, onPost },
                { this, onDebug },
                { this, onLog },
                { this, onProgress },
                { this, onResult },
                cb_notify);
        }

        ~NetService()
        {
            if (handle_ && this->cb_free_)
                this->cb_free_(handle_);
        }

        void processResults()
        {
            if (handle_ && cb_proc_)
                cb_proc_(handle_);
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

        void setResultCallback(std::function<void(const Result&)> cb)
        {
            cb_res_ = cb;
        }

    public:
        static void onError(void* user, const char* msg)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_err_)
                this_->cb_err_(msg);
            else
                std::cerr << msg << std::endl;
        }

        static void onPost(void* user, const char* msg)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_post_)
                this_->cb_post_(msg);
        }

        static void onDebug(void* user, const char* msg)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_debug_)
                this_->cb_debug_(msg);
        }

        static void onLog(void* user, const char* msg)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_log_)
                this_->cb_log_(msg);
        }

        static void onProgress(void* user, std::uint8_t percent)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_progress_)
                this_->cb_progress_(percent);
        }

        static void onResult(void* user, const Result* res)
        {
            auto this_ = static_cast<NetService*>(user);
            if (this_ && this_->cb_res_ && res)
                this_->cb_res_(*res);
        }
    };

}
}
