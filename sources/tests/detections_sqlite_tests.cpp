#include "gtest/gtest.h"
#include <chrono>
#include "sysaster/core/dao/SQLite3DetectionDAO.h"
#include "sysaster/core/detector/DetectionResultData.h"
#include <string>

namespace {

    TEST(DetectionsSQLite, CreateTable) {

        SQLite3DetectionDAO dao {"sysaster.db"};

    }

    TEST(DetectionsSQLite, InsertDetection) {
        SQLite3DetectionDAO dao {"sysaster.db"};
        DetectionResultData r;
        r.id_frame = 12;
        r.confidence = 0.95;
        r.x = 123;
        r.y = 12;
        r.width = 200;
        r.height = 200;
        r.channels = 1;
        r.latitude = 123.2;
        r.longitude = 12.2;
        r.time_ms = 12;
        r.clipped_image = new char[20];
        r.clipped_image_size = 20;
        dao.insert_detection(r);
    }

}
