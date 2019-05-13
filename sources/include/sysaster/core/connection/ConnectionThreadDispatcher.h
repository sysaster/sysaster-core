#ifndef _CONN_THREAD_DISPATCHER_
#define _CONN_THREAD_DISPATCHER_

#include "sysaster/core/detector/DetectionResultData.h"
#include "sysaster/core/detector/CdsResultDataTypes.h"
#include "CdsClientInfoDataTypes.h"
#include "ClientInfo.h"
#include "ConnectionThread.h"
#include "extern/ctpl_stl.h"
#include "sysaster/core/settings/Settings.h"
#include "sysaster/common.h"

#include <boost/lockfree/queue.hpp>

/**
 * Dispatcher for client connections,
 * aiming at sending detection data.
 *
 * @author Vitor Greati
 * */
class ConnectionThreadDispatcher {

    private:

        std::shared_ptr<Settings> settings = sysaster::settings;

        ResultDataQueueT dataQueue;

        boost::lockfree::queue<ClientInfo> connectionPool {settings->connection_pool_size};

        ctpl::thread_pool connThreadPool { settings->connection_pool_size };

    public:

        ~ConnectionThreadDispatcher() {
            cds::threading::Manager::detachThread(); 
        }

        ConnectionThreadDispatcher() {

            cds::threading::Manager::attachThread();

            for (auto i {0}; i < settings->connection_pool_size; ++i) {
                auto clin = ClientInfoNode{ClientInfo{}};
                connectionPool.push(ClientInfo{});
            }
        }

        void require_send(std::vector<DetectionResultData>& data) {
            for (DetectionResultData& d : data) {
                DetectionResultDataNode n {d};
                dataQueue.enqueue(n);
            }
        }

        bool next(DetectionResultData & data) {
            if (dataQueue.size() == 0)
                return false;
            else
                data = dataQueue.pop()->get_data();
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
