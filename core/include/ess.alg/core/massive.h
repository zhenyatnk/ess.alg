#pragma once

#include <ess.alg/core/BaseExceptions.h>
#include <ess.alg/core/export.h>

#include <vector>
#include <cassert>

namespace ess_alg {
namespace core {

template<typename T> using massive_t = std::vector<T>;

template<typename T>
T FindMedian(const massive_t<T>& massive)
{
    for(const auto& element: massive)
    {
        uint32_t greatCount = 0;
        uint32_t lessCount = 0;
        for(const auto& current: massive)
        {
            if(current > element)
                ++greatCount;
            if(current < element)
                ++lessCount;
        }

        if(!!greatCount && greatCount == lessCount)
            return element;
    }
    return {};
}


}
}
