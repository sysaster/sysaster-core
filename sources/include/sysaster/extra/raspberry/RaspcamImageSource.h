#ifndef _RASP_SOURCE_
#define _RASP_SOURCE_

#include "sysaster/core/imgsource/ImageSource.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "sysaster/common.h"

class RaspcamImageSource : public ImageSource {


    private:
        cv::VideoCapture videocapture;
        std::string url;

    public:

        RaspcamImageSource(const std::string& url) : url {url} {}

        bool get(cv::Mat& image) override {
            if (!videocapture.isOpened()) {
                videocapture = cv::VideoCapture(url);
            }
            cv::Mat img;
            bool r = videocapture.read(img);
            image = img.clone();
            return r;
        }
};

#endif
