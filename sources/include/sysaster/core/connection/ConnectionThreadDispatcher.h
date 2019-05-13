#ifndef _CONN_THREAD_DISPATCHER_
#define _CONN_THREAD_DISPATCHER_

#include "sysaster/core/detector/DetectionResultData.h"
#include "ClientInfo.h"
#include "ConnectionThread.h"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"
#include <boost/lockfree/queue.hpp>
#include "extern/ctpl_stl.h"

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

        boost::lockfree::queue<ClientInfo> connectionPool {settings->connection_pool_size};

        ctpl::thread_pool connThreadPool { settings->connection_pool_size };

    public:

        ~ConnectionThreadDispatcher() {}

        ConnectionThreadDispatcher() {
            for (auto i {0}; i < settings->connection_pool_size; ++i)
                connectionPool.push(ClientInfo{});
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
                if (connectionPool.empty()) 
                    continue;
                DetectionResultData data;
                if (!next(data))
                    continue;
                ClientInfo conn; 
                connectionPool.pop(conn);
                connThreadPool.push(ConnectionThread(), 
                        conn, std::ref(data), std::ref(this->connectionPool));
            } 
        }
};

#endif
