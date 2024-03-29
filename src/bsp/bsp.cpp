/* Board Support Package */

#include <cstdint>
#include <cstdbool>

#include "bsp.h"
#include "tm4c_cmsis.h"

#include "drivers/i2c/i2cmodule.hpp"
#include "hal/gpioport.hpp"

extern I2CModule<1> i2cModule1;

extern "C" {
__attribute__ ((naked)) void assert_failed (char const *file, int line) {
    // TBD: damage control
    //NVIC_SystemReset();
    while (1);
}

void I2C1_IRQHandler(void) {
    i2cModule1.interrupt();
}

void GPIOPortF_IRQHandler(void) {
    GPIOPorts::F::t_cntl::instance().interrupt();
}
}
