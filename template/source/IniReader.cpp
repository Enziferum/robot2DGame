#include <fstream>
#include <algorithm>

#include "IniReader.hpp"

namespace mobb {
    std::vector<std::string> split(const std::string& str, std::string separator) {
        std::vector<std::string> res;
        const auto sepSize = separator.size();
        std::string::size_type start{0}, end;

        while((end = str.find(separator, start)) != std::string::npos) {
            res.emplace_back(str.substr(start, end - start));
            start = end + sepSize;
        }

        res.emplace_back(str.substr(start));

        return res;
    }

    std::pair<std::string, std::string> divide(const std::string& str, char sep) {
        auto found = str.find(sep);
        if(found == std::string::npos)
            return {};
        return {str.substr(0, found), str.substr(found+1, str.size())};
    }

    inline std::string& removeWhitespaces(std::string& str) {
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        return str;
    }


    template<>
    std::string Value::as() const {
        return m_value;
    }

    template<>
    int Value::as() const {
        return std::stoi(m_value);
    }

    template<>
    unsigned int Value::as() const {
        return static_cast<unsigned int>(std::stoi(m_value));
    }

    template<>
    bool Value::as() const {
        return static_cast<bool>(std::stoi(m_value));
    }

    template<>
    float Value::as() const {
        return std::stof(m_value);
    }

    template<>
    double Value::as() const {
        return std::stod(m_value);
    }



    bool IniReader::loadFromFile(const std::string& path) {
        std::fstream file{path};
        if(!file.is_open())
            return false;

        bool mustHaveSection = true;
        bool hasSection{false};

        std::string line;
        Section currSection{};

        while(std::getline(file, line)) {
            // comments
            if(line[0] == '#' || line[0] == ';')
                continue;

            // section tag
            if(line[0] == '['){
                auto index = line.find(']');
                auto name = line.substr(1, index - 1);
                if(hasSection) {
                    m_sections[currSection.m_name] = currSection;
                }
                hasSection = true;
                currSection = Section{name};
            }
            else {
                if(mustHaveSection && !hasSection) {
                    throw(BadFormat{});
                }

                line = removeWhitespaces(line);
                auto [name, value] = divide(line, '=');
                if(name.empty()) {
                    // error
                }
                currSection[name] = Value{value};
            }
        }

        return true;
    }

    bool IniReader::saveToFile(const std::string& path) {
        //std::ostream file{path};
        for(const auto& section: m_sections) {

        }

        return true;
    }

    bool IniReader::loadFromMemory(void* buffer, std::size_t bufferSize) {
        return true;
    }

}