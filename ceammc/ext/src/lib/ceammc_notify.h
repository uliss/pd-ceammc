#ifndef CEAMMC_NOTIFY_H
#define CEAMMC_NOTIFY_H

namespace ceammc {

/**
 * Notify interface
 */
class NotifiedObject {
public:
    virtual ~NotifiedObject() {}
    virtual bool notify(int code) = 0;
};

}

#endif // CEAMMC_NOTIFY_H
