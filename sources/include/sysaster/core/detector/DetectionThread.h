#ifndef _DETECTION_THREAD_
#define _DETECTION_THREAD_

#include "sysaster/core/detector/PersonDetector.h"
#include "sysaster/core/detector/DetectionResultData.h"
#include "sysaster/core/connection/ConnectionThreadDispatcher.h"

/**
 * Member that calls a detector
 * after taking an image.
 *
 * @author Vitor Greati
 * */
class DetectionThread {

    private:

        std::shared_ptr<PersonDetector> person_detector;                      /** Person detector instance */
        std::shared_ptr<ConnectionThreadDispatcher> connection_dispatcher;    /** Connection dispatcher */

    public:

        DetectionThread(const decltype(person_detector)& person_detector,
                const decltype(connection_dispatcher)& connection_dispatcher) 
            : person_detector {person_detector}, connection_dispatcher {connection_dispatcher} {}

        ~DetectionThread() {}

        /**
         * Performs detection and asks for sending the
         * data.
         *
         * @param image the image to be processed
         * */
        void operator()(int id, const cv::Mat& image) {
            std::vector<DetectionResultData> detection_results;
            person_detector->detect(image, detection_results); 
            connection_dispatcher->require_send(detection_results);
        }

};

#endif
