#ifndef I2CINTERRUPTCONTEXT_CXX
#define I2CINTERRUPTCONTEXT_CXX

#include <cstdint>

#include "i2ctypes.hpp"

struct I2CInterruptContext {
    const t_slave_status slaveStatus;
    const t_slave_data slaveData;
};

#endif
