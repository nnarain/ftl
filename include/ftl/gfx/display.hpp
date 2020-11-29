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
        virtual void drawPixel(unsigned int col, unsigned int row, const Color& c) = 0;

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
         * Draw a vertical line
        */
        virtual void drawVLine(int x0, int y0, int h, const Color& c)
        {
            drawLine(x0, y0, x0, y0 + h, c);
        }

        /**
         * Draw a horizontal line
        */
        virtual void drawHLine(int x0, int y0, int w, const Color& c)
        {
            drawLine(x0, y0, x0 + w, y0, c);
        }

        /**
         * Draw a rectangle
        */
        virtual void drawRect(int x0, int y0, int w, int h, const Color& c)
        {
            // Draw top
            drawHLine(x0, y0, w, c);
            // Draw bottom
            drawHLine(x0, y0 + h, w, c);
            // Draw left
            drawVLine(x0, y0, h, c);
            // Draw right
            drawVLine(x0 + w, y0, h, c);
        }

        /**
         * Draw fill rectangle
        */
        virtual void drawFillRect(int x0, int y0, int w, int h, const Color& c)
        {
            const auto x1 = x0 + w;
            const auto y1 = y0 + h;
            for (auto x = x0; x <= x1; ++x)
            {
                for (auto y = y0; y <= y1; ++y)
                {
                    drawPixel(x, y, c);
                }
            }
        }

        // /**
        //  * Draw circle
        // */
        // virtual void drawCircle(int xc, int yc, int r, const Color& c)
        // {
        //     const auto x = 0;
        //     const auto y = r;
        // }

        /**
         * Draw a bitmap in XBITMAP format
        */
        virtual void drawXBitmap(const uint8_t* const bitmap, int x, int y, int w, int h, const Color& c)
        {
            unsigned int idx = 0;
            uint8_t byte = bitmap[idx++];

            const auto y1 = y + h;
            const auto x1 = x + w;

            // Iterate over rows
            for (auto row = y; row < y1; ++row)
            {
                auto scan = 0;
                // Iterate over columns
                for (auto col = x; col < x1; ++col)
                {
                    if (byte & 0x01)
                    {
                        drawPixel(col, row, c);
                    }

                    scan++;

                    if ((col + 1) & 0x07)
                    {
                        byte >>= 1;
                    }
                    else
                    {
                        byte = bitmap[idx++];
                    }
                }

                // Extra bit not used in the byte are ignored
                if (w % 8 != 0)
                {
                    byte = bitmap[idx++];
                }
            }

        }

    private:
    };
}
}

#endif
