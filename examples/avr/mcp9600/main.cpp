//
// MCP9600 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include <libfirmware/utils/logger.hpp>
#include <libfirmware/comms/i2c/i2c_device.hpp>
#include <libfirmware/platform/avr/atmega2560/hardware.hpp>

#include <libfirmware/drivers/sensors/mcp9600.hpp>

#define MCP9600_ADDRESS 0x67

using namespace libfirmware::drivers;
using namespace libfirmware::utils;
using namespace libfirmware::platform::avr::atmega2560;

int main()
{
    Logger<Hardware::Uart0> logger{Baud::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    // Arduino Mega - Pullup SDA, SCL
    Hardware::GpioD::OutputPin<0> scl_pullup;
    Hardware::GpioD::OutputPin<1> sda_pullup;

    sda_pullup.set();
    scl_pullup.set();

    LOG_INFO("Initialize I2C");

    Hardware::I2C::initialize();

    sensors::Mcp9600<Hardware::I2C> mcp{MCP9600_ADDRESS};

    if (mcp.verify())
    {
        LOG_INFO("MCP9600 found! Configuring...");
        mcp.setThermocouple(sensors::Mcp9600_ThermocoupleType::S);
    }

    for(;;)
    {
        const auto cold = (int)mcp.readAmbient();
        const auto hot = (int)mcp.readThermocouple();
        const auto delta = (int)mcp.readDelta();
        const auto adc = mcp.readAdc();
        const auto type = mcp.getThermocoupleType();

        LOG_INFO("Cold:  %d", cold);
        LOG_INFO("Hot:   %d", hot);
        LOG_INFO("Delta: %d", delta);
        LOG_INFO("ADC:   %X -> %d", adc, adc);
        LOG_INFO("Type: %d", static_cast<uint8_t>(type));

        _delay_ms(1000);
    }

    return 0;
}
