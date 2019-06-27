#ifndef _IMG_REQUESTOR_
#define _IMG_REQUESTOR_

#include "ImageThreadDispatcher.h"
#include "ImageSource.h"
#include "sysaster/common.h"
#include "restclient-cpp/restclient.h"
#include <chrono>

/**
 * Main loop for requiring images.
 *
 * @author Vitor Greati
 * */
class ImageRequestor {

    private:

        std::shared_ptr<ImageSource> image_source;

        std::shared_ptr<ImageThreadDispatcher> image_dispatcher;

        std::shared_ptr<Settings> settings = sysaster::settings;

    public:

        ImageRequestor(const std::shared_ptr<ImageSource> image_source,
                const std::shared_ptr<ImageThreadDispatcher> image_dispatcher) 
            : image_source {image_source}, image_dispatcher {image_dispatcher}
        { /* empty */ }

        /**
         * Main loop.
         * */
        void require() const {
            while(true) {
                cv::Mat img;
                if (image_source->get(img)) {
                    std::cout << "[sysaster INFO] got a new image" << std::endl;
                    //image_dispatcher->require_detection(img);
                    std::vector<DetectionResultData> results;
                    sysaster::person_detector->detect(img, results);
                    for (auto& res : results) {
                        RestClient::Response r = RestClient::post(
                                sysaster::settings->server_url + sysaster::settings->server_endpoint, 
                                "application/json", res.to_json());
                    }
		        } else 
                    std::cout << "[sysaster ERROR] fail to retrieve a new image" << std::endl;
	            std::this_thread::sleep_for (std::chrono::seconds(settings->image_source_interval));
            }
        }
};

#endif
