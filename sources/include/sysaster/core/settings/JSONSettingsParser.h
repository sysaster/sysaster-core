#ifndef _SET_JSON_PARSER_
#define _SET_JSON_PARSER_

#include "extern/json.hpp"
#include "Settings.h"

using nlohmann::json;

void from_json(const json& j, Settings& s) {
    s.detection_pool_size = j["detection"]["pool_size"];
    s.connection_pool_size = j["connection"]["pool_size"];
    s.server_port = j["connection"]["server_port"];
    s.server_ip = j["connection"]["server_ip"];
    s.image_source_interval = j["image_source"]["interval"];
}

class JSONSettingsParser {

    public:

        void parse(const std::string& text, Settings& settings) const {
            auto j = json::parse(text);
            settings = j.get<Settings>();
        }
};

#endif
