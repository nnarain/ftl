//
// font.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//

#ifndef FTL_GFX_FONT_HPP
#define FTL_GFX_FONT_HPP

#include <stdint.h>

namespace ftl
{
namespace gfx
{
/**
 * A single graphical representation within the font
*/
struct Glyph
{
    const uint8_t* source;
    unsigned int offset;
    unsigned int width;
    unsigned int height;

    template<typename DataReader>
    bool at(unsigned int x, unsigned int y, const DataReader& reader) const
    {
        const uint8_t data = reader(source, offset + y);
        return (data & (0x80 >> x));
    }
};

/**
 * A collections of glyphs
*/
class Font
{
    static constexpr uint8_t ASCII_START = 32;
    static constexpr uint8_t ASCII_END = 127;

public:
    Font(const Glyph* glyphs)
        : glyphs_{glyphs}
    {
    }

    const Glyph* glyph(char c) const
    {
        if (!glyphs_) return nullptr;

        if (c >= ASCII_START && c <= ASCII_END)
        {
            // return &glyphs_[c - ASCII_START];
            return &glyphs_[0];
        }
        else
        {
            return nullptr;
        }
    }

private:
    const Glyph* glyphs_{nullptr};
};
}
}

#endif // FTL_GFX_FONT_HPP
