#include "gtest/gtest.h"
#include "sysaster/extra/tello/TelloImageSource.h"
#include <chrono>
#include <thread>

namespace {

    TEST(TelloPython, TestInitialize) {
        TelloImageSource tello {"", 8889, "192.168.10.1"};
        cv::Mat mat;
        tello.get(mat);
    }

    TEST(TelloPython, TestCaptureImage) {
    }
}
