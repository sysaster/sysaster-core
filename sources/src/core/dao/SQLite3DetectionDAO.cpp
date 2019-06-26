#include "sysaster/core/dao/SQLite3DetectionDAO.h"
#include <string>
#include "sysaster/core/detector/DetectionResultData.h"
#include <stdexcept>
#include "sysaster/helpers.h"

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

    std::string sql = "INSERT INTO detections ("
            "id_frame,"
            "longitude,"
            "latitude,"
            "bbox_x,"
            "bbox_y,"
            "bbox_w,"
            "bbox_h,"
            "bbox_c,"
            "confidence,"
            "time_ms,"
            "crop_b64,"
            "crop_b64_size"           
        ") VALUES (" +
           quotesql(std::to_string(detection.id_frame)) + "," +
           quotesql(std::to_string(detection.latitude)) +"," +
           quotesql(std::to_string(detection.longitude)) +"," +
           quotesql(std::to_string(detection.x)) +"," +
           quotesql(std::to_string(detection.y)) +"," +
           quotesql(std::to_string(detection.width)) +"," +
           quotesql(std::to_string(detection.height)) +"," +
           quotesql(std::to_string(detection.channels)) +"," +
           quotesql(std::to_string(detection.confidence)) +"," +
           quotesql(std::to_string(detection.time_ms)) +"," +
           quotesql(std::string(detection.clipped_image)) +"," +
           quotesql(std::to_string(detection.clipped_image_size)) +
        ");";

    int rc;
    char* err;

    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &err);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Fail to insert detection, err " + std::string(err));
        sqlite3_free(err);
    }
}

int SQLite3DetectionDAO::list_detections(std::vector<DetectionResultData>& detections) {

}

int SQLite3DetectionDAO::remove_detections(const std::vector<DetectionResultData> detections) {

}

