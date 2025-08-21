#include <DxLib.h>
#include "misc/utility_dx.hpp"

namespace utility
{
    dx_math::Vector2D_I GetCenterScreen() noexcept
    {
        int x, y;
        GetWindowSize(&x, &y);
        
        dx_math::Vector2D_I center(x / 2, y / 2);
        return center;
    }
}