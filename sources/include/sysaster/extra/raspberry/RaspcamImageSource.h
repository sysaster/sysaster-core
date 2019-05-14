#ifndef _RASP_SOURCE_
#define _RASP_SOURCE_

#include "sysaster/core/imgsource/ImageSource.h"

class RaspcamImageSource : public ImageSource {

    public:

        void get(cv::Mat& image) const override {
            image = cv::Mat(30, 30, CV_16SC3);
        }
};

#endif
