#ifndef _SETTINGS_
#define _SETTINGS_

/**
 * Module running settings.
 * */
struct Settings {
    int detection_pool_size {1};
    int connection_pool_size {1};
    //size_t server_port;
    //std::string server_ip;
    std::string server_endpoint;
    std::string image_source_url;
    int image_source_interval {1};
};

#endif
