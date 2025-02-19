#ifndef ceammc_rust_system_h
#define ceammc_rust_system_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


enum class ceammc_system_process_mode {
    Bytes,
    Lines,
    Fudi,
    String,
};

enum class ceammc_system_process_state {
    Error,
    Running,
    Ready,
};

struct ceammc_system_info;

struct ceammc_system_process;

struct ceammc_sysinfo_temp_cb {
    void *user;
    void (*cb)(void *user, const char *label, float cpu_temp);
};

struct ceammc_sysinfo_cpu_cb {
    void *user;
    void (*cb)(void *user, int n, int freq, float usage, const char *name, const char *brand, const char *vendor);
};

struct ceammc_system_notify_cb {
    size_t client_id;
    void (*cb)(size_t client_id);
};

struct ceammc_system_process_cmd {
    const char *prog;
    const char *const *argv;
    size_t argc;
};


extern "C" {

ceammc_system_info *ceammc_sysinfo_create(ceammc_sysinfo_temp_cb cb_temp,
                                          ceammc_sysinfo_cpu_cb cb_cpu,
                                          ceammc_system_notify_cb cb_notify);

void ceammc_sysinfo_free(ceammc_system_info *sysinfo);

bool ceammc_sysinfo_get_cpu(ceammc_system_info *sysinfo);

bool ceammc_sysinfo_get_temperature(ceammc_system_info *sysinfo);

bool ceammc_sysinfo_process(ceammc_system_info *sysinfo);

/// clear current system command (kill if running)
/// @param proc - system command pointer
bool ceammc_system_process_clear(ceammc_system_process *proc);

/// run system command
/// @param proc - system command pointer
/// @return true on success, false on error (just success command creation status)
bool ceammc_system_process_exec(ceammc_system_process *proc);

/// free system command process (kill if running)
/// @param proc - system command pointer
void ceammc_system_process_free(ceammc_system_process *proc);

/// create new system command
/// @param cmd - pointer to array of commands
/// @param cmd_len - number of commands in array
/// @param mode - output processing mode
/// @param capture_stdout - do stdout capture
/// @param capture_stderr - to stderr capture
/// @param pwd - process working directory
/// @param stdin_data - data for stdin input
/// @param user - callback user pointer
/// @param on_err - on error callback (in current thread)
/// @param on_stdout_data - on stdout data callback (current thread)
/// @param on_stderr_data - on stdout data callback (current thread)
/// @return pointer to system command or NULL on error
ceammc_system_process *ceammc_system_process_new(const ceammc_system_process_cmd *cmd,
                                                 size_t cmd_len,
                                                 ceammc_system_process_mode mode,
                                                 bool capture_stdout,
                                                 bool capture_stderr,
                                                 const char *pwd,
                                                 const char *stdin_data,
                                                 void *user,
                                                 void (*on_err)(void *user, const char *msg),
                                                 void (*on_stdout_data)(void *user, const uint8_t *data, size_t len),
                                                 void (*on_stderr_data)(void *user, const uint8_t *data, size_t len));

/// process system command results
/// @param proc - system command pointer
/// @param result_code - pointer to command result code
/// @return system command state
ceammc_system_process_state ceammc_system_process_results(ceammc_system_process *proc,
                                                          int32_t *result_code);

/// terminate system command (SIGKILL on unix)
/// @param proc - system command pointer
/// @return true on success, false on error
bool ceammc_system_process_terminate(ceammc_system_process *proc);

} // extern "C"

#endif // ceammc_rust_system_h
