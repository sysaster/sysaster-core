#ifndef _CLIENT_INFO_
#define _CLIENT_INFO_

#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 

/**
 * Data about a client connection.
 * */
struct ClientInfo {

    ClientInfo() {}

    struct sockaddr_in addr;
    socklen_t addr_len;
    int fd;
};

#endif
