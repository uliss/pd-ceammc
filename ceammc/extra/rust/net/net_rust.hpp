#ifndef ceammc_rust_net_h
#define ceammc_rust_net_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


enum class ceammc_http_client_param_type {
    /// header key/value
    Header,
    /// form key/value
    Form,
    /// multipart key/value
    MultiPart,
    /// CSS selector/output type
    Selector,
    /// mime type (for uploading)
    Mime,
    /// username/password
    BasicAuth,
    /// strip whitespaces
    StripWhiteSpaces,
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
    Success = 0,
    RefusedProtocolVersion,
    BadClientId,
    ServiceUnavailable,
    BadUserNamePassword,
    NotAuthorized,
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

struct ceammc_telegram_bot_client;

struct ceammc_ws_client;

struct ceammc_ws_server;

struct ceammc_http_client_param {
    /// param name or key
    const char *name;
    /// param value
    const char *value;
    /// parameter type
    ceammc_http_client_param_type param_type;
};

struct ceammc_http_client_init {
    const void *_dummy;
};

struct ceammc_callback_msg {
    void *user;
    void (*cb)(void *user, const char *msg);
};

struct ceammc_callback_progress {
    void *user;
    void (*cb)(void *user, uint8_t msg);
};

struct ceammc_http_client_result {
    /// reply text (can be NULL!)
    const char *data;
    /// HTTP status
    uint16_t status;
};

struct ceammc_http_client_result_cb {
    /// user data pointer (can be NULL)
    void *user;
    /// callback function (can be NULL)
    void (*cb)(void *user, const ceammc_http_client_result*);
};

struct ceammc_callback_notify {
    size_t id;
    void (*cb)(size_t id);
};

struct ceammc_mqtt_client_init {
    const char *url;
    const char *id;
};

struct ceammc_mqtt_client_result_cb {
    /// user data pointer (can be NULL)
    void *user;
    /// ping callback function (can be NULL)
    void (*ping_cb)(void *user);
    /// connected callback function (can be NULL)
    void (*conn_cb)(void *user, ceammc_mqtt_rc code);
    /// publish callback function (can be NULL)
    void (*pub_cb)(void *user, const char *topic, const uint8_t *data, size_t data_len, ceammc_mqtt_qos qos, bool retain, uint16_t pkid);
};

struct ceammc_telegram_bot_init {
    /// telegram bot TOKEN
    const char *token;
};

struct ceammc_telegram_bot_result_cb {
    /// user data pointer (can be NULL)
    void *user;
    /// connection callback function (can be NULL)
    void (*whoami_cb)(void *user, uint64_t user_id, const char *username);
    /// text callback function (can be NULL)
    void (*text_cb)(void *user, int64_t chat_id, int32_t msg_id, const char *text);
    /// location callback function (can be NULL)
    void (*loc_cb)(void *user, int64_t chat_id, double latitude, double longitude);
    /// sticker callback function (can be NULL)
    void (*sti_cb)(void *user, int64_t chat_id, const char *file_id, const char *emoji);
    /// voice callback function (can be NULL)
    void (*voice_cb)(void *user, int64_t chat_id, const char *file_id, const char *file_unique_id, const char *mime, uint32_t file_duration, uint64_t file_size);
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

/// download file with GET request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param file - output filename (if NULL - try to auto detect)
/// @param dir - base directory to save if filename is not absolute
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
bool ceammc_http_client_download(ceammc_http_client *cli,
                                 const char *url,
                                 const char *file,
                                 const char *dir,
                                 const ceammc_http_client_param *params,
                                 size_t params_len);

/// free http client
/// @param cli - pointer to http client (can be NULL)
void ceammc_http_client_free(ceammc_http_client *cli);

/// do http get request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
bool ceammc_http_client_get(ceammc_http_client *cli,
                            const char *url,
                            const ceammc_http_client_param *param,
                            size_t param_len);

/// create new http client
/// @param cb_err - called in the main thread on error message
/// @param cb_post - called in the main thread on post message
/// @param cb_debug - called in the main thread on debug message
/// @param cb_log - called in the main thread on log message
/// @param cb_progress - called in the main thread on progress message
/// @param cb_reply - called in the main thread on result reply message
/// @param cb_notify - called in the worker thread (!) to notify main thread
/// @return pointer to new client or NULL on error
[[nodiscard]]
ceammc_http_client *ceammc_http_client_new(ceammc_http_client_init _params,
                                           ceammc_callback_msg cb_err,
                                           ceammc_callback_msg cb_post,
                                           ceammc_callback_msg cb_debug,
                                           ceammc_callback_msg cb_log,
                                           ceammc_callback_progress cb_progress,
                                           ceammc_http_client_result_cb cb_reply,
                                           ceammc_callback_notify cb_notify);

/// do http post request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
bool ceammc_http_client_post(ceammc_http_client *cli,
                             const char *url,
                             const ceammc_http_client_param *param,
                             size_t param_len);

/// process all results that are ready
/// @param cli - http client pointer
/// @return true on success, false on error
bool ceammc_http_client_process(ceammc_http_client *cli);

/// upload file with POST request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param file - filename
/// @param file_key - file part name in multipart data
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
bool ceammc_http_client_upload(ceammc_http_client *cli,
                               const char *url,
                               const char *file,
                               const char *file_key,
                               const ceammc_http_client_param *params,
                               size_t params_len);

/// free mqtt client
/// @param cli - mqtt client (can be NULL)
void ceammc_mqtt_client_free(ceammc_mqtt_client *cli);

/// create new mqtt client
/// @param url - mqtt broker url in format mqtt://host:port
/// @param id - client id (can be NULL)
/// @param cb - callbacks
/// @return pointer to mqtt_client (must be freed by ceammc_mqtt_client_free()) on success
///         or NULL on error
ceammc_mqtt_client *ceammc_mqtt_client_new(ceammc_mqtt_client_init params,
                                           ceammc_callback_msg cb_err,
                                           ceammc_callback_msg cb_post,
                                           ceammc_callback_msg cb_debug,
                                           ceammc_callback_msg cb_log,
                                           ceammc_callback_progress _cb_progress,
                                           ceammc_mqtt_client_result_cb cb_reply,
                                           ceammc_callback_notify cb_notify);

/// iterate mqtt events
/// @param cli - mqtt client pointer
/// @return true on success
bool ceammc_mqtt_client_process(ceammc_mqtt_client *cli);

/// publish text message into mqtt topic
/// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
/// @param msg - mqtt message
/// @param qos - Quality of Service flag
/// @param retain - This flag tells the broker to store the message for a topic
///        and ensures any new client subscribing to that topic will receive the stored message.
/// @return true on success
bool ceammc_mqtt_client_publish(ceammc_mqtt_client *cli,
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
/// @return true on success
bool ceammc_mqtt_client_publish_data(ceammc_mqtt_client *cli,
                                     const char *topic,
                                     const uint8_t *data,
                                     size_t len,
                                     ceammc_mqtt_qos qos,
                                     bool retain);

/// subscribe to mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return true on success
bool ceammc_mqtt_client_subscribe(ceammc_mqtt_client *cli, const char *topic, ceammc_mqtt_qos qos);

/// unsubscribe from mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return true on success
bool ceammc_mqtt_client_unsubscribe(ceammc_mqtt_client *cli, const char *topic);

/// free telegram bot
/// @param cli - pointer to telegram bot
void ceammc_telegram_bot_free(ceammc_telegram_bot_client *cli);

/// get file from telegram bot
/// @param cli - pointer to telegram bot
bool ceammc_telegram_bot_getfile(ceammc_telegram_bot_client *cli, const char *file_id);

/// logout telegram bot
/// @param cli - pointer to telegram bot
bool ceammc_telegram_bot_logout(ceammc_telegram_bot_client *cli);

/// create new telegram bot
/// @param params - telegram init settings
/// @param cb_err - callback called on error
/// @param cb_post - callback called on post
/// @param cb_debug - callback called on debug
/// @param cb_log - callback called on log
/// @param _cb_progress - not used
/// @param cb_reply - reply callback
/// @param cb_notify - callback called to notify that it's time to check results
/// @return pointer to telegram bot handle or NULL on error
ceammc_telegram_bot_client *ceammc_telegram_bot_new(ceammc_telegram_bot_init params,
                                                    ceammc_callback_msg cb_err,
                                                    ceammc_callback_msg cb_post,
                                                    ceammc_callback_msg cb_debug,
                                                    ceammc_callback_msg cb_log,
                                                    ceammc_callback_progress _cb_progress,
                                                    ceammc_telegram_bot_result_cb cb_reply,
                                                    ceammc_callback_notify cb_notify);

/// process telegram events
/// @param cli - pointer to telegram bot
bool ceammc_telegram_bot_process(ceammc_telegram_bot_client *cli);

/// send audio from telegram bot
/// @param cli - pointer to telegram bot
bool ceammc_telegram_bot_send_audio(ceammc_telegram_bot_client *cli,
                                    int64_t chat_id,
                                    const char *file);

/// send text message from telegram bot
/// @param cli - pointer to telegram bot
/// @param chat_id - target chat id
/// @param msg_id - reply message id
/// @param text - message text
/// @return true on success
bool ceammc_telegram_bot_send_message(ceammc_telegram_bot_client *cli,
                                      int64_t chat_id,
                                      int32_t msg_id,
                                      const char *text);

/// send text message from telegram bot
/// @param cli - pointer to telegram bot
bool ceammc_telegram_bot_whoami(ceammc_telegram_bot_client *cli);

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
