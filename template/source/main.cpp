#include <robot2D/Engine.hpp>
#include <game/MyApplication.hpp>

#include <al.h>
#include <alc.h>
#include <fstream>
#include <functional>

int runEngine() {
    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = {1280, 920};
    engineConfiguration.windowTitle = "Template Game";
    ROBOT2D_RUN_ENGINE(MyApplication, engineConfiguration);
}

struct WaveData {
    std::uint8_t channels;
    std::int32_t sampleRate;
    std::uint8_t bitsPerSample;
    std::vector<char> soundData;
};

inline std::int32_t read32int(const char* buffer, std::size_t len) {
    std::int32_t res;
    memcpy(&res, buffer, len);
    return res;
}


bool load_wav_header(std::ifstream& file,
                     std::uint8_t& channels,
                     std::int32_t& sampleRate,
                     std::uint8_t& bitsPerSample,
                     ALsizei& size) {

    if(!file.is_open())
        return false;

    char buffer[4];

    if(!file.read(buffer, 4))
        return false;

    if(std::strncmp(buffer, "RIFF", 4) != 0)
        return false;

    // the size of the file
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return false;
    }

    // the WAVE
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return false;
    }
    if(std::strncmp(buffer, "WAVE", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return false;
    }

    // "fmt/0"
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return false;
    }

    // PCM should be 1?
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return false;
    }

    // the number of channels
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return false;
    }
    channels = read32int(buffer, 2);

    // sample rate
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return false;
    }
    sampleRate = read32int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return false;
    }

    // ?? dafaq
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return false;
    }

    // bitsPerSample
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return false;
    }
    bitsPerSample = read32int(buffer, 2);

    // data chunk header "data"
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return false;
    }
    if(std::strncmp(buffer, "data", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return false;
    }

    // size of data
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return false;
    }
    size = read32int(buffer, 4);

    /* cannot be at the end of file */
    if(file.eof())
    {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return false;
    }
    if(file.fail())
    {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return false;
    }

    return true;
}

bool load_wav(const std::string& path, WaveData& waveData) {
    std::ifstream file{path, std::ios::binary};
    if(!file.is_open())
        return false;
    ALsizei size;
    if(!load_wav_header(file, waveData.channels, waveData.sampleRate, waveData.bitsPerSample, size))
        return false;

    waveData.soundData.resize(size);
    file.read(waveData.soundData.data(), size);

    return true;
}


bool check_al_errors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if(error != ALC_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename, line);
    return ret;
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return check_al_errors(filename, line);
}

template<typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename,
                 const std::uint_fast32_t line,
                 alcFunction function,
                 ALCdevice* device,
                 Params... params)
->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool>
{
    function(std::forward<Params>(params)...);
    return check_alc_errors(filename,line,device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename,
                 const std::uint_fast32_t line,
                 alcFunction function,
                 ReturnType& returnValue,
                 ALCdevice* device,
                 Params... params)
-> typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return check_alc_errors(filename,line,device);
}


class Sound {
public:
    Sound();
    ~Sound();

    bool load(const std::string& path);
    void play();
private:
    void setupBuffer();
    void setupSource();
private:
    WaveData m_waveData;

    ALuint buffer;
    ALuint source;
};

Sound::Sound() {}

Sound::~Sound() {
    alCall(alDeleteSources, 1, &source);
    alCall(alDeleteBuffers, 1, &buffer);
}

bool Sound::load(const std::string& path) {

    if(!load_wav(path, m_waveData))
        return false;

    setupBuffer();
    setupSource();
    return true;
}

void Sound::play() {
    alCall(alSourcePlay, source);
}

void Sound::setupBuffer() {
    alCall(alGenBuffers, 1, &buffer);

    ALenum format;

    if(m_waveData.channels == 1 && m_waveData.bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if(m_waveData.channels == 1 && m_waveData.bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if(m_waveData.channels == 2 && m_waveData.bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if(m_waveData.channels == 2 && m_waveData.bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
                << "ERROR: unrecognised wave format: "
                << m_waveData.channels << " channels, "
                << m_waveData.bitsPerSample << " bps" << std::endl;
        return;
    }

    // pack buffer //
    alCall(alBufferData,
           buffer,
           format, m_waveData.soundData.data(), m_waveData.soundData.size(),
           m_waveData.sampleRate);

    m_waveData.soundData.clear();
}

void Sound::setupSource() {
    alCall(alGenSources, 1, &source);
    alCall(alSourcef, source, AL_PITCH, 1);
    alCall(alSourcef, source, AL_GAIN, 1.0f);
    alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
    alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
    alCall(alSourcei, source, AL_BUFFER, buffer);
}

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
private:
    ALCdevice* openALDevice;
    ALCcontext* openALContext;
    ALCboolean contextMadeCurrent;
};

AudioManager::AudioManager() {

    openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
        return;

    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext) {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
    }

    contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
       || contextMadeCurrent != ALC_TRUE)
    {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
    }
}

AudioManager::~AudioManager() {
    alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
    alcCall(alcDestroyContext, openALDevice, openALContext);

    ALCboolean closed;
    alcCall(alcCloseDevice, closed, openALDevice, openALDevice);
}

int play_wav() {
    AudioManager audioManager;

    std::string wav_path = "res/audio/iamtheprotectorofthissystem.wav";

    Sound sound;
    if(!sound.load(wav_path))
        return -1;

    sound.play();
    std::cout << "I love you " << std::endl;

    return 0;
}

int main() {
   return play_wav();
}