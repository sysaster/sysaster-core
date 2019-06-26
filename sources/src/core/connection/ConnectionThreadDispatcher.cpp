#include "sysaster/core/connection/ConnectionThreadDispatcher.h"
#include "sysaster/common.h"

ConnectionThreadDispatcher::ConnectionThreadDispatcher() : settings {sysaster::settings} {
    RestClient::init();

    for (auto i {0}; i < settings->connection_pool_size; ++i)
        restConnPool.push(new RestClient::Connection(settings->server_url));
}
