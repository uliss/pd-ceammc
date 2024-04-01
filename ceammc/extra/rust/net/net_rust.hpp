#ifndef ceammc_rust_net_h
#define ceammc_rust_net_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


enum class ceammc_http_client_param_type {
    Header,
    Form,
    MultiPart,
    Selector,
    Mime,
};

enum class ceammc_mqtt_qos {
    /// may lose messages
    AtMostOnce = 0,
    /// guarantees the message delivery but potentially exists duplicate messages
    AtLeastOnce = 1,
    /// ensures that messages are delivered exactly once without duplication
    ExactlyOnce = 2,
};

enum class ceammc_mqtt_rc {
    Ok = 0,
    InvalidString,
    InvalidData,
    InvalidClient,
    ClientError,
    Disconnected,
    ConnectionError,
};

enum class ceammc_ws_client_selector {
    ALL,
    FIRST,
    LAST,
    ID,
    EXCEPT,
};

enum class ceammc_ws_rc {
    Ok = 0,
    InvalidClient,
    InvalidServer,
    InvalidMessage,
    InvalidData,
    InvalidClientId,
    SendError,
    NoData,
    CloseError,
    ConnectionClosed,
    NonBlockingError,
    SocketAcceptError,
    SocketReadError,
    SocketDeferClose,
    RunloopExit,
};

enum class ceammc_ws_trim {
    NO,
    START,
    END,
    BOTH,
};

struct ceammc_http_client;

struct ceammc_mqtt_client;

struct ceammc_ws_client;

struct ceammc_ws_server;

struct ceammc_http_client_param {
    const char *name;
    const char *value;
    ceammc_http_client_param_type param_type;
};

struct ceammc_callback_msg {
    void *user;
    void (*cb)(void *user, const char *msg);
};

struct ceammc_http_client_result {
    const char *body;
    uint16_t status;
};

struct ceammc_http_client_result_cb {
    void *user;
    void (*cb)(void *user, const ceammc_http_client_result*);
};

struct ceammc_callback_notify {
    size_t id;
    void (*cb)(size_t id);
};

struct ceammc_mqtt_err_cb {
    void *user;
    void (*cb)(void *user, const char *msg);
};

struct ceammc_mqtt_ping_cb {
    void *user;
    void (*cb)(void *user);
};

struct ceammc_mqtt_pub_cb {
    void *user;
    void (*cb)(void *user, const char *topic, const uint8_t *data, size_t len);
};

struct ceammc_mqtt_cb {
    ceammc_mqtt_err_cb on_err;
    ceammc_mqtt_ping_cb on_ping;
    ceammc_mqtt_pub_cb on_pub;
};

struct ceammc_ws_callback_text {
    void *user;
    void (*cb)(void *user, const char *msg);
};

struct ceammc_ws_callback_data {
    void *user;
    void (*cb)(void *user, const uint8_t *data, size_t len);
};

struct ceammc_ws_client_target {
    ceammc_ws_client_selector sel;
    size_t id;
};

struct ceammc_ws_conn_info {
    const char *addr;
    size_t id;
};

struct ceammc_ws_srv_on_text {
    void *user;
    void (*cb)(void *user, const char *msg, const ceammc_ws_conn_info *info);
};

struct ceammc_ws_srv_on_data {
    void *user;
    void (*cb)(void *user, const uint8_t *msg, size_t len, const ceammc_ws_conn_info *info);
};

struct ceammc_ws_srv_on_cli {
    void *user;
    void (*cb)(void *user, const ceammc_ws_conn_info *info);
};


extern "C" {

void ceammc_http_client_free(ceammc_http_client *cli);

bool ceammc_http_client_get(ceammc_http_client *cli,
                            const char *url,
                            const ceammc_http_client_param *param,
                            size_t param_len);

[[nodiscard]]
ceammc_http_client *ceammc_http_client_new(ceammc_callback_msg cb_err,
                                           ceammc_callback_msg cb_post,
                                           ceammc_callback_msg cb_debug,
                                           ceammc_callback_msg cb_log,
                                           ceammc_http_client_result_cb cb_reply,
                                           ceammc_callback_notify cb_notify);

bool ceammc_http_client_post(ceammc_http_client *cli,
                             const char *url,
                             const ceammc_http_client_param *param,
                             size_t param_len);

bool ceammc_http_client_process(ceammc_http_client *cli);

bool ceammc_http_client_upload(ceammc_http_client *cli,
                               const char *url,
                               const char *file,
                               const char *file_key,
                               const ceammc_http_client_param *params,
                               size_t params_len);

/// create new mqtt client
/// @param url - mqtt broker url in format mqtt://host:port
/// @param id - client id (can be NULL)
/// @param cb - callbacks
/// @return pointer to mqtt_client (must be freed by ceammc_mqtt_client_free()) on success
///         or NULL on error
ceammc_mqtt_client *ceammc_mqtt_client_create(const char *url, const char *id, ceammc_mqtt_cb cb);

/// free mqtt client
/// @param cli - mqtt client
void ceammc_mqtt_client_free(ceammc_mqtt_client *cli);

/// publish text message into mqtt topic
/// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
/// @param msg - mqtt message
/// @param qos - Quality of Service flag
/// @param retain - This flag tells the broker to store the message for a topic
///        and ensures any new client subscribing to that topic will receive the stored message.
/// @return ceammc_mqtt_rc::Ok on success
ceammc_mqtt_rc ceammc_mqtt_client_publish(ceammc_mqtt_client *cli,
                                          const char *topic,
                                          const char *msg,
                                          ceammc_mqtt_qos qos,
                                          bool retain);

/// publish binary data into mqtt topic
/// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
/// @param data - mqtt binary data
/// @param len - mqtt binary data length
/// @param qos - Quality of Service flag
/// @param retain - This flag tells the broker to store the message for a topic
///        and ensures any new client subscribing to that topic will receive the stored message
/// @return ceammc_mqtt_rc::Ok on success
ceammc_mqtt_rc ceammc_mqtt_client_publish_data(ceammc_mqtt_client *cli,
                                               const char *topic,
                                               const uint8_t *data,
                                               size_t len,
                                               ceammc_mqtt_qos qos,
                                               bool retain);

/// subscribe to mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return ceammc_mqtt_rc::Ok on success
ceammc_mqtt_rc ceammc_mqtt_client_subscribe(ceammc_mqtt_client *cli, const char *topic);

/// unsubscribe from mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return ceammc_mqtt_rc::Ok on success
ceammc_mqtt_rc ceammc_mqtt_client_unsubscribe(ceammc_mqtt_client *cli, const char *topic);

/// iterate mqtt events
/// @note - this is blocking call
/// @param cli - mqtt client pointer
/// @param time_ms - time to blocking wait in milliseconds
/// @return ceammc_mqtt_rc::Ok on success
ceammc_mqtt_rc ceammc_mqtt_process_events(ceammc_mqtt_client *cli, uint16_t time_ms);

/// close client connection
/// @param cli - pointer to websocket client
/// @return ceammc_ws_rc::Ok, ceammc_ws_rc::InvalidClient, ceammc_ws_rc::CloseError,
ceammc_ws_rc ceammc_ws_client_close(ceammc_ws_client *cli);

/// create websocket client
/// @param url - string in format: ws://HOST:PORT?/path?
/// @param on_err - callback for error messages
/// @param on_text - callback for incoming text messages
/// @param on_bin - callback for incoming binary messages
/// @param on_ping - callback for incoming ping messages
/// @param on_pong - callback for incoming pong messages
/// @param on_close - callback on connection close
/// @return pointer to client or NULL
ceammc_ws_client *ceammc_ws_client_create(const char *url,
                                          ceammc_ws_callback_text on_err,
                                          ceammc_ws_callback_text on_text,
                                          ceammc_ws_callback_data on_bin,
                                          ceammc_ws_callback_data on_ping,
                                          ceammc_ws_callback_data on_pong,
                                          ceammc_ws_callback_data on_close);

/// flush client connection
/// @param cli - pointer to websocket client
/// @return ceammc_ws_rc::Ok, ceammc_ws_rc::InvalidClient, ceammc_ws_rc::SendError,
ceammc_ws_rc ceammc_ws_client_flush(ceammc_ws_client *cli);

/// free websocket client
/// @param cli - pointer to websocket client
void ceammc_ws_client_free(ceammc_ws_client *cli);

/// process all available messages from WebSocket server
/// @param cli - pointer to websocket client
/// @param trim - text message trim mode
/// @return ws_rc::Ok, ws_rc::InvalidClient, ws_rc::InvalidMessage, ws_rc::CloseError, ws_rc::SendError,
ceammc_ws_rc ceammc_ws_client_process_events(ceammc_ws_client *cli,
                                             ceammc_ws_trim trim);

/// sends binary message to WebSocket server
/// @param cli - pointer to ws client
/// @param data - data pointer
/// @param len - data length
/// @param flush - if true ensures all messages
///        previously passed to write and automatic queued pong responses are written & flushed into the underlying stream.
/// @return ws_rc::Ok, ws_rc::InvalidClient, ws_rc::InvalidMessage, ws_rc::CloseError, ws_rc::SendError,
ceammc_ws_rc ceammc_ws_client_send_binary(ceammc_ws_client *cli,
                                          const uint8_t *data,
                                          size_t len,
                                          bool flush);

/// sends ping to WebSocket server
/// @param cli - pointer to websocket client
/// @param data - pointer to ping data (can be NULL)
/// @param len - data length
/// @return ws_rc::Ok, ws_rc::InvalidClient, ws_rc::InvalidMessage, ws_rc::CloseError, ws_rc::SendError,
ceammc_ws_rc ceammc_ws_client_send_ping(ceammc_ws_client *cli,
                                        const uint8_t *data,
                                        size_t len);

/// sends text message to WebSocket server
/// @param cli - pointer to ws client
/// @param msg - text message
/// @param flush - if true ensures all messages
///        previously passed to write and automatic queued pong responses are written & flushed into the underlying stream.
/// @return ws_rc::Ok, ws_rc::InvalidClient, ws_rc::InvalidMessage, ws_rc::CloseError, ws_rc::SendError,
ceammc_ws_rc ceammc_ws_client_send_text(ceammc_ws_client *cli,
                                        const char *msg,
                                        bool flush);

/// close websocket server client connections by sending them close handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_close_clients(ceammc_ws_server *srv, ceammc_ws_client_target target);

/// request connected client id
/// @param srv - pointer to server
/// @param cb - request callback
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_connected_clients(ceammc_ws_server *srv,
                                                void *user,
                                                void (*cb)(void *user, const ceammc_ws_conn_info *msg, size_t len));

ceammc_ws_server *ceammc_ws_server_create(const char *addr,
                                          ceammc_ws_srv_on_text on_err,
                                          ceammc_ws_srv_on_text on_txt,
                                          ceammc_ws_srv_on_data on_bin,
                                          ceammc_ws_srv_on_data on_ping,
                                          ceammc_ws_srv_on_cli on_conn,
                                          ceammc_ws_srv_on_cli on_disc);

/// free websocket server
/// @param src - pointer to server
void ceammc_ws_server_free(ceammc_ws_server *srv);

/// process server events (non-blocking) and execute callbacks
/// @param srv - pointer to websocket server
ceammc_ws_rc ceammc_ws_server_process_events(ceammc_ws_server *srv);

/// send binary message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_send_binary(ceammc_ws_server *srv,
                                          const uint8_t *data,
                                          size_t len,
                                          ceammc_ws_client_target target);

/// send ping message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_send_ping(ceammc_ws_server *srv,
                                        const uint8_t *data,
                                        size_t len,
                                        ceammc_ws_client_target target);

/// send text message to connected clients
/// @param srv - pointer to websocket server
/// @param msg - text message
/// @param target - specify target clients
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_send_text(ceammc_ws_server *srv,
                                        const char *msg,
                                        ceammc_ws_client_target target);

/// abort all client connections without handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
/// @return ceammc_ws_rc
ceammc_ws_rc ceammc_ws_server_shutdown_clients(ceammc_ws_server *srv,
                                               ceammc_ws_client_target target);

} // extern "C"

#endif // ceammc_rust_net_h
