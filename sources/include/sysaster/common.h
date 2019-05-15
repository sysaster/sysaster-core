#ifndef _SYS_COMMON_
#define _SYS_COMMON_

#include "sysaster/core/settings/Settings.h"
#include "sysaster/core/connection/ConnectionThreadDispatcher.h"

class ConnectionThreadDispatcher;

namespace sysaster {

    const static std::shared_ptr<Settings> settings = std::make_shared<Settings>();

    static std::shared_ptr<PersonDetector> person_detector {nullptr};

    static std::shared_ptr<ConnectionThreadDispatcher> connection_dispatcher {nullptr};

};

#endif
