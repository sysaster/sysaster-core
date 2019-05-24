#ifndef _CON_THREAD_
#define _CON_THREAD_

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
#include <boost/lockfree/queue.hpp>
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

/**
 * Wrapper for the function that actually sends a data via
 * socket.
 * */
using nlohmann::json;
class ConnectionThread {

    private:

        std::shared_ptr<Settings> settings = sysaster::settings;

    public:

        ConnectionThread() {}
        ~ConnectionThread() {}


        /**
         * Called to send the data.
         * */
        void operator()(
                int id, 
                RestClient::Connection* connection,
                const DetectionResultData& data, 
                boost::lockfree::queue<RestClient::Connection*>& connPool){
        
            RestClient::Response resp = connection->post("/detections/person", data.to_json());

            if (resp.code == -1) {
                std::cout << "[sysaster ERROR] fail to post detection data" << std::endl;
            } else if (resp.code == 200) {
                std::cout << "[sysaster SUCCESS] posted detection data" << std::endl;
            } else {
                std::cout << "[sysaster ERROR] unknown REST connection behaviour" << std::endl;
            }

            connPool.push(connection);
        }
};

#endif
