#pragma once
#include <random>

template<typename T>
T random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}

template<typename T>
T randomf(T range_from, T range_to) {
    return range_from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (range_to - range_from)));
}