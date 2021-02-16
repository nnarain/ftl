//
// memreader.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 15 2021
//

#ifndef FTL_MEMORY_MEMREADER_HPP
#define FTL_MEMORY_MEMREADER_HPP

#include <stdint.h>

namespace ftl
{
namespace memory
{

/**
 * Abstract reading memory
 * For example, reading from a pointer or reading from flash memory
*/

/**
 * Simple read from a pointer using an offset
*/
struct DefaultMemoryReader
{
    uint8_t operator()(const uint8_t* ptr, unsigned int offset) const
    {
        return ptr[offset];
    }
};

}
} // namespace ftl


#endif // FTL_MEMORY_MEMREADER_HPP
