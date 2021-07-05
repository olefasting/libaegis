//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_SYSTEM_TIME_H
#define LIBAEGIS_SYSTEM_TIME_H

#include <chrono>

namespace aegis {
    typedef std::chrono::high_resolution_clock SystemClock;
    typedef std::chrono::high_resolution_clock::duration Duration;
    typedef std::chrono::high_resolution_clock::time_point TimePoint;
}

#endif    // LIBAEGIS_SYSTEM_TIME_H
