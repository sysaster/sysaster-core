#ifndef _RASP_SOURCE_
#define _RASP_SOURCE_

#include "sysaster/core/imgsource/ImageSource.h"

class RaspcamImageSource : public ImageSource {

    public:

        void get(cv::Mat& image) const override {
            std::cout << "called img get" << std::endl;
        }
};

#endif
