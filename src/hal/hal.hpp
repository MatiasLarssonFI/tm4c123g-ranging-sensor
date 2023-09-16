#ifndef HAL_CXX
#define HAL_CXX

#include <type_traits>

#include "gpiopin.hpp"
#include "gpioport.hpp"
#include "gpioconfig.hpp"

#include "bsp/bsp.h"

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
        using t_callback = GPIOPort::t_cntl::t_int_callback;
        
        // GPIOIS
        void configure(GPIOIntConfig::Sense s) {
            HWREG(GPIOPort::intSenseRegAddr) |= static_cast<int>(s.is);
        }
        
        // GPIOIEV
        void configure(GPIOIntConfig::Event e) {
            HWREG(GPIOPort::intEventRegAddr) |= static_cast<int>(e.iev);
        }
        
        // GPIOIM
        template <class... PinTypes, class = std::common_type_t<GPIOPin, PinTypes...>>
        void enable(t_callback cb, GPIOPin pin, PinTypes... pins) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            enable(std::move(cb), pins...);
        }
        
        // GPIOIM and NVIC interrupt enable, and user callback setup
        void enable(t_callback cb, GPIOPin pin) {
            HWREG(GPIOPort::intMaskRegAddr) |= (1U << static_cast<int>(pin));
            HWREG(GPIOPort::nvicIntEnableRegAddr) |= GPIOPort::nvicIntEnableMask;
            GPIOPort::t_cntl::instance().enableInterrupt(std::move(cb));
        }
    };
};

#endif
