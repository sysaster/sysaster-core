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
    
    cv::Rect bounding_box;
    cv::Mat frame_clip;
    bool person_found {false};
    long time_ms;

    ~DetectionResultData() {}

    DetectionResultData();
    DetectionResultData(decltype(person_found));
    DetectionResultData(decltype(bounding_box));
    DetectionResultData(decltype(bounding_box), decltype(frame_clip));
    DetectionResultData(decltype(bounding_box), decltype(frame_clip), decltype(person_found));
};

void to_json(json& j, const DetectionResultData& d) {
    j = json{
        {"bounding_box", 
            {
                {"x", 0},
                {"y", 0}
            }
        },
    };
}

void from_json(const json& j, DetectionResultData& d) {

}

#endif
