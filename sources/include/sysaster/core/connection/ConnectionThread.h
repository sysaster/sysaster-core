#ifndef _CON_THREAD_
#define _CON_THREAD_

#include "CdsClientInfoDataTypes.h"
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
                ClientInfo& connection, 
                const DetectionResultData& data, 
                boost::lockfree::queue<ClientInfo>& connPool){

            cds::threading::Manager::attachThread();
            // if connection was stablished
            if (connection.fd == 0) {
                if ((connection.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                    printf("\n Socket creation error \n");
                    return;
                }

                fcntl(connection.fd, F_SETFL, fcntl(connection.fd, F_GETFL, 0) | O_NONBLOCK);

                memset(&connection.addr, '0', sizeof(connection.addr)); 
               
                connection.addr.sin_family = AF_INET; 
                connection.addr.sin_port = htons(settings->server_port); 
                   
                // Convert IPv4 and IPv6 addresses from text to binary form 
                if(inet_pton(AF_INET, settings->server_ip.c_str(), &connection.addr.sin_addr)<=0)  
                { 
                    printf("\nInvalid address/ Address not supported \n"); 
                    return; 
                }  

                if (connect(connection.fd, (struct sockaddr *) &connection.addr, connection.addr_len) < 0
                && (errno == EALREADY
                ||  errno == EINPROGRESS)) {
                    printf("\nConnection Failed \n"); 
                    return; 
                } 
            }

            // make message
            json json_message = data;                                    
            std::string message = json_message.dump();

            // send
            send(connection.fd, message.c_str(), message.length(), 0);

            // repopulate connection pool
            connPool.push(connection); 
            cds::threading::Manager::detachThread();
        }
};

#endif
