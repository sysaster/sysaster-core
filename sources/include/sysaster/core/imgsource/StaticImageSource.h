#ifndef _STATIC_IMG_SOURCE_
#define _STATIC_IMG_SOURCE_

#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"

/**
 * An image source which returns a single image.
 * Used for testing purposes only.
 *
 * @author Vitor Greati
 * */
class StaticImageSource : public ImageSource {

    private:

        cv::Mat img;

    public:

        StaticImageSource(const std::string& img_path) 
            : img {cv::imread(img_path, cv::IMREAD_COLOR)} {/*empty*/}

        /**
         * Asks for an image.
         *
         * @param image a reference to the image
         * */
        bool get(cv::Mat& image) {
            std::cout << "asked for image" << std::endl;
            if (!img.data)
                return false;
            image = img; 
            return true;
        }

};

#endif
