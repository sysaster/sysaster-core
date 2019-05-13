#ifndef _IMG_THREAD_DISP_
#define _IMG_THREAD_DISP_

#include "opencv2/core/mat.hpp"
#include "CdsImageData.h"
#include "extern/ctpl_stl.h"
#include "sysaster/core/detector/DetectionThread.h"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"

/**
 * Enqueue images and dispatch them
 * for detection when possible.
 *
 * @author Vitor Greati
 * */
class ImageThreadDispatcher {

    private:

        std::shared_ptr<Settings> settings = sysaster::settings;

        ImageQueueT imageQueue;
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
            ImageNode imgnode {img};
            imageQueue.enqueue(imgnode); 
        }

        void operator()() {
            cds::threading::Manager::attachThread();
            while(true) {
               if (imageQueue.empty()) {
                    continue;
               } else {
                    std::cout << "a" << std::endl;
                    if(imageQueue.dequeue() != nullptr);//->get_data();
                    //cv::Mat img {imageQueue.pop()->get_data()};
                    //detectorPool.push(DetectionThread{
                    //        sysaster::person_detector, 
                    //        sysaster::connection_dispatcher
                    //}, img);
               }
            } 
            cds::threading::Manager::detachThread(); 
        }
};

#endif
