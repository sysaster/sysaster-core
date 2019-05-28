#ifndef _SET_JSON_PARSER_
#define _SET_JSON_PARSER_

#include <fstream>
#include "extern/json.hpp"
#include "Settings.h"

using nlohmann::json;

void from_json(const json& j, Settings& s) {

    //> Image source
    auto img_source_type = j["image_source"]["type"];

    if (img_source_type == "static") {
        s.img_source_type = Settings::ImgSourceType::STATIC;
        s.extra.insert({"image_source_url", j["image_source"]["url"]});
    } else if (img_source_type == "raspcam") {
        s.img_source_type = Settings::ImgSourceType::RASPCAM;
        s.extra.insert({"image_source_url", j["image_source"]["url"]});
    } else if (img_source_type == "tello") {
        s.img_source_type = Settings::ImgSourceType::TELLO;
        s.extra.insert({"image_source_url", j["image_source"]["url"]});
    }

    s.image_source_interval = j["image_source"]["interval"];

    //> Detector
    auto detector_type = j["detection"]["type"];

    if (detector_type == "yolov3") {
        s.detector_type = Settings::DetectorType::YOLOV3;
        s.extra.insert({"file_config", j["detection"]["file_config"]});
        s.extra.insert({"file_weights", j["detection"]["file_weights"]});
        s.extra.insert({"file_class_names", j["detection"]["file_class_names"]});
    }
    s.detection_pool_size = j["detection"]["pool_size"];

    //> Connection
    s.connection_pool_size = j["connection"]["pool_size"];
    s.server_endpoint = j["connection"]["server_endpoint"];
}

class JSONSettingsParser {

    public:

        void parse(const std::string& file_path, Settings& settings) const {

            std::ifstream i {file_path};

            json j;

            i >> j;

            settings = j.get<Settings>();
        }
};

#endif
