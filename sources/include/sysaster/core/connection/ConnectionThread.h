#ifndef _CON_THREAD_
#define _CON_THREAD_

#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include <boost/lockfree/queue.hpp>
#include "sysaster/core/detector/DetectionResultData.h"

/**
 * Wrapper for the function that actually sends a data via
 * socket.
 * */
class Settings;
class ConnectionThread {

    private:

        std::shared_ptr<Settings> settings;

    public:

        ConnectionThread();
        ~ConnectionThread() {}

        /**
         * Called to send the data.
         * */
        void operator()(
                int id, 
                RestClient::Connection* connection,
                const DetectionResultData& data, 
                boost::lockfree::queue<RestClient::Connection*>& connPool);
};

#endif
