//
// color.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 25 2020
//
#ifndef FTL_GFX_COLOR_HPP
#define FTL_GFX_COLOR_HPP

#include <stdint.h>

namespace ftl
{
namespace gfx
{
    struct Color
    {
        Color(uint8_t r, uint8_t g, uint8_t b)
            : r{r}, g{g}, b{b}
        {
        }

        Color() : Color{0, 0, 0}
        {
        }

        uint8_t monochrome() const
        {
            // If the Color is not (0, 0, 0) return 1
            return (r != 0 && g != 0 && b != 0);
        }

        /* Color constants */

        static Color black()
        {
            return Color{};
        }

        static Color white()
        {
            return Color{255, 255, 255};
        }

        static Color red()
        {
            return Color{255, 0, 0};
        }

        static Color green()
        {
            return Color{0, 255, 0};
        }

        static Color blue()
        {
            return Color{0, 0, 255};
        }

        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
}
}

#endif
