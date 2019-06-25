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
        std::string img_path;

    protected:

        bool establish() override {
            img = cv::imread(img_path, cv::IMREAD_COLOR);
            return img.data;
        }

        bool get_image(cv::Mat& image) override {
            image = img.clone(); 
            return true;
        }

    public:

        StaticImageSource(const std::string& img_path) : img_path {img_path} {}
};

#endif
