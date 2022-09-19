#ifndef CEAMMC_NOTIFY_H
#define CEAMMC_NOTIFY_H

namespace ceammc {

enum NotifyEventType {
    NOTIFY_NONE,
    NOTIFY_UPDATE,
    NOTIFY_SOURCE_REMOVED,
    NOTIFY_DONE,
    NOTIFY_LOG_ERROR,
    NOTIFY_LOG_POST,
    NOTIFY_LOG_DEBUG,
    NOTIFY_LOG_LOG
};

/**
 * Notify interface
 */
class NotifiedObject {
public:
    virtual ~NotifiedObject() {}
    virtual bool notify(NotifyEventType code) = 0;
};

}

#endif // CEAMMC_NOTIFY_H
