#include <iostream>
#include "sysaster/core.h"
#include "sysaster/extra/raspberry/RaspcamImageSource.h"
#include "sysaster/extra/yolo/YOLOv3PersonDetector.h"

int main(int argn, char* args[]) {

    if (argn < 2) {
        std::cout << "[sysaster ERROR] you must provide a settings file" << std::endl;
        return 1;
    }

    std::string file_path = std::string(args[1]);

    //> Parse settings
    JSONSettingsParser parser;
    parser.parse(file_path, *sysaster::settings);

    //> Instantiate ImageSource
    std::shared_ptr<ImageSource> imgSource = std::make_shared<RaspcamImageSource>(sysaster::settings->image_source_url);

    //> Instantiate ConnectionThreadDispatcher
    sysaster::connection_dispatcher = std::make_shared<ConnectionThreadDispatcher>();

    //> Instantiate ImageThreadDispatcher
    std::shared_ptr<ImageThreadDispatcher> imgThreDispat = std::make_shared<ImageThreadDispatcher>();

    //> Instantiate ImageRequestor
    std::shared_ptr<ImageRequestor> imgRequestor = std::make_shared<ImageRequestor>(imgSource, imgThreDispat);

    //> Instantiate PersonDetector
    auto file_class_names = "";
    auto file_config = "";
    auto file_weight = "";
    sysaster::person_detector = std::make_shared<YOLOv3PersonDetector>(file_class_names, file_config, file_weight);

    //> Start connection dispatcher thread
    std::thread conn_disp_thread {std::ref(*sysaster::connection_dispatcher)};
    //conn_disp_thread.detach();

    //> Start image dispatcher thread
    std::thread img_thre_dispatcher {std::ref(*imgThreDispat)};
    //img_thre_dispatcher.detach();

    //>> Start the module
    imgRequestor->require();

    std::cout << "Finish sysaster." << std::endl;

    return 0;
}
