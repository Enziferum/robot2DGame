#pragma once

#include <cstdint>
#include <vector>

namespace robot2D {

    enum class AudioFormatType: int {
        None = -1, MONO8 = 0, MONO16, STEREO8, STEREO16
    };

    struct AudioData {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        std::int32_t audioSize;
        AudioFormatType audioFormatType;
    };
}