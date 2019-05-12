#ifndef _SYS_COMMON_
#define _SYS_COMMON_

#include "sysaster/core/settings/Settings.h"
#include "sysaster/core/connection/ConnectionThreadDispatcher.h"

class ConnectionThreadDispatcher;

namespace sysaster {

    inline const static std::shared_ptr<Settings> settings = std::make_shared<Settings>();

    inline static std::shared_ptr<PersonDetector> person_detector {nullptr};

    inline static std::shared_ptr<ConnectionThreadDispatcher> connection_dispatcher {nullptr};

};

#endif
