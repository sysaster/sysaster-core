#ifndef _IMG_THREAD_DISP_
#define _IMG_THREAD_DISP_

#include "opencv2/core/mat.hpp"
#include "extern/ctpl_stl.h"
#include "sysaster/core/detector/DetectionThread.h"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"
#include <boost/lockfree/spsc_queue.hpp>

/**
 * Enqueue images and dispatch them
 * for detection when possible.
 *
 * @author Vitor Greati
 * */
class ImageThreadDispatcher {

    private:

        std::shared_ptr<Settings> settings = sysaster::settings;
        boost::lockfree::spsc_queue<cv::Mat> imageQueue {100};
        ctpl::thread_pool detectorPool { settings->detection_pool_size };

    public:

        ~ImageThreadDispatcher() {}

        ImageThreadDispatcher() {}

        /**
         * Require the detection for an
         * image.
         *
         * @param img the image
         * */
        void require_detection(const cv::Mat& img) {
            imageQueue.push(img);
        }

        /**
         * Main dispatcher loop. Creates detection
         * jobs.
         * */
        void operator()() {
            while(true) {
               if (imageQueue.empty()) {
                    continue;
               } else {
                    cv::Mat & img = imageQueue.front();
                    detectorPool.push(DetectionThread{
                            sysaster::person_detector, 
                            sysaster::connection_dispatcher
                    }, img);
                    imageQueue.pop();
               }
               std::this_thread::sleep_for (std::chrono::seconds(settings->image_source_interval));
            } 
        }
};

#endif
