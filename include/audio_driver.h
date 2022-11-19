#ifndef _AUDIO_DRIVER_H_
#define _AUDIO_DRIVER_H_

#include <SDL.h>

class AudioDriver {
public:
    AudioDriver();
    ~AudioDriver() noexcept;
    void beep(bool should_beep) const;
private:
    struct SquareWave {
        float phase;
        float phase_inc;
        float volume;
    };

    SDL_AudioDeviceID device{};
    SquareWave wave{};
};


#endif

