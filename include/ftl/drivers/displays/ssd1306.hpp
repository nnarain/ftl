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
// Memory Addressing Mode
enum class Ssd1306_AddressingMode
{
    Horizontal = 0x00,
    Vertical   = 0x01,
    Page       = 0x02
};

// Scroll mode
enum class Ssd1306_ScrollDirection
{
    Right,
    Left,
};

// Scroll timing
enum class Ssd1306_FrameInterval
{
    FRAME_5   = 0x00,
    FRAME_64  = 0x01,
    FRAME_128 = 0x02,
    FRAME_256 = 0x03,
    FRAME_3   = 0x04,
    FRAME_4   = 0x05,
    FRAME_25  = 0x06,
    FRAME_2   = 0x07,
};

/**
 * SSD1306 - OLED display
 * 
 * I2C Bus Frame
 * 
 * |SLA+RW|Co:D/C#:000000|Data|Co:D/C#:0..0|...
 * 
 * Co   - continuation bit
 *      - If set to logic 0, transmission of the following information will contain data bytes only
 * D/C# - Data / Command selection bit
 *      - Determines if the next byte acts as a command or as data
 *      - Logic 0: The following byte is a command
 *      - Logic 1: The following byte is data for GDDRAM
 * 
 * GDDRAM is divided into 8 pages (PAGE0 - PAGE7).
 * COM0 - COM63 refers to pixel row.
 * SEG0 - SEG127 refers to pixel column
 * 
 * Both can be re-mapped to reverse the order.
*/
template<class I2C>
class Ssd1306
{
    static constexpr uint8_t CONTROL_COMMAND = 0x00;
    static constexpr uint8_t CONTROL_DATA = 0x40;

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

    // Continuous vertical and horizontal scroll
    static constexpr uint8_t COMMAND_SCROLL_SETUP = 0x29;
    // Scroll activation
    static constexpr uint8_t COMMAND_SCROLL_ACTIVATION = 0x2E;
    // Scroll horizontal
    static constexpr uint8_t COMMAND_SCROLL_HORIZONTAL = 0x26;
    // Vertical Scroll Area
    static constexpr uint8_t COMMAND_SET_VERTICAL_SCROLL_AREA = 0xAE;

    // Charge Pump
    static constexpr uint8_t COMMAND_CHARGE_PUMP = 0x8D;

    static constexpr uint8_t NUM_PAGES = 8;
    static constexpr uint8_t ROW_PER_PAGE = 8;
    static constexpr uint8_t COL_PER_PAGE = 128;

public:
    static constexpr uint8_t WIDTH = 128;
    // static constexpr uint8_t HEIGHT = 64;

    Ssd1306(uint8_t address, uint8_t height)
        : device_{address}
        , height_{height}
    {
    }

    Ssd1306(uint8_t address)
        : Ssd1306{address, 64}
    {
    }

    bool initialize()
    {
        if (!device_.detect())
        {
            // Failed to detect
            return false;
        }

        enable(false);
        setDisplayOffset(0);
        setDisplayStartLine(0);
        setSegmentRemap(true);
        setComScanReverse(false);
        if (height_ == 64)
        {
            setComConfig(true, false);
        }
        else
        {
            // height = 32
            setComConfig(false, false);
        }
        setConstrast(0x7F);
        setClockConfig(0x00, 0x08);
        setMultiplexRatio(height_ - 1);
        enableChargePump(true);
        invert(false);
        scroll(false);
        enable(true);
        resume();

        clear();

        return true;
    }

    /**
     * Zero out display RAM
    */
    void clear()
    {
        // Clear the display by writing a zeroed out page buffer
        uint8_t page_buf[WIDTH] = {0};

        setAddresingMode(Ssd1306_AddressingMode::Page);

        for (auto i = 0u; i < NUM_PAGES; ++i)
        {
            setPageStart(i);
            sendBuffer(page_buf, sizeof(page_buf));
        }
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

    /**
     * Set the column starting address for page addressing mode
    */
    void setColumnStartAddressPaging(uint8_t address)
    {
        sendCommand(COMMAND_PAGE_MODE_SET_LOW_COL_START | static_cast<uint8_t>(address & 0x0F));
        sendCommand(COMMAND_PAGE_MODE_SET_HIGH_COL_START | static_cast<uint8_t>(address >> 4));
    }

    void setAddresingMode(Ssd1306_AddressingMode mode)
    {
        sendCommand(COMMAND_ADDRESSING_MODE);
        sendCommand(static_cast<uint8_t>(mode));
    }

    /**
     * Set start and end column addresses
     * RESET:
     *  start = 0
     *  end = 127
    */
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

    void setPageStart(uint8_t address)
    {
        sendCommand(COMMAND_SET_PAGE_START_ADDRESS | static_cast<uint8_t>(address & 0x07));
    }

    /**
     * Set segment remap
     * 
     * If true, column address 0 is mapped to SEG0 (RESET). If false, column address 127 is mapped to SEG0
    */
    void setSegmentRemap(bool remap)
    {
        sendCommand(COMMAND_SEGMENT_REMAP | static_cast<uint8_t>(remap));
    }

    /**
     * Set the starting line
    */
    void setDisplayStartLine(uint8_t line)
    {
        sendCommand(COMMAND_DISPLAY_START_LINE | (line & 0x3F));
    }

    void setMultiplexRatio(uint8_t ratio)
    {
        sendCommand(COMMAND_MULTIPLEX_RATIO);
        sendCommand(ratio & 0x3F);
    }

    /**
     * Set COM Scan Direction.
     * 
     * If true, sets the scan direction from COM[N-1] to COM0. Otherwise, COM0 to COM[N-1] (Reset).
    */
    void setComScanReverse(bool r)
    {
        sendCommand(COMMAND_COM_SCAN_DIRECTION | (static_cast<uint8_t>(r) << 3));
    }

    void setDisplayOffset(uint8_t offset)
    {
        sendCommand(COMMAND_DISPLAY_OFFSET);
        sendCommand(offset & 0x3F);
    }

    /**
     * Set COM configuration
     * 
     * @param com_alt Use alternative COM pin configuration
     * @param left_right_remap Enable left right remap
    */
    void setComConfig(bool com_alt, bool left_right_remap)
    {
        const auto config = (static_cast<uint8_t>(com_alt) << 4)
                          | (static_cast<uint8_t>(left_right_remap) << 5)
                          | 0x02;
        sendCommand(COMMAND_COM_CONFIG);
        sendCommand(config);
    }

    /**
     * Set divide ratio and oscillator frequency
     * 
     * Divide ratio is 0-15.
     * Frequency is 0-15.
    */
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

    /**
     * Enable the charge pump.
     * 
     * Enable internal voltage supply
    */
    void enableChargePump(bool enabled)
    {
        sendCommand(COMMAND_CHARGE_PUMP);
        // 0x14 to enable
        // 0x10 to disable
        sendCommand(0x10 | (static_cast<uint8_t>(enabled) << 2));
    }

    void nop()
    {
        sendCommand(COMMAND_NOP);
    }

    /**
     * Activate scroll
    */
    void scroll(bool e)
    {
        sendCommand(COMMAND_SCROLL_ACTIVATION | static_cast<uint8_t>(e));
    }

    /**
     * Setup continuous vertical and horizontal scrolling
    */
    void setupVerticalAndHorizontalScroll(Ssd1306_ScrollDirection dir, uint8_t page_start, uint8_t page_end, Ssd1306_FrameInterval interval, uint8_t scroll_offset)
    {
        uint8_t mode = (dir == Ssd1306_ScrollDirection::Right) ? 0x29 : 0x2A;
        // Set the scroll mode
        sendCommand(static_cast<uint8_t>(mode));
        // Dummy byte
        sendCommand(0x00);
        // Set the start page address
        sendCommand(page_start & 0x07);
        // Set frame interval between scroll steps
        sendCommand(static_cast<uint8_t>(interval));
        // Set page end
        sendCommand(page_end & 0x07);
        // Set the vertical scroll offset
        sendCommand(scroll_offset & 0x3F);
    }

    /**
     * Setup continuous horizontal scroll
    */
    void setupHorizontalScroll(Ssd1306_ScrollDirection dir, uint8_t page_start, uint8_t page_end, Ssd1306_FrameInterval interval)
    {
        // Send horizontal scroll with the direction bit
        sendCommand(COMMAND_SCROLL_HORIZONTAL | static_cast<uint8_t>(dir));
        // dummy byte
        sendCommand(0x00);
        // set the start page
        sendCommand(page_start & 0x07);
        // set the frame interval between scroll sets
        sendCommand(static_cast<uint8_t>(interval));
        // set the end page
        sendCommand(page_end & 0x07);
        // dummy bytes
        sendCommand(0x00);
        sendCommand(0xFF);
    }

    /**
     * Set vertical scroll area
    */
    void setVerticalScrollArea(uint8_t top_fixed_rows, uint8_t scroll_area_rows)
    {
        sendCommand(COMMAND_SET_VERTICAL_SCROLL_AREA);
        sendCommand(top_fixed_rows & 0x3F);
        sendCommand(scroll_area_rows & 0x7F);
    }

    /**
     * Send a data buffer GDDRAM
    */
    void sendBuffer(const uint8_t* buffer, unsigned long length)
    {
        if (device_.begin(comms::i2c::SlaMode::Write))
        {
            device_.write(CONTROL_DATA);
            for (auto i = 0u; i < length; ++i)
            {
                device_.write(buffer[i]);
            }
            device_.end();
        }
    }

    void sendByte(uint8_t data)
    {
        sendBuffer(&data, 1);
    }

private:
    /**
     * Send a command to the display
    */
    void sendCommand(uint8_t cmd)
    {
        device_.begin(comms::i2c::SlaMode::Write);
        device_.write(CONTROL_COMMAND);
        device_.write(cmd);
        device_.end();
    }

    ftl::comms::i2c::I2CDevice<I2C> device_;
    uint8_t height_;
};

}
}

#endif
