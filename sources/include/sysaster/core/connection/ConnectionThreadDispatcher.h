#ifndef _CONN_THREAD_DISPATCHER_
#define _CONN_THREAD_DISPATCHER_

#include "sysaster/core/detector/DetectionResultData.h"
#include "ClientInfo.h"
#include "ConnectionThread.h"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"
#include <boost/lockfree/queue.hpp>
#include "extern/ctpl_stl.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

/**
 * Dispatcher for client connections,
 * aiming at sending detection data.
 *
 * @author Vitor Greati
 * */
class ConnectionThreadDispatcher {

    private:

        std::shared_ptr<Settings> settings = sysaster::settings;

        boost::lockfree::queue<DetectionResultData> dataQueue {100};

        boost::lockfree::queue<RestClient::Connection*> restConnPool {settings->connection_pool_size};

        ctpl::thread_pool connThreadPool { settings->connection_pool_size };

    public:

        ~ConnectionThreadDispatcher() {
            RestClient::disable();
        }

        ConnectionThreadDispatcher() {

            RestClient::init();

            for (auto i {0}; i < settings->connection_pool_size; ++i)
                restConnPool.push(new RestClient::Connection(settings->server_endpoint));
        }

        void require_send(std::vector<DetectionResultData>& data) {
            for (DetectionResultData& d : data)
                dataQueue.push(d);
        }

        bool next(DetectionResultData & data) {
            if (dataQueue.empty())
                return false;
            dataQueue.pop(data);
            return true;
        }

        /**
         * Continuous loop for checking connection availability
         * and dispaching.
         * */
        void operator()() {
            while (true) {
                if (restConnPool.empty()) 
                    continue;
                DetectionResultData data;
                if (!next(data))
                    continue;
                RestClient::Connection* conn;
                restConnPool.pop(conn);
                connThreadPool.push(ConnectionThread(), 
                        conn, std::ref(data), std::ref(this->restConnPool));
            } 
        }
};

#endif
