#pragma once

#include <thread>
#include <mutex>
#include <robot2D/Core/FileInputStream.hpp>

#include "SoundStream.hpp"


namespace robot2D {

    class Music: public robot2D::SoundStream {
    public:
        Music() = default;
        ~Music() override = default;

        /// \brief Actually open filestream and read information about audio file.
        bool loadFromFile(const std::string& path);

        /// \brief Loop or not.
        void setLoop(bool flag) { m_isLoop = flag; }

        /// \brief Actual looping status.
        bool isLooping() const { return m_isLoop; }

        /// \brief Get already played time.
        float getCurrentTime() const;

        /// \brief Get audio duration time in seconds.
        float getDuration() const;
    private:
        /// \brief SoundStream call's this method then needed new data.
        bool onGetData(void* buffer, std::int32_t size) override;
        const AudioData& getAudioData() const override { return m_audioData; }
    private:
        FileInputStream m_fileInputStream;
        AudioData m_audioData{};
        bool m_isLoop{false};
    };
}