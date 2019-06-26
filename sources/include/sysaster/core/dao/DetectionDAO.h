#ifndef _DETECTION_DAO_
#define _DETECTION_DAO_

#include <vector>

/**
 * Data access object for detection 
 * data.
 *
 * @author Vitor Greati
 * */
class DetectionResultData;
class DetectionDAO {

    public:

        /**
         * Insert a detection result in the database.
         *
         * @param detection the detection to be inserted
         * @return the status of the operation
         * */
        virtual int insert_detection(const DetectionResultData& detection) = 0;

        /**
         * List detection data.
         *
         * @param detections vector of detections
         * @param stored_only list only the stored-only ones
         * @return the numbert of detections found
         * */
        virtual int list_detections(std::vector<DetectionResultData>& detections) = 0;

        /**
         * Remove listed detections.
         *
         * @param listed detections
         * @return the number of rows affected
         * */
        virtual int remove_detections(const std::vector<DetectionResultData> detections) = 0;

};

#endif
