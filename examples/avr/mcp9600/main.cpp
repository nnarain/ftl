//
// MCP9600 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/drivers/sensors/mcp9600.hpp>

#include <ftl/platform/platform.hpp>

#define MCP9600_ADDRESS 0x67

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    LOG_INFO("Initialize I2C");

    Hardware::I2C0::initialize();

    sensors::Mcp9600<Hardware::I2C0> mcp{MCP9600_ADDRESS};

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

        Hardware::Timer::delayMs(1000);
    }

    return 0;
}
