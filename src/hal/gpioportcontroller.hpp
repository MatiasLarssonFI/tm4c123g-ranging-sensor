#ifndef GPIOCONTROLLER_CXX
#define GPIOCONTROLLER_CXX

#include <cstdint>
#include <functional>
#include <optional>

struct GPIOPortInterruptContext { /* TBD */ };

template <class TPort>
class GPIOPortController {
    public:
        using t_int_callback = std::function<void(GPIOPortInterruptContext)>;
        
        //! Returns the singleton instance.
        static GPIOPortController<TPort>& instance() {
            static GPIOPortController instance;
            return instance;
        }
        
        //! Interrupt handler
        void interrupt() const noexcept {
            if (m_intCB) {
                (*m_intCB)(GPIOPortInterruptContext{ /* TBD */ });
                // TBD: clear the interrupt, if not level-detect.
                // Define the registers in TPort i.e. GPIOPort.
            }
        }
        
        void enableInterrupt(t_int_callback cb) {
            m_intCB = std::move(cb);
        }
        
    private:
        GPIOPortController()
            {}
        
        std::optional<t_int_callback> m_intCB;
};

#endif
