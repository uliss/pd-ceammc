#include "mod_net.h"
#include "net_host.h"
#include "net_rust.hpp"

namespace ceammc {
void setup_net_esphome_client();
void setup_net_wled_udp();
}

extern void setup_net_artnet_send();
extern void setup_net_freesound();
extern void setup_net_http_client();
extern void setup_net_http_send();
extern void setup_net_mdns();
extern void setup_net_mqtt_client();
extern void setup_net_osc_receive();
extern void setup_net_osc_send();
extern void setup_net_osc_server();
extern void setup_net_telegram_bot();
extern void setup_net_ws_client();
extern void setup_net_ws_server();

void ceammc_net_setup()
{
    ceammc_net_logger_init();

    ceammc::setup_net_esphome_client();
    ceammc::setup_net_wled_udp();
    setup_net_artnet_send();
    setup_net_freesound();
    setup_net_host();
    setup_net_http_client();
    setup_net_http_send();
    setup_net_mdns();
    setup_net_mqtt_client();
    setup_net_osc_receive();
    setup_net_osc_send();
    setup_net_osc_server();
    setup_net_telegram_bot();
    setup_net_ws_client();
    setup_net_ws_server();
}
