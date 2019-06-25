#ifndef _IMG_SOURCE_
#define _IMG_SOURCE_

#include <opencv2/core/mat.hpp>

/**
 * Represents an image source, like a camera.
 *
 * @author Vitor Greati
 * */
class ImageSource {

    protected:

        bool ready {false};

        virtual bool establish() = 0;

        virtual bool get_image(cv::Mat& image) = 0;

    public:

        /**
         * Asks for an image.
         *
         * @param image a reference to the image
         * */
        virtual bool get(cv::Mat& image) {
            if (!this->ready)
                this->ready = this->establish();

            if (this->ready)
                return this->get_image(image);

            return false;
        }

        /**
         * Indicate if the image source is ready.
         * */
        inline bool is_ready() { return ready; }


};

#endif
