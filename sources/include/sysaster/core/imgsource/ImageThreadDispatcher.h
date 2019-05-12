#ifndef _IMG_THREAD_DISP_
#define _IMG_THREAD_DISP_

#include "opencv2/core/mat.hpp"
#include "CdsImageData.h"
#include "extern/ctpl_stl.h"
#include "sysaster/core/detector/DetectionThread.h"

/**
 * Enqueue images and dispatch them
 * for detection when possible.
 *
 * @author Vitor Greati
 * */
class ImageThreadDispatcher {

    private:

        ImageQueueT imageQueue;
        ctpl::thread_pool detectorPool {10};

    public:

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
            while(true) {
               if (imageQueue.size() == 0)
                    continue;
               else {
                    auto img = imageQueue.pop()->get_data();
                    detectorPool.push(DetectionThread{nullptr, nullptr}, std::ref(img));
               }
            } 
        }
};

#endif
