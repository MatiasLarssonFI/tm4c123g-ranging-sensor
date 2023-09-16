#ifndef GPIOCONFIG_CXX
#define GPIOCONFIG_CXX

#include <cstdint>

#include "bsp/bsp.h"


/*
 * GPIO interrupt configurations.
 * 
 * Values for GPIO interrupt configuration registers.
 */

namespace {
    struct GPIOIntSenseImpl {
        enum class IS { EDGE, LEVEL };
        IS is;
    };


    struct GPIOIntEventImpl {
        enum class IEV { LOW, HIGH };
        IEV iev;
    };
}

namespace GPIOIntConfig {
    using Sense = GPIOIntSenseImpl;
    using Event = GPIOIntEventImpl;
};

#endif
