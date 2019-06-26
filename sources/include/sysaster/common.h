#ifndef _SYS_COMMON_
#define _SYS_COMMON_

#include "sysaster/core/settings/Settings.h"
#include "sysaster/core/connection/ConnectionThreadDispatcher.h"
#include "sysaster/core/detector/PersonDetector.h"

class ConnectionThreadDispatcher;

namespace sysaster {

    extern std::shared_ptr<Settings> settings;

    static std::shared_ptr<PersonDetector> person_detector {nullptr};

    static std::shared_ptr<ConnectionThreadDispatcher> connection_dispatcher {nullptr};

};

#endif
