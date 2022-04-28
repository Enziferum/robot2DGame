#pragma once

#include <cassert>
#include <unordered_map>

namespace robot2D {

    enum class AppContextID {
        Configuration,
        Audio,
        Window
    };

    struct AppContext {
        AppContext() = default;
        ~AppContext() = default;

        template<typename T>
        bool store(const AppContextID& id, T* object) {
            assert(object != nullptr && "Storing object in AppContext must be not null");
            auto buffer = static_cast<void*>(object);
            return m_buffer.insert(std::pair<AppContextID, void*>(id, buffer)).second;
        }

        template<typename T>
        T& get(const AppContextID& id) {
            return *(static_cast<T *>(m_buffer[id]));
        }
    private:
        std::unordered_map<AppContextID, void* > m_buffer;
    };


}