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

/**
 * Wrapper for the function that actually sends a data via
 * socket.
 * */
using nlohmann::json;
class ConnectionThread {

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
                ClientInfoQueueT& connPool){

            // if connection not stablished
            if (connection.fd == 0) {
                if ((connection.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                    printf("\n Socket creation error \n");
                    return;
                }

                fcntl(connection.fd, F_SETFL, fcntl(connection.fd, F_GETFL, 0) | O_NONBLOCK);

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
            ClientInfoNode connNode {connection};
            connPool.enqueue(connNode); 
        }
};

#endif
