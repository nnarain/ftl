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
#include <ftl/memory/memreader.hpp>
#include <ftl/utils/bitutil.hpp>

#include <ftl/drivers/displays/ssd1306.hpp>


namespace ftl
{
namespace gfx
{

/**
 * SSD1306 Display Adaptor
*/
template<typename T, typename GfxReader = memory::DefaultMemoryReader>
class Ssd1306Display : public RasterDisplay<GfxReader>
{
public:
    static constexpr uint8_t NUM_COLUMNS = 128;
    static constexpr uint8_t NUM_PAGES = 8;
    static constexpr uint8_t NUM_ROWS_PER_PAGE = 8;

    Ssd1306Display(uint8_t i2c_address, uint8_t height)
        : driver_{i2c_address, height}
    {
        clear();
    }

    /**
     * Create an instance of an SSD1306 display.
    */
    Ssd1306Display(uint8_t i2c_address)
        : Ssd1306Display{i2c_address, 64}
    {
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
        auto& page = framebuffer_[row / NUM_ROWS_PER_PAGE];
        const auto page_row = row % 8;

        // FORCE(page[col], 1 << page_row, c.monochrome() << page_row);
        page[col] = (page[col] & ~(1 << page_row)) | (c.monochrome() << page_row);
    }

    /**
     * Update the display with the current framebuffer
    */
    void update() override
    {
        // Set the column address bounds to the entire display
        driver_.setColumnAddress(0, 127);
        driver_.setPageAddress(0, 7);
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
