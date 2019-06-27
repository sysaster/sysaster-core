#include "sysaster/core/connection/ConnectionThread.h"
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "extern/json.hpp"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"
#include <iostream>

using nlohmann::json;

ConnectionThread::ConnectionThread() : settings {sysaster::settings} {}

void ConnectionThread::operator()(
        int id, 
        RestClient::Connection* connection,
        const DetectionResultData& data, 
        boost::lockfree::queue<RestClient::Connection*>& connPool){

        std::cout << "[sysaster INFO] try to send" << std::endl;
    	
        std::string datajson = data.to_json();

        RestClient::Response resp = connection->post(settings->server_endpoint, datajson);

        bool success = false;

        if (resp.code == -1) {
            std::cout << "[sysaster ERROR] fail to post detection data" << std::endl;
        } else if (resp.code == 200) {
            std::cout << "[sysaster SUCCESS] posted detection data" << std::endl;
            success = true;
        } else {
            std::cout << "[sysaster ERROR] unknown REST connection behaviour" << std::endl;
        }

        if (!success) {
            try {
                if (sysaster::detection_dao != nullptr) {
                    sysaster::detection_dao->insert_detection(data);
                    std::cout << "[sysasterdb WARNING] data saved on local db" << std::endl;
                } else {
                    std::cout << "[sysasterdb ERROR] local db unavailable" << std::endl;
                }
            } catch (std::exception e) {
                std::cout << e.what() << std::endl;
            }
        }

        //if (data.clipped_image != nullptr)
        //    delete[] data.clipped_image;

        connPool.push(connection);
}
