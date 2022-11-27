#ifndef GPIOPORT_CXX
#define GPIOPORT_CXX

#include <cstdint>

#include "bsp/bsp.h"
#include "drivers/i2c/i2ctypes.hpp" // TODO: move to a generic file


/*
 * GPIO ports
 * 
 * Each GPIO port defines the parameter values such as bit positions
 * in registers for a single GPIO hardware port.
 */

namespace {
    enum class GPIOPort { A, B, C, D, E, F };
};

//! GPIO port
/*!
 * \tparam GPIOPortID GPIO port ID
 */
template <GPIOPort Port>
class GPIOPortImpl {};


//! GPIO Port A
template <>
class GPIOPortImpl<GPIOPort::A> {
    public:
        static constexpr t_register_addr AFSelRegAddr = GPIOAFSEL_PORTA;
        static constexpr t_register_addr openDrainRegAddr = GPIOODR_PORTA;
        static constexpr t_register_addr ctlRegAddr = GPIOPCTL_PORTA;
        static constexpr t_register_addr digitalEnableRegAddr = GPIODEN_PORTA;
        
        static constexpr std::uint_fast8_t sysCtlPeripheral = (1U << 0);
        
        static constexpr t_register_addr nvicIntEnableRegAddr = GPIOA_NVIC_INT_ENABLE_R;
        static constexpr std::uint_fast8_t nvicIntEnableMask = (1U << 0); // interrupt 0
        static constexpr t_register_addr intMaskRegAddr = GPIOIM_PORTA;
};


//! GPIO Port F
template <>
class GPIOPortImpl<GPIOPort::F> {
    public:
        static constexpr t_register_addr AFSelRegAddr = GPIOAFSEL_PORTF;
        static constexpr t_register_addr openDrainRegAddr = GPIOODR_PORTF;
        static constexpr t_register_addr ctlRegAddr = GPIOPCTL_PORTF;
        static constexpr t_register_addr digitalEnableRegAddr = GPIODEN_PORTF;
        
        static constexpr std::uint_fast8_t sysCtlPeripheral = (1U << 5);
        
        static constexpr t_register_addr nvicIntEnableRegAddr = GPIOF_NVIC_INT_ENABLE_R;
        static constexpr std::uint_fast8_t nvicIntEnableMask = (1U << 30); // interrupt 30
        static constexpr t_register_addr intMaskRegAddr = GPIOIM_PORTF;
};

namespace GPIOPorts {
    using A = GPIOPortImpl<GPIOPort::A>;
    using F = GPIOPortImpl<GPIOPort::F>;
};

#endif
