#include "sysaster/common.h"

std::shared_ptr<Settings> sysaster::settings = std::make_shared<Settings>();
std::shared_ptr<DetectionDAO> sysaster::detection_dao {nullptr};
std::shared_ptr<PersonDetector> sysaster::person_detector {nullptr};
