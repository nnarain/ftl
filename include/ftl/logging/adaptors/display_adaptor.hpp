//
// display_adaptor.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//

#include <ftl/gfx/display.hpp>
#include <ftl/gfx/color.hpp>

namespace ftl
{
namespace logging
{

template<class DisplayT>
class RasterDisplayLoggerAdaptor
{
public:
    template<typename... Args>
    RasterDisplayLoggerAdaptor(Args... args)
        : display_{args...}
        , y_offset_{0}
    {
        display_.initialize();
    }

    void write(const char* str)
    {
        display_.drawString(str, 0, y_offset_, gfx::Color::white());
        y_offset_ = (y_offset_ + display_.font()->height()) % display_.height();

        display_.update();
    }

    DisplayT& getDisplay()
    {
        return display_;
    }

private:
    DisplayT display_;
    unsigned int y_offset_;
};

}
}