#pragma once
#include <fstream>
#include "IAudioReader.hpp"

namespace robot2D {
    class WaveReader final : public IAudioReader {
    public:
        WaveReader();
        ~WaveReader() override = default;

        bool loadFromFile(const std::string& path, std::vector<char>& buffer) override;
        //bool openFromFile(const std::string& path, const FileInputStream& fileInputStream) override;
        bool parseHeader(FileInputStream& fileInputStream) override;
    private:
        bool checkHeader(std::ifstream& file, unsigned int &size);
    };

}