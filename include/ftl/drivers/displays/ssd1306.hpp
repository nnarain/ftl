//
// ssd1306.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 08 2020
//
#ifndef FTL_COMMS_DRIVERS_DISPLAY_HPP
#define FTL_COMMS_DRIVERS_DISPLAY_HPP

#include <stdint.h>

#include <ftl/comms/i2c/i2c_device.hpp>

namespace ftl
{
namespace drivers
{
namespace displays
{
/**
 * SSD1306 - OLED display
 * 
 * I2C Bus Frame
 * 
 * |SLA+RW|Co:D/C#:000000|Data|Co:D/C#:0..0|...
 * 
 * Co   - continuation bit
 *      - If set to logic 0, transmission of the following information will container data bytes only
 * D/C# - Data / Command selection bit
 *      - Determines if the next byte acts as a command or as data
 *      - Logic 0: The following byte is a command
 *      - Logic 1: The following byte is data for GDDRAM
*/
template<class I2C>
class Ssd1306
{
    static constexpr uint8_t CONTROL_COMMAND = 0x00;
    static constexpr uint8_t CONTROL_DATA = 0xC0;

    // Constrast, followed by 8-bit constrast level
    static constexpr uint8_t COMMAND_CONTRAST = 0x81;
    // Display on or off (A4 = resume, A5 = ignore RAM)
    static constexpr uint8_t COMMAND_DISPLAY_RESUME = 0xA4;
    // Invert display (A6 = Normal, A7 = Inverted)
    static constexpr uint8_t COMMAND_DISPLAY_INVERSE = 0xA6;
    // Display on of off (normal AF vs sleep mode AE)
    static constexpr  uint8_t COMMAND_DISPLAY_ON = 0xAE;

    // Set the lower column start address for page addressing mode
    // 0000XXXX
    static constexpr uint8_t COMMAND_PAGE_MODE_SET_LOW_COL_START = 0x00;
    // 0001XXXX
    static constexpr uint8_t COMMAND_PAGE_MODE_SET_HIGH_COL_START = 0x10;

    // Addressing mode
    static constexpr uint8_t COMMAND_ADDRESSING_MODE = 0x20;
    // Set column address
    static constexpr uint8_t COMMAND_COLUMN_ADDRESS = 0x21;
    // Set page address
    static constexpr uint8_t COMMAND_SET_PAGE_ADDRESS = 0x22;
    // Set page start address for page addressing mode
    static constexpr uint8_t COMMAND_SET_PAGE_START_ADDRESS = 0xB0;
    // Display start line
    static constexpr uint8_t COMMAND_DISPLAY_START_LINE = 0x40;
    // Set segment re-map (A0 = column address 0 is mapped to SEG0, A1 = column address 127 is mapped to SEG0)
    static constexpr uint8_t COMMAND_SEGMENT_REMAP = 0xA0;
    // Set multiplex ratio
    static constexpr uint8_t COMMAND_MULTIPLEX_RATIO = 0xA8;
    // COM output scan direction
    static constexpr uint8_t COMMAND_COM_SCAN_DIRECTION = 0xC0;
    // Set display offset
    static constexpr uint8_t COMMAND_DISPLAY_OFFSET = 0xD3;
    // COM pins configuration
    static constexpr uint8_t COMMAND_COM_CONFIG = 0xDA;
    // Set display clock divide
    static constexpr uint8_t COMMAND_DISPLAY_CLOCK_DIVIDE = 0xD5;
    // Pre-charge period
    static constexpr uint8_t COMMAND_PRE_CHARGE_PERIOD = 0xD9;
    // Deselect level
    static constexpr uint8_t COMMAND_DESELECT_LEVEL = 0xDB;
    // NOP
    static constexpr uint8_t COMMAND_NOP = 0xE3;


public:
    // Memory Addressing Mode
    enum class AddressingMode
    {
        Horizontal = 0x00,
        Vertical   = 0x01,
        Page       = 0x02
    };

    Ssd1306(uint8_t address)
        : device_{address}
    {
    }

    bool detect()
    {
        return device_.detect();
    }

    void enable(bool on)
    {
        sendCommand(COMMAND_DISPLAY_ON | static_cast<uint8_t>(on));
    }

    void resume(bool ignore_ram=false)
    {
        sendCommand(COMMAND_DISPLAY_RESUME | static_cast<uint8_t>(ignore_ram));
    }

    void invert(bool invert)
    {
        sendCommand(COMMAND_DISPLAY_INVERSE | static_cast<uint8_t>(invert));
    }

    void setConstrast(uint8_t value)
    {
        sendCommand(COMMAND_CONTRAST);
        sendCommand(value);
    }

    void setColumnStartAddressPaging(uint8_t address)
    {
        sendCommand(COMMAND_PAGE_MODE_SET_LOW_COL_START | static_cast<uint8_t>(address & 0x0F));
        sendCommand(COMMAND_PAGE_MODE_SET_HIGH_COL_START | static_cast<uint8_t>(address >> 4));
    }

    void setAddresingMode(AddressingMode mode)
    {
        sendCommand(COMMAND_ADDRESSING_MODE);
        sendCommand(static_cast<uint8_t>(mode));
    }

    void setColumnAddress(uint8_t start, uint8_t end)
    {
        sendCommand(COMMAND_COLUMN_ADDRESS);
        sendCommand(start);
        sendCommand(end);
    }

    void setPageAddress(uint8_t start, uint8_t end)
    {
        sendCommand(COMMAND_SET_PAGE_ADDRESS);
        sendCommand(start & 0x07);
        sendCommand(end & 0x07);
    }

    void setPageStartPaging(uint8_t address)
    {
        sendCommand(COMMAND_SET_PAGE_START_ADDRESS | static_cast<uint8_t>(address & 0x07));
    }

    void setDisplayStartLine(uint8_t line)
    {
        sendCommand(COMMAND_DISPLAY_START_LINE | (line & 0x3F));
    }

    void setSegmentRemap(uint8_t ratio)
    {
        sendCommand(COMMAND_MULTIPLEX_RATIO);
        sendCommand(ratio & 0x3F);
    }

    void setComDirection(bool remap)
    {
        sendCommand(COMMAND_COM_SCAN_DIRECTION | (static_cast<uint8_t>(remap) << 3));
    }

    void setDisplayOffset(uint8_t offset)
    {
        sendCommand(COMMAND_DISPLAY_OFFSET);
        sendCommand(offset & 0x3F);
    }

    void setCommConfig(uint8_t data)
    {
        sendCommand(COMMAND_COM_CONFIG);
        sendCommand(data << 4);
    }

    void setClockConfig(uint8_t divide, uint8_t freq)
    {
        sendCommand(COMMAND_DISPLAY_CLOCK_DIVIDE);
        sendCommand(((freq & 0x0F) << 4) | (divide & 0x0F));
    }

    void setPrecharge(uint8_t data)
    {
        sendCommand(COMMAND_PRE_CHARGE_PERIOD);
        sendCommand(data);
    }

    void setDeselect(uint8_t data)
    {
        sendCommand(COMMAND_DESELECT_LEVEL);
        sendCommand(data);
    }

    void nop()
    {
        sendCommand(COMMAND_NOP);
    }

private:
    void sendCommand(uint8_t cmd)
    {
        // i2c::start(address_, i2c::SlaMode::Write);
        // i2c::write(CONTROL_COMMAND);
        // i2c::write(cmd);
        // i2c::stop();
    }

    ftl::comms::i2c::I2CDevice<I2C> device_;
};

}
}
}

#endif
