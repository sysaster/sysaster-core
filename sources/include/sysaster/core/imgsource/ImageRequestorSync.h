#ifndef _IMG_REQUESTOR_SYNC_
#define _IMG_REQUESTOR_SYNC_

#include "ImageSource.h"
#include "sysaster/common.h"
#include <chrono>

/**
 * Main loop for requiring images.
 *
 * @author Vitor Greati
 * */
class ImageRequestorSync : public ImageRequestor {

    private:

        std::shared_ptr<ImageSource> image_source;

        std::shared_ptr<Settings> settings = sysaster::settings;

    public:

        ImageRequestorSync(const std::shared_ptr<ImageSource> image_source) 
            : image_source {image_source}
        { /* empty */ }

        /**
         * Main loop.
         * */
        void require() const override {
            while(true) {
                cv::Mat img;
                if (image_source->get(img)) {
                    std::cout << "[sysaster INFO] syncly got a new image" << std::endl;
                    std::vector<DetectionResultData> results;
                    sysaster::person_detector->detect(img, results);
                    for (auto& res : results) {
                        RestClient::Response resp = RestClient::post(
                                sysaster::settings->server_url + sysaster::settings->server_endpoint, 
                                "application/json", res.to_json());
                        bool success = false;

                        if (resp.code == -1) {
                            std::cout << "[sysaster ERROR] fail to post detection data" << std::endl;
                        } else if (resp.code == 200) {
                            std::cout << "[sysaster SUCCESS] posted detection data" << std::endl;
                            success = true;
                        } else {
                            std::cout << "[sysaster ERROR] unknown REST connection behaviour" << std::endl;
                        }

                        if (!success) {
                            try {
                                if (sysaster::detection_dao != nullptr) {
                                    sysaster::detection_dao->insert_detection(res);
                                    std::cout << "[sysasterdb WARNING] data saved on local db" << std::endl;
                                } else {
                                    std::cout << "[sysasterdb ERROR] local db unavailable" << std::endl;
                                }
                            } catch (std::exception e) {
                                std::cout << e.what() << std::endl;
                            }
                        }
                    }
		        } else {
                    std::cout << "[sysaster ERROR] fail to retrieve a new image" << std::endl;
                }
                std::this_thread::sleep_for (std::chrono::seconds(settings->image_source_interval));
            }
        }
};

#endif
