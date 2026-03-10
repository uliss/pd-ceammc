#include "mod_net.h"
#include "net_host.h"

#if defined(WITH_MQTT) || defined(WITH_TELEGRAM) || defined(WITH_HTTP) || defined(WITH_FREESOUND) || defined(WITH_WEBSOCKET)
#include "net_rust.hpp"
#define HAS_NET_RUST 1
#endif

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
#ifdef HAS_NET_RUST
    ceammc_net_logger_init();
#endif

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
