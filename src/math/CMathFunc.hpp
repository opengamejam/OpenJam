//
//  CMathFunc.h
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 2/7/15.
//
//

#ifndef CMATHFUNC_HPP
#define CMATHFUNC_HPP

#include "Global.h"
#include "CStringUtils.h"

namespace jam {

template <typename T>
INL static T clamp(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(const std::tuple<Tp...>&, const std::function<void(uint8_t*, uint64_t)>&)
{
}

template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if < I<sizeof...(Tp), void>::type
                                     for_each(const std::tuple<Tp...>& t, const std::function<void(uint8_t*, uint64_t)>& f)
{
    auto data = std::get<I>(t);
    f((uint8_t*)&data, (uint64_t)sizeof(data));
    for_each<I + 1, Tp...>(t, f);
}

template <typename... T>
INL uint64_t hash_tuple(const std::tuple<T...>& arg)
{
    std::function<uint64_t(uint64_t, const uint8_t*, uint64_t)> hash_combine = [](uint64_t seed,
        const uint8_t* data,
        uint64_t size) -> uint64_t {
        for (uint64_t i = 0; i < size; ++i) {
            seed = (seed * 16777619ull) ^ data[i];
        }

        return seed;
    };

    uint64_t h = 2166136261ull;
    for_each(arg, [&](uint8_t* data, uint64_t size) {
        h = hash_combine(h, data, size);
    });

    return h;
}

}; // namespace jam

#endif /* defined(CMATHFUNC_HPP) */
