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

	std::cout << settings->server_endpoint << std::endl;
	std::cout << datajson << std::endl;

        if (resp.code == -1) {
            std::cout << "[sysaster ERROR] fail to post detection data" << std::endl;
        } else if (resp.code == 200) {
            std::cout << "[sysaster SUCCESS] posted detection data" << std::endl;
        } else {
            std::cout << "[sysaster ERROR] unknown REST connection behaviour" << std::endl;
        }

        connPool.push(connection);
}
