#pragma once

#include <vector>

namespace robot2D {
    struct AudioData {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;

        std::vector<char> soundData;
    };
}