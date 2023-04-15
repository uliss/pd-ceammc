#ifndef CEAMMC_NOTIFY_H
#define CEAMMC_NOTIFY_H

#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace ceammc {

/**
 * Notify interface
 */
class NotifiedObject {
public:
    virtual ~NotifiedObject() { }
    virtual bool notify(int code) = 0;
};

using SubscriberId = uint64_t;

struct NotifyMessage {
    SubscriberId id;
    int event;
};

struct SubscriberInfo {
    SubscriberId id;
    NotifiedObject* obj;
};

/**
 * Send notifications to waiting threads
 */
class ThreadNotify {
    std::condition_variable notify_;
    std::mutex mtx_;
    using Lock = std::unique_lock<std::mutex>;

    ThreadNotify(const ThreadNotify&) = delete;
    ThreadNotify(ThreadNotify&&) = delete;
    ThreadNotify& operator=(const ThreadNotify&) = delete;
    ThreadNotify& operator=(ThreadNotify&&) = delete;

public:
    ThreadNotify();

    /**
     * notify one waiting thread
     * @note called from main thread
     */
    void notifyOne();

    /**
     * notify all waiting threads
     * @note called from main thread
     */
    void notifyAll();

    /**
     * @brief wait for notification
     * blocks until specified time in ms has elapsed
     * or the notification arrived, whichever comes first
     * @param ms = maximum blocking time
     * @note called from worker thread
     */
    void waitFor(int ms = 100);
};

constexpr const char* OSC_DISPATCHER = "#osc";
constexpr const char* OSC_METHOD_UPDATE = ".update";

}

#endif // CEAMMC_NOTIFY_H
