/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_FILESYSTEM_H
#define CEAMMC_FILESYSTEM_H

#include "ceammc_either.h"
#include "ceammc_notify.h"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"

#include <chrono>
#include <exception>
#include <functional>
#include <future>

namespace ceammc {

namespace fs {
    Either<std::string> readFileContent(const char* path);

    Either<bool> readFileLines(const char* path, const std::function<void(size_t, const std::string&)>& line_cb);

    Either<bool> writeFileContent(const char* path, const char* data, size_t len, bool overwrite = false);
    Either<bool> writeFileContent(const char* path, const std::string& data, bool overwrite = false);

}

template <typename Base>
class AsyncFileReader : public DispatchedObject<Base> {
    std::future<std::string> result_;
    std::string content_;
    ThreadPdLogger logger_;

    AsyncFileReader(const PdArgs& args)
        : DispatchedObject<Base>(args)
    {
    }

    ~AsyncFileReader()
    {
        if (result_.valid()) {
            try {
                content_ = result_.get();
            } catch (std::exception& e) {
                OBJ_ERR << e.what();
            }
        }
    }

    const std::string& content() const { return content_; }

    bool notify(int /*code*/) override
    {
        try {
            if (result_.valid()) {
                switch (result_.wait_for(std::chrono::milliseconds(100))) {
                case std::future_status::ready:
                    content_ = result_.get();
                    break;
                case std::future_status::timeout:
                    OBJ_ERR << "no read result";
                    return false;
                default:
                    return false;
                }
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            return false;
        }

        return false;
    }

protected:
    bool fileReadRequest(const char* path)
    {
        try {
            if (result_.valid()) {
                switch (result_.wait_for(std::chrono::milliseconds(0))) {
                case std::future_status::deferred:
                    result_.get();
                    break;
                case std::future_status::timeout: {
                    OBJ_ERR << "previous file read request is not finished";
                    return false;
                }
                case std::future_status::ready:
                    result_.get();
                    break;
                }
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }

        result_ = std::async(std::launch::async, [this](const std::string& path) -> std::string {
            auto res = fs::readFileContent(path.c_str());
            RuntimeError err;
            if (res.matchError(err)) {
                logger_.error(err.what());
                return {};
            }

            Dispatcher::instance().send({ asyncReadId(), 0 });

            return res.value();
        });
    }

    SubscriberId asyncReadId() const { return reinterpret_cast<SubscriberId>(this); }
};

}

#endif // CEAMMC_FILESYSTEM_H
