#ifndef _DETECT_RES_DATA_
#define _DETECT_RES_DATA_

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"

/**
 * Represents the detection results.
 *
 * @author Vitor Greati
 * */
struct DetectionResultData {
    
    long id_frame;
    float confidence;
    float x;
    float y;
    float width {3};
    float height {3};
    int channels {1};
    float latitude;
    float longitude;
    long time_ms;
    char * clipped_image;//[900];
    int clipped_image_size;

    DetectionResultData() {}

    DetectionResultData(const cv::Mat& clip) {}

    std::string to_json() const;
};

#endif
