#ifndef _DETECT_RES_DATA_
#define _DETECT_RES_DATA_

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "extern/json.hpp"

/**
 * Represents the detection results.
 *
 * @author Vitor Greati
 * */
using nlohmann::json;

struct DetectionResultData {
    
    float confidence;
    float x;
    float y;
    float width {3};
    float height {3};
    float latitude;
    float longitude;
    long time_ms;
    char * clipped_image;//[900];
    int clipped_image_size;

    DetectionResultData() {}

    DetectionResultData(const cv::Mat& clip) {}

    std::string to_json() const;
};

void to_json(json& j, const DetectionResultData& d) {
    j = json{
            {"bounding_box", 
                {
                    {"x", d.x},
                    {"y", d.y},
                    {"w", d.width},
                    {"h", d.height}
                }
            },
            {"time_ms", d.time_ms},
            {"latitude", d.latitude},
            {"longitude", d.longitude},
        };

    if (d.clipped_image != nullptr) {
        std::string clip = "";
        for (int i {0}; i < d.clipped_image_size; ++i)
            clip += d.clipped_image[i];
        j["clipped_image"] = clip;

        delete[] d.clipped_image;
    }
}

void from_json(const json& j, DetectionResultData& d) { /*TODO*/ }

std::string DetectionResultData::to_json() const {
    json j = *this;
    return j.dump();
}
#endif
