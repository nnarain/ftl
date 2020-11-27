//
// ssd1306_display.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 25 2020
//

#ifndef FTL_GFX_SSD1306_DISPLAY_HPP
#define FTL_GFX_SSD1306_DISPLAY_HPP

#include <stdint.h>


#include <ftl/gfx/display.hpp>
#include <ftl/gfx/color.hpp>
#include <ftl/utils/bitutil.hpp>

#include <ftl/drivers/displays/ssd1306.hpp>


namespace ftl
{
namespace gfx
{

/**
 * SSD1306 Display Adaptor
*/
template<typename T>
class Ssd1306Display : public RasterDisplay
{
public:
    static constexpr uint8_t NUM_COLUMNS = 128;
    static constexpr uint8_t NUM_PAGES = 8;

    /**
     * Create an instance of an SSD1306 display.
    */
    Ssd1306Display(uint8_t i2c_address)
        : driver_{i2c_address}
    {
        clear();
    }

    bool initialize(bool com_reverse = true)
    {
        if (!driver_.initialize())
        {
            return false;
        }

        driver_.setComScanReverse(com_reverse);
        driver_.setAddresingMode(drivers::Ssd1306_AddressingMode::Horizontal);
        driver_.setPageAddress(0, 7);

        return true;
    }

    void drawPixel(unsigned int col, unsigned int row, const Color& c) override
    {
        auto& page = framebuffer_[row / 8];
        const auto page_row = row % 8;

        FORCE(page[col], 1 << page_row, c.monochrome() << page_row);
    }

    /**
     * Update the display with the current framebuffer
    */
    void update() override
    {
        // Set the column address bounds to the entire display
        driver_.setColumnAddress(0, 127);
        driver_.sendBuffer(&framebuffer_[0][0], sizeof(framebuffer_));
    }

    void clear()
    {
        // zero framebuffer
        for (auto page = 0u; page < NUM_PAGES; ++page)
        {
            for (auto col = 0u; col < NUM_COLUMNS; ++col)
            {
                framebuffer_[page][col] = 0x00;
            }
        }
    }

    /**
     * @return the underlying display driver
    */
    drivers::Ssd1306<T>& getDriver()
    {
        return driver_;
    }
private:
    using PageBuffer = uint8_t[NUM_COLUMNS];
    using FrameBuffer = PageBuffer[NUM_PAGES];

    // TODO: Allocate on heap?
    FrameBuffer framebuffer_;

    drivers::Ssd1306<T> driver_;
};

}
}

#endif
