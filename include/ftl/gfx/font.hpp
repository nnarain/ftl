//
// font.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//

#ifndef FTL_GFX_FONT_HPP
#define FTL_GFX_FONT_HPP

// #include <ftl/gfx/color.hpp>
#include <stdint.h>

namespace ftl
{
namespace gfx
{
/**
 * Handle Font data
 * 
 * Currently this assumes 8x8 tiles
*/
class Font
{
public:
    Font(const uint8_t* data)
        : font_data_{data}
    {
    }

    /**
     * Return if the pixel at the specified location in the glyph
    */
    template<typename DataReader>
    bool at(char c, unsigned int x, unsigned int y, const DataReader& reader) const
    {
        if (!font_data_) return false;

        const auto offset = (unsigned int)c * 8;
        const uint8_t data = reader(font_data_, offset + y);

        return !!(data & (1 << x));
    }

    unsigned int width() const
    {
        return 8;
    }

    unsigned int height() const
    {
        return 8;
    }

private:
    const uint8_t* font_data_{nullptr};
};
}
}

#endif // FTL_GFX_FONT_HPP
