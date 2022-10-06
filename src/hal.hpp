#ifndef HAL_CXX
#define HAL_CXX

#include <type_traits>

#include "bsp.h"
#include "gpiopin.hpp"
#include "gpioport.hpp"

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
        template <class... PinTypes, class = std::common_type_t<GPIOPin, PinTypes...>>
        void enable(GPIOPin pin, PinTypes... pins) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            enable(pins...);
        }
        
        void enable(GPIOPin pin) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            HWREG(GPIOPort::nvicIntEnableRegAddr) |= GPIOPort::nvicIntEnableMask;
        }
    };
};

#endif
