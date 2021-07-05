//
// Created by oasf on 04.07.2021.
//

#include <crossguid/guid.hpp>

#include "aegis/uuid.h"

namespace aegis {
    std::string generate_uuid() {
        auto guid = xg::newGuid();
        return guid.str();
    }
}