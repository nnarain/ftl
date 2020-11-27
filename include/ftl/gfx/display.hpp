//
// display.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 26 2020
//
#ifndef FTL_GFX_DISPLAY_HPP
#define FTL_GFX_DISPLAY_HPP

#include <ftl/gfx/color.hpp>

// FIXME: Can I assume this header exists?
#include <stdlib.h>

namespace ftl
{
namespace gfx
{
    /**
     * Abstraction for raster displays.
     * 
     * Implementors must provide a method of setting a single pixel and a method to update the display.
     * All other functions can be overridden for display specific optimizations.
    */
    class RasterDisplay
    {
    public:
        /**
         * Set a pixel in the display
        */
        virtual void drawPixel(unsigned int col, unsigned int row, const Color& pixel) = 0;

        /**
         * Update the display
        */
        virtual void update() = 0;

        /**
         * Draw a line
         * 
         * Implements Bresenham's algorithm https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        */
        virtual void drawLine(int x0, int y0, int x1, int y1, const Color& c)
        {
            const auto dx = abs(x1 - x0);
            const auto sx = x0 < x1 ? 1 : -1;
            const auto dy = -abs(y1 - y0);
            const auto sy = y0 < y1 ? 1 : -1;

            auto err = dx + dy;

            while (true)
            {
                drawPixel(x0, y0, c);
                // Check if at the end of the line
                if (x0 == x1 && y0 == y1) break;

                const auto e2 = 2 * err;
                if (e2 >= dy)
                {
                    err += dy;
                    x0 += sx;
                }

                if (e2 <= dx)
                {
                    err += dx;
                    y0 += sy;
                }
            }
            
        }

        /**
         * Draw a rectangle
        */
        virtual void drawRect(int x0, int y0, int w, int h, const Color& c)
        {
            // Draw top
            drawLine(x0, y0, x0 + w, y0, c);
            // Draw bottom
            drawLine(x0, y0 + h, x0 + w, y0 + h, c);
            // Draw left
            drawLine(x0, y0, x0, y0 + h, c);
            // Draw right
            drawLine(x0 + w, y0, x0 + w, y0 + h, c);
        }

    private:
    };
}
}

#endif
