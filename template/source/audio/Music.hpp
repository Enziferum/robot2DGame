#pragma once

#include <thread>
#include <mutex>

#include "SoundStream.hpp"

namespace robot2D {

    struct FileInputStream {
        FileInputStream();
        FileInputStream(const FileInputStream& other) = delete;
        FileInputStream& operator=(const FileInputStream& other) = delete;
        FileInputStream(FileInputStream&& other) = delete;
        FileInputStream& operator=(FileInputStream&& other) = delete;
        ~FileInputStream() = default;

        /// \brief open's file as binary in reading mode only
        bool openFromFile(std::string path);

        /// \brief Read's buffer from File and move cursor byself.
        std::uint64_t read(void* buffer, std::uint64_t size);

        /// \brief Move file's actual cursor. If result == -1 endof file or can't get.
        std::uint64_t seek(std::uint64_t size);

        /// \brief Get file's actual cursor. If result == -1 endof file or can't get.
        std::uint64_t tell();

        /// \brief Get file size.
        std::uint64_t getSize();
    private:
        struct FileCloser {
            void operator()(std::FILE*);
        };

        std::unique_ptr<std::FILE, FileCloser> m_file;
    };

    class Music: public robot2D::SoundStream {
    public:
        Music();
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
        bool onGetData(char* buffer, std::int32_t size) override;
        const AudioData& getAudioData() const override { return m_audioData; }
    private:
        FileInputStream m_fileInputStream;
        AudioData m_audioData;
        bool m_isLoop{false};
    };
}