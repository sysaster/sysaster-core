#ifndef _RASP_SOURCE_
#define _RASP_SOURCE_

#include "sysaster/core/imgsource/ImageSource.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/videoio.hpp"
#include "sysaster/common.h"

class RaspcamImageSource : public ImageSource {


    private:
        cv::VideoCapture videocapture;
        std::string url;

    protected:

        bool establish() override {
            videocapture = cv::VideoCapture(url);
	    if (videocapture.isOpened()) {
            std::cout << videocapture.get(5) << std::endl;
            videocapture.set(38, 1);
	    }
            return videocapture.isOpened();
        }

        bool get_image(cv::Mat& image) override {
            cv::Mat img;
            bool r = videocapture.read(img);
            if (img.empty()) return false;
            image = img.clone();
            return r;
        }

    public:

        RaspcamImageSource(const std::string& url) : url {url} {}
};

#endif
