#include <cstdint>
#include <cstdbool>

#include "tm4c_cmsis.h"

#include "bsp/bsp.h"
#include "drivers/i2c/i2cmodule.hpp"
#include "hal/gpiopin.hpp"
#include "hal/gpioport.hpp"
#include "hal/hal.hpp"

using I2CModule1 = I2CModule<1U>;
I2CModule1 i2cModule1{};

hal::GPIO<GPIOPorts::F>::AHB gpioFAHB;
hal::GPIO<GPIOPorts::F>::Clock gpioFClock;
hal::GPIO<GPIOPorts::F>::Interrupt gpioFInterrupt;
//constexpr unsigned DATA_READY_PIN = 6U; //TBD

//TODO
// - Handle the 'data ready' interrupt, including
//     - I2C communication with the sensor
// - Configure and enable the sensor

void setupDataReadyInterrupt() {
    gpioFClock.enable();
    gpioFAHB.enable();
    
    // Some notes below.
    // - GPIOIM must be masked (cleared) until the other registers have been configured.
    // - trigger on positive level (active high)
    //     - GPIOIS = 1, GPIOIEV = 1
    // - see 10.3 Initialization and Configuration from data sheet
    gpioFInterrupt.configure(GPIOIntConfig::Sense{ GPIOIntConfig::Sense::IS::LEVEL });
    gpioFInterrupt.configure(GPIOIntConfig::Event{ GPIOIntConfig::Event::IEV::HIGH });
    // enable interrupt on GPIO port F pin 0
    gpioFInterrupt.enable([] (GPIOPortInterruptContext ctx) {
        // toggle LED (test)
        GPIOF_HS->DATA_Bits[1] ^= 1;
    }, GPIOPin::P0);
    
    //TBD:
    // Port F pin 0 is connected to the physical switch, so we use it for test.
    // Later switch to a pin header.
    // Use some other port than GPIOA perhaps, since it uses APB for I2C.
}

int main() {
    setupDataReadyInterrupt();
    //TBD: use the hal for this, or remove if leds not needed
    GPIOF_HS->DIR |= (LED_RED | LED_BLUE | LED_GREEN); // set led pins as outputs
    GPIOF_HS->DEN |= (LED_RED | LED_BLUE | LED_GREEN); // digital enable
    
    while (1)
        ;
    //return 0;
}
