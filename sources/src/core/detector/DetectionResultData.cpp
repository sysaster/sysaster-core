#include "sysaster/core/detector/DetectionResultData.h"
#include "extern/json.hpp"
#include "sysaster/common.h"

using nlohmann::json;

void to_json(json& j, const DetectionResultData& d) {
    j = json{
            {"bounding_box", 
                {
                    {"x", d.x},
                    {"y", d.y},
                    {"w", d.width},
                    {"h", d.height},
                    {"c", d.channels}
                }
            },
            {"time_ms", d.time_ms},
            {"latitude", std::to_string(d.latitude)},
            {"longitude", std::to_string(d.longitude)},
        };

    if (d.clipped_image != nullptr) {
        std::string clip = "";
        for (int i {0}; i < d.clipped_image_size; ++i)
            clip += d.clipped_image[i];
        j["clipped_image"] = clip;

        delete[] d.clipped_image;
    }
    j["topic"] = sysaster::settings->topic;
    j["id_frame"] = d.id_frame;
}

void from_json(const json& j, DetectionResultData& d) { /*TODO*/ }

std::string DetectionResultData::to_json() const {
    json j = *this;
    return j.dump();
}
