#ifndef _PERSON_DETECTOR_
#define _PERSON_DETECTOR_

#include "DetectionResultData.h"
#include "opencv2/core/mat.hpp"

/**
 * Interface for a person detector.
 *
 * @author Vitor Greati
 * */
class PersonDetector {

    public:

        virtual ~PersonDetector() {}

        /**
         * Performs person detection given an image.
         *
         * @param image     image to be analysed
         * @param results   detection results 
         * */
        virtual void detect(const cv::Mat image, std::vector<DetectionResultData>& results) const = 0;

};

#endif
