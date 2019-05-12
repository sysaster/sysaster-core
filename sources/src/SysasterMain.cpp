#include <iostream>
#include "sysaster/core.h"

int main(int argn, char* args[]) {

    if (argn < 2) {
        std::cout << "[sysaster ERROR] you must provide a settings file";
        return 1;
    }

    std::string file_path = std::string(args[1]);

    //> Parse settings
    JSONSettingsParser parser;
    parser.parse(file_path, *sysaster::settings);

    //> Instantiate ImageSource
    std::shared_ptr<ImageSource> imgSource {nullptr};

    //> Instantiate ConnectionThreadDispatcher
    sysaster::connection_dispatcher = std::make_shared<ConnectionThreadDispatcher>();

    //> Instantiate ImageThreadDispatcher
    std::shared_ptr<ImageThreadDispatcher> imgThreDispat = std::make_shared<ImageThreadDispatcher>();

    //> Instantiate ImageRequestor
    std::shared_ptr<ImageRequestor> imgRequestor = std::make_shared<ImageRequestor>(imgSource, imgThreDispat);

    //> Instantiate PersonDetector
    sysaster::person_detector = nullptr;

    //>> Start the module
    imgRequestor->require();

    std::cout << "Finish sysaster." << std::endl;

    return 0;
}
