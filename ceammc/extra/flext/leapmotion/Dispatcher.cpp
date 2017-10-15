#include "m_pd.h"
#include "Dispatcher.h"

using namespace Leap;

void Dispatcher::onInit(const Controller& controller) {
    post("Leap Motion for Pd by Chikashi Miyama Ver 0.1");
}

void Dispatcher::onConnect(const Controller& controller) {
    post("Leap:connectd");
}

void Dispatcher::onDisconnect(const Controller& controller) {
    post("Leap:disconnected");
}

void Dispatcher::onExit(const Controller& controller) {
    post("Leap:Exited");
}

void Dispatcher::onFrame(const Controller& controller) {
    frame = controller.frame();
}
