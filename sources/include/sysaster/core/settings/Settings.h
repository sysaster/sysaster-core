#ifndef _SETTINGS_
#define _SETTINGS_

/**
 * Module running settings.
 * */
struct Settings {
    int detection_pool_size {1};
    int connection_pool_size {1};
    size_t server_port;
    std::string server_ip;
    int image_source_interval {1};
};

#endif
