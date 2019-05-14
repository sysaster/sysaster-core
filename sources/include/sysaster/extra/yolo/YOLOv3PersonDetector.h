#ifndef _YOLO_PERSON_DETECTOR_
#define _YOLO_PERSON_DETECTOR_

#include "sysaster/core/detector/PersonDetector.h"
#include "sysaster/core/detector/DetectionResultData.h"
#include "opencv2/imgcodecs.hpp"

/**
 * Interface for a person detector.
 *
 * @author Vitor Greati
 * */
class YOLOv3PersonDetector : public PersonDetector {

    public:

        /**
         * Performs person detection given an image.
         *
         * @param image     image to be analysed
         * @param results   detection results 
         * */
        void detect(const cv::Mat image, std::vector<DetectionResultData>& results) const override {
            DetectionResultData r;
            r.height = image.rows; 
            r.width = image.cols; 

            std::vector<uchar> buf;
            cv::imencode(".jpg", image, buf);
            memcpy(r.clipped_image, buf.data(), buf.size());
            results.push_back(r);
        }

};

#endif

