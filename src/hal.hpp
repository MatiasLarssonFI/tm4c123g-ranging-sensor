#ifndef HAL_CXX
#define HAL_CXX

#include <cstdint>

#include "bsp.h"

namespace hal {
    template <class GPIOPort> class GPIO;
};

template <class GPIOPort>
struct hal::GPIO {
    struct AHB {
        void enable() {
            GPIOHSCTL_REG |= GPIOPort::sysCtlPeripheral;
        }
    };
    
    struct Clock {
        void enable() {
            RCGCGPIO_REG |= GPIOPort::sysCtlPeripheral;
        }
    };
    
    struct Interrupt {
        template <class PinType, class... PinTypes>
        void enable(PinType pin, PinTypes... pins) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            enableInterrupt(pins...);
        }
        
        template <class PinType>
        void enable(PinType pin) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            HWREG(GPIOPort::nvicIntEnableRegAddr) |= GPIOPort::nvicIntEnableMask;
        }
    };
};

#endif
