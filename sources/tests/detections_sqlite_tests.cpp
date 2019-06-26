#include "gtest/gtest.h"
#include <chrono>
#include "sysaster/core/dao/SQLite3DetectionDAO.h"
#include <string>

namespace {

    TEST(DetectionsSQLite, CreateTable) {

        SQLite3DetectionDAO dao {"~/sysaster.db"};

    }

}
