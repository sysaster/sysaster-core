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
        ClientInfoQueueT connectionPool;
        ctpl::thread_pool connThreadPool { settings->connection_pool_size };

    public:

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
                if (connectionPool.size() == 0) 
                    continue;
                DetectionResultData data;
                if (!next(data))
                    continue;
                ClientInfo conn = connectionPool.pop()->get_data();
                connThreadPool.push(ConnectionThread(), 
                        std::ref(conn), std::ref(data), std::ref(this->connectionPool));
            } 
        }
};

#endif
