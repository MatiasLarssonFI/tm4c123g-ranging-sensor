#ifndef I2CMODULECONTROLLER_CXX
#define I2CMODULECONTROLLER_CXX

#include <functional>
#include <cstdint>

#include "i2cinterruptcontext.hpp"
#include "i2ctypes.hpp"

#include "hal/hal.hpp"


//! I2C module controller
/*!
 * Implements the OO interface for an I2C module.
 * \tparam TModule The I2C module to control
 */
template <class TModule>
class I2CModuleController {
    public:
        using t_int_callback = std::function<void(I2CInterruptContext)>;
        
        //! Returns the singleton instance.
        static I2CModuleController<TModule>& instance() {
            static I2CModuleController<TModule> instance;
            return instance;
        }
        
        
        //! Initialize the peripheral and module
        void initSlave(std::uint8_t address, t_int_callback cb) noexcept {
            this->initCommon();
            // Enable the I2C slave
            HWREG(masterCfgRegAddr) |= masterCfgSlaveEnable;
            HWREG(slaveCtlStatusRegAddr) = 1U;
            // Set slave own address
            HWREG(slaveOwnAddrRegAddr) = address;
            // Enable the I2C slave data interrupt
            HWREG(slaveIntMaskRegAddr) |= slaveDataInterrupt;
            m_intCB = std::move(cb);
            m_isInitialized = true;
        }
        
        
        //! Reset the peripheral
        void resetPeripheral() const noexcept {
            // set reset bit
            SRI2C_REG |= sysCtlPeripheral;
            // short delay here
            for(std::uint_fast8_t delay = 0; delay < 16U; ++delay)
                ;
            // clear reset bit
            SRI2C_REG &= ~sysCtlPeripheral;
        }
        
        
        //! Interrupt handler
        void interrupt() const noexcept {
            if (m_isInitialized) {
                m_intCB(I2CInterruptContext{
                    HWREG(slaveCtlStatusRegAddr) & slaveStatusBits,
                    HWREG(slaveDataRegAddr) & slaveDataBits
                });
                HWREG(slaveIntClearRegAddr) = slaveDataInterrupt; // clear the interrupt
            }
        }
        
        
        I2CModuleController(I2CModuleController const &) = delete;
        I2CModuleController& operator = (I2CModuleController const &) = delete;
        
        
        static constexpr t_slave_status slaveStatusFBR = 0x5U;
        static constexpr t_slave_status slaveStatusRREQ = 0x1U;
    private:
        I2CModuleController()
            : m_isInitialized(false)
            {}
        
        void initCommon() const noexcept {
            // system control: enable I2C module clock
            RCGCI2C_REG |= sysCtlPeripheral;
            // reset peripheral
            resetPeripheral();
            // system control: enable GPIO module clock
            typename hal::GPIO<typename TModule::gpioPort>::Clock gpioClock;
            gpioClock.enable();
            // Select peripheral (i.e. I2C) function for the pins
            HWREG(TModule::gpioPort::AFSelRegAddr) |= (TModule::gpioPortDataPin | TModule::gpioPortClockPin);
            // Enable data pin for open drain
            HWREG(TModule::gpioPort::openDrainRegAddr) |= TModule::gpioPortDataPin;
            // Assing I2C signals to pins
            HWREG(TModule::gpioPort::ctlRegAddr) |= TModule::gpioPortCtlMask;
            // Select digital enable for the pins
            HWREG(TModule::gpioPort::digitalEnableRegAddr) |= (TModule::gpioPortDataPin | TModule::gpioPortClockPin);
            // Enable the I2C NVIC interrupt
            HWREG(TModule::nvicIntEnableRegAddr) |= TModule::nvicIntEnableMask;
        }
        
        
        bool m_isInitialized;
        t_int_callback m_intCB;
        
        static constexpr std::uint_fast8_t masterCfgSlaveEnable = (1U << 5);
        static constexpr std::uint_fast8_t slaveDataInterrupt = 1U;
        static constexpr t_slave_status slaveStatusBits = 0xf;
        static constexpr t_slave_data slaveDataBits = 0xff;
        
        static constexpr t_register_addr slaveOwnAddrRegAddr = TModule::moduleSlaveBase;
        static constexpr t_register_addr slaveCtlStatusRegAddr = TModule::moduleSlaveBase + 4U;
        static constexpr t_register_addr slaveDataRegAddr = TModule::moduleSlaveBase + 8U;
        static constexpr t_register_addr slaveIntMaskRegAddr = TModule::moduleSlaveBase + 0xCU;
        static constexpr t_register_addr slaveIntClearRegAddr = TModule::moduleSlaveBase + 0x18U;
        static constexpr t_register_addr masterCfgRegAddr = TModule::moduleMasterBase + 0x20U;
        
        static constexpr std::uint_fast8_t sysCtlPeripheral = (1U << TModule::moduleIndex);
};

#endif
