#pragma once
#include <fstream>
#include "IAudioReader.hpp"

namespace robot2D {
    class WaveReader final : public IAudioReader {
    public:
        WaveReader();
        ~WaveReader() override = default;

        bool loadFromFile(const std::string &path) override;
    private:
        bool checkHeader(std::ifstream& file, unsigned int &size);
    };

}