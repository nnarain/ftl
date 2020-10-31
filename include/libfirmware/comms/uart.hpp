//
// comms/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 30 2020
//

#ifndef LIBFIRMWARE_COMMS_UART_HPP
#define LIBFIRMWARE_COMMS_UART_HPP

namespace libfirmware
{
namespace comms
{
namespace uart
{
    enum class BaudRate: unsigned long
    {
        // Rate_110    = 110,
        // Rate_300    = 300,
        // Rate_600    = 600,
        // Rate_1200   = 1200,
        // Rate_2400   = 2400,
        // Rate_4800   = 4800,
        Rate_9600   = 9600,
        Rate_14400  = 14400,
        Rate_19200  = 19200,
        Rate_38400  = 38400,
        Rate_57600  = 56700,
        Rate_115200 = 115200,
        // Rate_128000 = 128000,
        // Rate_256000 = 256000
    };
}
}
}

#endif // LIBFIRMWARE_COMMS_UART_HPP
