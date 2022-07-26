#pragma once

#include <random>

namespace destroid::random {

inline std::minstd_rand& engine() noexcept
{
    static auto rng = std::minstd_rand {std::random_device()()};
    return rng;
}

inline float either(float a, float b) noexcept
{
    std::bernoulli_distribution bdist(0.5);
    return bdist(engine()) ? a : b;
}

inline float between(float min, float max) noexcept
{
    std::uniform_real_distribution<float> ufdist(min, max);
    return ufdist(engine());
}

inline int between(int min, int max) noexcept
{
    std::uniform_int_distribution<int> uidist(min, max);
    return uidist(engine());
}

} // namespace destroid::random
