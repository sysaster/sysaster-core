#include "sysaster/helpers.h"

std::string quotesql( const std::string& s ) {
    return std::string("'") + s + std::string("'");
}
