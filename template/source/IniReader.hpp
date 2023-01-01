#pragma once
#include <vector>
#include <unordered_map>

namespace mobb {

    struct BadFormat: std::exception {};

    std::vector<std::string> split(const std::string& str, std::string separator);

    std::pair<std::string, std::string> divide(const std::string& str, char sep);

    inline std::string& removeWhitespaces(std::string& str);

    class Value {
    public:
        Value(): m_value{""}{}
        Value(std::string value): m_value{value} {}
        ~Value() = default;

        template<typename T>
        T as() const;
    private:
        std::string m_value;
    };

    class Section {
    public:
        Section(): m_name{}{}
        explicit Section(std::string name): m_name{name} {}
        ~Section() = default;

        const std::string& getName() const { m_name; }

        bool hasValue(const std::string& name) const {
            return m_values.find(name) != m_values.end();
        }

        Value& operator[](std::string key) {
            return m_values[key];
        }

        const Value& operator[](std::string key) const {
            return m_values.at(key);
        }
    private:
        std::string m_name;
        friend class IniReader;
        std::unordered_map<std::string, Value> m_values;
    };

    class IniReader {
    public:
        IniReader() = default;
        ~IniReader() = default;

        bool loadFromFile(const std::string& path);
        bool saveToFile(const std::string& path);


        bool loadFromMemory(void* buffer, std::size_t bufferSize);

        bool hasSection(const std::string& name) const {
            return m_sections.find(name) != m_sections.end();
        }

        Section& operator[](std::string key) {
            return m_sections[key];
        }

        const Section& operator[](std::string key) const {
            return m_sections.at(key);
        }
    private:
        std::unordered_map<std::string, Section> m_sections;
    };
}