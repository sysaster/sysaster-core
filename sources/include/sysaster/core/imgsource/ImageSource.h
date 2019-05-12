#ifndef _IMG_SOURCE_
#define _IMG_SOURCE_

#include "opencv2/core/mat.hpp"

/**
 * Represents an image source, like a camera.
 *
 * @author Vitor Greati
 * */
class ImageSource {

    public:

        /**
         * Asks for an image.
         *
         * @param image a reference to the image
         * */
        virtual void get(cv::Mat& image) const = 0;

};

#endif
