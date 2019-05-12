#ifndef _YOLO_PERSON_DETECTOR_
#define _YOLO_PERSON_DETECTOR_

#include "sysaster/core/detector/PersonDetector.h"

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
        void detect(const cv::Mat image, const std::vector<DetectionResultData>& results) const override {
            std::cout << "called yolov3 detector" << std::endl;
        }

};

#endif

