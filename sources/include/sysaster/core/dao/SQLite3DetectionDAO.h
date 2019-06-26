#ifndef _SQLITE_DETECTION_DAO_
#define _SQLITE_DETECTION_DAO_

#include <sqlite3.h>
#include "sysaster/core/dao/DetectionDAO.h"
#include <stdexcept>

/**
 * Data access object for detection 
 * data in SQLite3 database.
 *
 * @author Vitor Greati
 * */
class SQLite3DetectionDAO : public DetectionDAO {

    private:

        sqlite3* db;

    public:

        SQLite3DetectionDAO(const std::string& dbpath);

        /**
         * Insert a detection result in the database.
         *
         * @param detection the detection to be inserted
         * @return the status of the operation
         * */
        int insert_detection(const DetectionResultData& detection) override;

        /**
         * List detection data.
         *
         * @param detections vector of detections
         * @param stored_only list only the stored-only ones
         * @return the numbert of detections found
         * */
        int list_detections(std::vector<DetectionResultData>& detections) override;

        /**
         * Remove listed detections.
         *
         * @param listed detections
         * @return the number of rows affected
         * */
        int remove_detections(const std::vector<DetectionResultData> detections) override;

};

#endif
