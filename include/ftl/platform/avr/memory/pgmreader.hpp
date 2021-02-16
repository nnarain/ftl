//
// pgmreader.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//
#ifndef FTL_PLATFORM_AVR_MEMORY_PGMREADER_HPP
#define FTL_PLATFORM_AVR_MEMORY_PGMREADER_HPP

#include <avr/pgmspace.h>

namespace ftl
{
namespace platform
{
namespace avr
{

/**
 * Read a byte from flash memory
*/
struct PgmSpaceReader
{
    uint8_t operator()(const uint8_t* ptr, unsigned int offset) const
    {
        return pgm_read_byte(ptr + offset);
    }
};

}
}
}


#endif // FTL_PLATFORM_AVR_MEMORY_PGMREADER_HPP
