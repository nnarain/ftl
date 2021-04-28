//
// range.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 27 2021
//

namespace ftl
{
namespace math
{

inline float scale(float x, float from_min, float from_max, float to_min, float to_max)
{
    return (((to_max - to_min) * (x - from_min)) / (from_max - from_min)) + to_min;
}

}
}
