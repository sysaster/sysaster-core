#ifndef _SET_JSON_PARSER_
#define _SET_JSON_PARSER_

#include <fstream>
#include "extern/json.hpp"
#include "Settings.h"

using nlohmann::json;

void from_json(const json& j, Settings& s) {
    s.detection_pool_size = j["detection"]["pool_size"];
    s.connection_pool_size = j["connection"]["pool_size"];
    s.server_port = j["connection"]["server_port"];
    //s.server_ip = j["connection"]["server_ip"];
    s.image_source_interval = j["image_source"]["interval"];
    s.server_endpoint = j["connection"]["server_endpoint"];
    s.image_source_url = j["image_source"]["url"];
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
