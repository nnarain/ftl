//
// display.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 26 2020
//
#ifndef FTL_GFX_DISPLAY_HPP
#define FTL_GFX_DISPLAY_HPP

#include <ftl/gfx/color.hpp>
#include <ftl/memory/memreader.hpp>
#include <ftl/gfx/font.hpp>

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
     * 
     * GfxDataReader - Because graphics data may be stored in a variety of ways on an embedded platform, provide a method
     *                 for different targets to override how data is read (e.g. from flash)
    */
    template<class GfxDataReader = memory::DefaultMemoryReader>
    class RasterDisplay
    {
    public:
        RasterDisplay(unsigned int width, unsigned int height)
            : width_{width}, height_{height}
        {
        }

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
            const auto bytes_per_row = w / 8;

            for (auto j = 0; j < h; ++j)
            {
                const auto row = y + j;
                uint8_t byte = 0;

                for (auto i = 0; i < w; ++i)
                {
                    if (i & 7)
                    {
                        // Processing a byte, shift 1 down to the next pixel
                        byte >>= 1;
                    }
                    else
                    {
                        // New byte
                        // Skip bytes until the current row + byte offset for columns (1 bit per column pixel)
                        byte = gfx_reader_(bitmap, j * bytes_per_row + i / 8);
                    }

                    if (byte & 0x01)
                    {
                        drawPixel(x + i, row, c);
                    }
                }
            }
        }

        /**
         * Draw a string
        */
        void drawString(const char* str, int x, int y, const gfx::Color& color)
        {
            while(*str)
            {
                const char c = *str++;

                if (c == '\n')
                {
                    // Next line
                    x = 0;
                    y = (y + font_->height()) % height_;
                }
                else if (c == '\r')
                {
                    // Return to start of line
                    x = 0;
                    y = 0;
                }
                else
                {
                    drawChar(c, x, y, color);
                    x += font_->width();
                }
            }
        }

        /**
         * Draw a single character from the set font (A font must be set!)
        */
        void drawChar(const char c, int x, int y, const gfx::Color& color)
        {
            if (!font_) return;

            for (auto i = 0; i < font_->width(); ++i)
            {
                for (auto j = 0; j < font_->height(); ++j)
                {
                    if (font_->at(c, i, j, gfx_reader_))
                    {
                        drawPixel(x + i, y + j, color);
                    }
                    else
                    {
                        drawPixel(x + i, y + j, gfx::Color::black());
                    }
                }
            }
        }

        /**
         * Set the font the display uses
        */
        void setFont(const Font* font)
        {
            font_ = font;
        }

        unsigned int width() const
        {
            return width_;
        }

        unsigned int height() const
        {
            return height_;
        }

        const Font* font() const
        {
            return font_;
        }

    private:
        GfxDataReader gfx_reader_;
        const Font* font_{nullptr};
        unsigned int width_;
        unsigned int height_;
    };
}
}

#endif
