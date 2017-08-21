#include "Leap.h"
using namespace Leap;

class Dispatcher : public Listener {
  public:
    Frame frame;
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
};
