#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "config.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __WIN32
#include <windows.h>
#endif

using namespace ceammc;

class SystemHostname : public BaseObject {
public:
    SystemHostname(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onBang()
    {
#ifdef __WIN32
        char buf[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD sz = MAX_COMPUTERNAME_LENGTH + 1;
        if (GetComputerNameA(buf, &sz))
            symbolTo(0, gensym(buf));
        else
            OBJ_ERR << "can't get hostname";
#else
        char buf[1000];
        if (gethostname(buf, 1000) == 0)
            symbolTo(0, gensym(buf));
        else
            OBJ_ERR << "can't get hostname";
#endif
    }
};

void setup_system_hostname()
{
    ObjectFactory<SystemHostname> obj("system.hostname");
}
