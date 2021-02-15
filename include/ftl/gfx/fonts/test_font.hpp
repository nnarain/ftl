//
// test_font.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//

#ifndef FTL_GFX_FONTS_TEST_FONT_HPP
#define FTL_GFX_FONTS_TEST_FONT_HPP

#include <ftl/gfx/font.hpp>

#include <stdint.h>

namespace ftl
{
namespace gfx
{
namespace fonts
{

const uint8_t TEST_FONT_DATA[] = {
    0xFF,
    0x81,
    0x81,
    0x81,
    0x81,
    0x81,
    0x81,
    0xFF
};

const Glyph TEST_FONT_GLYPHS[] = {
    {&TEST_FONT_DATA[0], 0, 8, 8},
};

const Font TEST_FONT{TEST_FONT_GLYPHS};

}
}
}

#endif // FTL_GFX_FONTS_TEST_FONT_HPP
