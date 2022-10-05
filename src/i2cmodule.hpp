#ifndef I2CMODULE_CXX
#define I2CMODULE_CXX

#include <cstdint>

#include "bsp.h"
#include "i2cmodulecontroller.hpp"
#include "i2ctypes.hpp"
#include "gpioport.hpp"


/*
 * I2C Modules
 * 
 * Each I2C module defines the parameter values such as bit positions
 * in registers for a single I2C hardware module. It also exposes an
 * interface to interact with the module.
 */


//! I2C module
/*!
 * \tparam ModuleNumber I2C module number (0-n where n is the greatest possible I2C module number)
 */
template <std::uint_least8_t ModuleNumber>
class I2CModule {};


//! I2C module 1, GPIO Port A, pins 6 (clock) and 7 (data)
template <>
class I2CModule<1U> {
    public:
        static constexpr std::uint_least8_t moduleIndex = 1U;
        using t_cntl = I2CModuleController<I2CModule<moduleIndex>>;
        
        I2CModule()
            : m_cntl(t_cntl::instance())
            {}
        
        static constexpr t_slave_status slaveStatusFBR = t_cntl::slaveStatusFBR;
        static constexpr t_slave_status slaveStatusRREQ = t_cntl::slaveStatusRREQ;
            
        //! Initialize the perihperal and module
        void initSlave(std::uint8_t address, t_cntl::t_int_callback cb) noexcept {
            m_cntl.initSlave(address, std::move(cb));
        }
        
        //! Reset the peripheral
        void reset() const noexcept { m_cntl.resetPeripheral(); }
        
        //! Interrupt handler
        void interrupt() const noexcept { m_cntl.interrupt(); };
        
        static constexpr t_register_addr moduleSlaveBase = I2C1_SLAVE_BASE;
        static constexpr t_register_addr moduleMasterBase = I2C1_MASTER_BASE;
        
        using gpioPort = GPIOPorts::A;
        static constexpr t_register_addr nvicIntEnableRegAddr = I2C1_NVIC_INT_ENABLE_R;
        
        static constexpr std::uint_fast8_t gpioPortDataPin = (1U << 7);
        static constexpr std::uint_fast8_t gpioPortClockPin = (1U << 6);
        static constexpr std::uint_fast32_t gpioPortCtlMask = (3U << 28) | (3U << 24);
        static constexpr std::uint_fast8_t nvicIntEnableMask = (1U << 5); // interrupt 37
        
    private:
        t_cntl & m_cntl;
};

#endif
