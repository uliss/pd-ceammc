#ifndef ceammc_rust_proto_h
#define ceammc_rust_proto_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


struct ceammc_obs_client;

struct ceammc_obs_monitor;

struct ceammc_obs_monitor_list;

struct ceammc_obs_scene;

struct ceammc_obs_scene_item;

struct ceammc_obs_scene_item_list;

struct ceammc_obs_scene_list;

struct ceammc_obs_version;

struct ceammc_obs_init {
    const char *host;
    const char *password;
    uint16_t port;
};

struct ceammc_callback_msg {
    void *user;
    void (*cb)(void *user, const char *msg);
};

struct ceammc_obs_result_cb {
    /// user data pointer (can be NULL)
    void *user;
    /// version callback function (can be NULL)
    void (*cb_version)(void *user, const ceammc_obs_version *ver);
    /// scene list callback function (can be NULL)
    void (*cb_scene_list)(void *user, const ceammc_obs_scene_list *scl);
    /// scene itme list callback function (can be NULL)
    void (*cb_scene_item_list)(void *user, const ceammc_obs_scene_item_list *items);
    /// monitor list callback function (can be NULL)
    void (*cb_monitor_list)(void *user, const ceammc_obs_monitor_list *mons);
    /// current scene callback function (can be NULL)
    void (*cb_current_scene)(void *user, const char *name);
    /// connected/disconnected callback function (can be NULL)
    void (*cb_connected)(void *user, bool state);
};

struct ceammc_callback_notify {
    size_t id;
    void (*cb)(size_t id);
};


extern "C" {

/// create new empty OBS scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
bool ceammc_obs_create_scene(const ceammc_obs_client *cli, const char *name);

/// enable OSB scene item
/// @param cli - pointer to obs client
/// @param scene - scene name
/// @param idx - item index
bool ceammc_obs_enable_scene_item(const ceammc_obs_client *cli,
                                  const char *scene,
                                  size_t idx,
                                  bool enabled);

/// change to first OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_first_scene(const ceammc_obs_client *cli);

/// free OBS Studio client
/// @param cli - pointer to obs client
void ceammc_obs_free(ceammc_obs_client *cli);

/// get current OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_get_current_scene(const ceammc_obs_client *cli);

/// get scene item at specified position
/// @param itl - pointer to item list (not NULL!)
/// @param idx - item position
const ceammc_obs_scene_item *ceammc_obs_get_scene_item_at(const ceammc_obs_scene_item_list *itl,
                                                          size_t idx);

/// get number of scene items
/// @param itl - pointer to item list (not NULL!)
size_t ceammc_obs_get_scene_item_count(const ceammc_obs_scene_item_list *itl);

/// get scene item index
/// @param item - pointer to item (not NULL!)
uint32_t ceammc_obs_get_scene_item_index(const ceammc_obs_scene_item *item);

/// get scene item input kind
/// @param item - pointer to item (not NULL!)
const char *ceammc_obs_get_scene_item_input_kind(const ceammc_obs_scene_item *item);

/// get scene item name
/// @param item - pointer to item (not NULL!)
const char *ceammc_obs_get_scene_item_name(const ceammc_obs_scene_item *item);

/// get scene item type
/// @param item - pointer to item (not NULL!)
const char *ceammc_obs_get_scene_item_type(const ceammc_obs_scene_item *item);

/// send version request to OBS studio
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_get_version(const ceammc_obs_client *cli);

/// change to last OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_last_scene(const ceammc_obs_client *cli);

/// request list of OBS studio monitors
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_list_monitors(const ceammc_obs_client *cli);

/// list OSB scene items
/// @param cli - pointer to obs client
/// @param scene - scene name
bool ceammc_obs_list_scene_items(const ceammc_obs_client *cli, const char *name);

/// send version request to OBS studio
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_list_scenes(const ceammc_obs_client *cli);

/// get monitor at specified position
/// @param ml - pointer to monitor list (not NULL!)
/// @return pointer to monitor or nullptr if not found
const ceammc_obs_monitor *ceammc_obs_monitor_at(const ceammc_obs_monitor_list *ml, size_t idx);

/// get monitor list length
/// @param ml - pointer to monitor list (not NULL!)
size_t ceammc_obs_monitor_count(const ceammc_obs_monitor_list *ml);

/// get monitor geometry
/// @param m - pointer to monitor (not NULL!)
/// @param x - pointer to store x coord
/// @param y - pointer to store y coord
/// @param w - pointer to store monitor width
/// @param h - pointer to store monitor height
void ceammc_obs_monitor_geom(const ceammc_obs_monitor *m,
                             uint16_t *x,
                             uint16_t *y,
                             uint16_t *w,
                             uint16_t *h);

/// get monitor index
/// @param m - pointer to monitor (not NULL!)
uint32_t ceammc_obs_monitor_index(const ceammc_obs_monitor *m);

/// get monitor name
/// @param m - pointer to monitor (not NULL!)
const char *ceammc_obs_monitor_name(const ceammc_obs_monitor *m);

/// create OSB Studio client
/// @param params - connection params
/// @param cb_err - callback for error messages
/// @param cb_post - callback for post messages
/// @param cb_debug - callback for debug messages
/// @param cb_log - callback for log messages
/// @param cb_reply - reply callbacks
/// @param cb_notify - notification callback
/// @return pointer to obs client or NULL on error
ceammc_obs_client *ceammc_obs_new(ceammc_obs_init params,
                                  ceammc_callback_msg cb_err,
                                  ceammc_callback_msg cb_post,
                                  ceammc_callback_msg cb_debug,
                                  ceammc_callback_msg cb_log,
                                  ceammc_obs_result_cb cb_reply,
                                  ceammc_callback_notify cb_notify);

/// move to next OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_next_scene(const ceammc_obs_client *cli);

/// move to previous OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
bool ceammc_obs_prev_scene(const ceammc_obs_client *cli);

/// process all available results from OBS studio
/// @param cli - pointer to obs client client
/// @return true on success, false on error
bool ceammc_obs_process_events(ceammc_obs_client *cli);

/// remove OBS scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
bool ceammc_obs_remove_scene(const ceammc_obs_client *cli, const char *name);

/// remove OSB scene item
/// @param cli - pointer to obs client
/// @param scene - scene name
/// @param idx - item index
bool ceammc_obs_remove_scene_item(const ceammc_obs_client *cli, const char *scene, size_t idx);

/// get current scene
/// @param scenes - pointer to scenes (not NULL!)
const ceammc_obs_scene *ceammc_obs_scene_current(const ceammc_obs_scene_list *scl);

/// get scene list data
/// @param scenes - pointer to scenes (not NULL!)
const ceammc_obs_scene *ceammc_obs_scene_list_at(const ceammc_obs_scene_list *scl, size_t idx);

/// get scene list length
/// @param scenes - pointer to scenes (not NULL!)
size_t ceammc_obs_scene_list_length(const ceammc_obs_scene_list *scl);

/// get scene name as C-string
/// @param scene - pointer to scene (not NULL!)
const char *ceammc_obs_scene_name(const ceammc_obs_scene *scene);

/// get scene UUID as C-string
/// @param scene - pointer to scene (not NULL!)
const char *ceammc_obs_scene_uuid(const ceammc_obs_scene *scene);

/// set current scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
bool ceammc_obs_set_current_scene(ceammc_obs_client *cli, const char *name);

/// get OBS image format at
/// @param v - pointer to version struct
/// @param idx - image format index
const char *ceammc_obs_version_image_fmt_at(const ceammc_obs_version *v, size_t idx);

/// get OBS image format count
/// @param v - pointer to version struct
size_t ceammc_obs_version_image_fmt_num(const ceammc_obs_version *v);

/// get OBS platform
/// @param v - pointer to version struct
const char *ceammc_obs_version_platform(const ceammc_obs_version *v);

/// get OBS platform description
/// @param v - pointer to version struct
const char *ceammc_obs_version_platform_desc(const ceammc_obs_version *v);

/// get RPC OBS version
/// @param v - pointer to version struct
uint32_t ceammc_obs_version_rpc(const ceammc_obs_version *v);

/// get OBS version
/// @param v - pointer to version struct
/// @param major - pointer to store major version data
/// @param minor - pointer to store minor version data
/// @param patch - pointer to store patch version data
void ceammc_obs_version_server(const ceammc_obs_version *v,
                               uint64_t *major,
                               uint64_t *minor,
                               uint64_t *patch);

/// get OBS Web Socket version
/// @param v - pointer to version struct
/// @param major - pointer to store major version data
/// @param minor - pointer to store minor version data
/// @param patch - pointer to store patch version data
void ceammc_obs_version_websocket(const ceammc_obs_version *v,
                                  uint64_t *major,
                                  uint64_t *minor,
                                  uint64_t *patch);

/// init rust env_logger
/// logger config is done with a RUST_LOG env variable
void ceammc_proto_log_init();

} // extern "C"

#endif // ceammc_rust_proto_h
