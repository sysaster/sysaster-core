#include <iostream>
#include "sysaster/core.h"
#include "sysaster/extra/raspberry/RaspcamImageSource.h"
#include "sysaster/extra/tello/TelloImageSource.h"
#include "sysaster/core/imgsource/StaticImageSource.h"
#include "sysaster/extra/yolo/YOLOv3PersonDetector.h"
#include "sysaster/core/dao/SQLite3DetectionDAO.h"

int main(int argn, char* args[]) {

    if (argn < 2) {
        std::cout << "[sysaster ERROR] you must provide a settings file" << std::endl;
        return 1;
    }

    std::string file_path = std::string(args[1]);

    //> Parse settings
    JSONSettingsParser parser;
    parser.parse(file_path, *sysaster::settings);
 
    std::cout << "[sysaster INFO] Finished parser" << std::endl;

    //> Instantiate ImageSource
    std::shared_ptr<ImageSource> imgSource = nullptr;

    if (sysaster::settings->img_source_type == Settings::ImgSourceType::STATIC)
        imgSource = std::make_shared<StaticImageSource>(sysaster::settings->extra["image_source_url"]);
    else if (sysaster::settings->img_source_type == Settings::ImgSourceType::RASPCAM)
        imgSource = std::make_shared<RaspcamImageSource>(sysaster::settings->extra["image_source_url"]);
    else if (sysaster::settings->img_source_type == Settings::ImgSourceType::TELLO)
        imgSource = std::make_shared<TelloImageSource>(
                sysaster::settings->extra["user_ip"],
                std::stoi(sysaster::settings->extra["user_port"]),
                sysaster::settings->extra["tello_ip"],
                std::stoi(sysaster::settings->extra["tello_port"]),
                false, 
                std::stof(sysaster::settings->extra["response_timeout"])
                );
    
    //> Instantiate SQLite DAO
    sysaster::detection_dao = std::make_shared<SQLite3DetectionDAO>("sysaster.db");
    
    //> Instantiate ConnectionThreadDispatcher
    sysaster::connection_dispatcher = std::make_shared<ConnectionThreadDispatcher>();

    //> Instantiate ImageThreadDispatcher
    std::shared_ptr<ImageThreadDispatcher> imgThreDispat = std::make_shared<ImageThreadDispatcher>();

    //> Instantiate ImageRequestor
    std::shared_ptr<ImageRequestor> imgRequestor = std::make_shared<ImageRequestor>(imgSource, imgThreDispat);

    //> Instantiate PersonDetector
    if (sysaster::settings->detector_type == Settings::DetectorType::YOLOV3) {
        sysaster::person_detector = std::make_shared<YOLOv3PersonDetector>(
            sysaster::settings->extra["file_class_names"],
            sysaster::settings->extra["file_config"],
            sysaster::settings->extra["file_weights"]
        );
    }

    if (sysaster::person_detector == nullptr || sysaster::person_detector->is_errored())
        std::cout << "[sysaster ERROR] error when loading detector" << std::endl;

    //> Start connection dispatcher thread
    std::thread conn_disp_thread {std::ref(*sysaster::connection_dispatcher)};

    //> Start image dispatcher thread
    std::thread img_thre_dispatcher {std::ref(*imgThreDispat)};

    //>> Start the module
    imgRequestor->require();

    std::cout << "[sysaster BYE] Finish sysaster." << std::endl;

    return 0;
}
