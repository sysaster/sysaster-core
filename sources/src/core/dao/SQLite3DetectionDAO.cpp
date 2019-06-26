#include "sysaster/core/dao/SQLite3DetectionDAO.h"

SQLite3DetectionDAO::SQLite3DetectionDAO(const std::string& dbpath) {
    int res = sqlite3_open(dbpath.c_str(), &db);

    if (res) {
        sqlite3_close(db);
        throw std::runtime_error("Unable to open the sqlite database at " + dbpath + ", err  " + std::to_string(res));
    }

    char *err = NULL;
    std::string create_table_stmt = "CREATE TABLE IF NOT EXISTS detections ("
            "id INTEGER PRIMARY KEY,"
            "id_frame INTEGER,"
            "longitude REAL,"
            "latitude REAL,"
            "bbox_x INT,"
            "bbox_y INT,"
            "bbox_w INT,"
            "bbox_h INT,"
            "bbox_c INT,"
            "confidence REAL,"
            "confidence REAL,"
            "time_ms INT,"
            "crop_b64 TEXT,"
            "crop_b64_size INT"
            ");";
    sqlite3_exec(db, create_table_stmt.c_str(), NULL, NULL, &err);
    if (err != NULL) {
        // Emit a warning
        delete err;
    }

}

int SQLite3DetectionDAO::insert_detection(const DetectionResultData& detection) {

    

}

int SQLite3DetectionDAO::list_detections(std::vector<DetectionResultData>& detections) {

}

int SQLite3DetectionDAO::remove_detections(const std::vector<DetectionResultData> detections) {

}

