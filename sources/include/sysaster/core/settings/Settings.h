#ifndef _SETTINGS_
#define _SETTINGS_

#include <map>
#include <string>

/**
 * Module running settings.
 * */
struct Settings {
    enum ImgSourceType {
        STATIC=0,
        RASPCAM=1,
        TELLO=2
    };

    enum DetectorType {
        YOLOV3=0,
    };

    ImgSourceType img_source_type; 
    DetectorType detector_type; 
    int detection_pool_size {1};
    int connection_pool_size {1};
    std::string server_url;
    std::string server_endpoint;
    std::string image_source_url;
    int image_source_interval {1};
    std::string topic;
    bool sync = false;

    std::map<std::string, std::string> extra;
};

#endif
