# include "parser_osc.h"
# include "ceammc_string_types.h"

%%{

# char* lo_url_get_port(const char* url)
# {
#     char* port = (char*)malloc(strlen(url));
#
#     if (sscanf(url, "osc://%*[^:]:%[0-9]", port) > 0) {
#         return port;
#     }
#     if (sscanf(url, "osc.%*[^:]://%*[^:]:%[0-9]", port) > 0) {
#         return port;
#     }
#     if (sscanf(url, "osc://[%*[^]]]:%[0-9]", port) > 0) {
#         return port;
#     }
#     if (sscanf(url, "osc.%*[^:]://[%*[^]]]:%[0-9]", port) > 0) {
#         return port;
#     }
#     if (sscanf(url, "osc://:%[0-9]", port) > 0) {
#         return port;
#     }
#     if (sscanf(url, "osc.%*[^:]://:%[0-9]", port) > 0) {
#         return port;
#     }
#
#     /* doesnt look like an OSC URL with port number */
#     free(port);
#
#     return NULL;
# }

    machine parser_osc;

    nz_any = any - 0;
    net_proto = (
                    'tcp'    %{ rl_proto = gensym("tcp"); }
                    | 'udp'  %{ rl_proto = gensym("udp"); }
                );
    unix_proto = '.unix' %{ rl_proto = gensym("unix"); };
    host  = (nz_any - (']' | ':'))+ ${ rl_host.push_back(fc); };
    port  = ':' ([0-9]+ ${ (rl_port *= 10) += (fc - '0'); });
    net_url   = 'osc' ('.' net_proto)? '://' host? port;

    path = nz_any+ ${ rl_path.push_back(fc); };
    socket_url = 'osc' unix_proto? '://' path;
    proto_url = net_proto port;
    url = net_url | socket_url | proto_url;

    main := url 0 @{ fbreak; };

    write data;
}%%


namespace ceammc {
namespace parser {

    bool parse_osc_url(const char* str, t_symbol*& proto, t_symbol*& host, int& port, t_symbol*& unix_path) {
        int cs = 0;
        const char* p = str;
        string::SmallString rl_host;
        string::SmallString rl_path;
        int rl_port = 0;
        t_symbol* rl_proto = nullptr;

        %% write init;
        %% write exec noend;

        if (cs >= %%{ write first_final; }%%) {
            rl_host.push_back('\0');
            proto = (rl_proto != nullptr) ? rl_proto : gensym("udp");
            host = gensym(rl_host.data());
            port = rl_port;
            unix_path = gensym(rl_path.data());
            return true;
        } else
            return false;
    }
}
}

