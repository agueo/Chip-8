#include <iostream>
#include <math.h>
#include <SDL.h>
#include "audio_driver.h"

AudioDriver::AudioDriver() 
    : device{}, wave{}  {

    // Initialize audio
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec desired_spec {};
    SDL_AudioSpec have_spec {};

    desired_spec.format = AUDIO_F32SYS;
    desired_spec.freq = 44100;
    desired_spec.channels = 1;
    desired_spec.samples = 0;

    desired_spec.callback = [](void * userdata, uint8_t* stream, int len) {
        SquareWave *wave = (SquareWave*) userdata;
        float *sample_stream = (float *) stream;
        for (int i = 0; i < len / (int)sizeof(float); ++i) {
            float volume = wave->volume;
            if (wave->phase < 0.5) {
                volume *= 1.0;
            }
            else {
                volume *= -1.0;
            }
            sample_stream[i] = volume;
            wave->phase = fmod((wave->phase + wave->phase_inc), 1.0);
        }
    };

    wave.phase_inc = 240.0 / desired_spec.freq;
    wave.phase = 0.0;
    wave.volume = 0.25;
    desired_spec.userdata = (void *)&wave;

    SDL_Log("Initializing Audio device");
    device = SDL_OpenAudioDevice(NULL, 0, &desired_spec, &have_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if (device == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    }
    if (have_spec.format != desired_spec.format) {
        SDL_Log("We did not get the float32 audio format");
    }
    // Start Audio playback
    SDL_PauseAudioDevice(device, 0);
}

AudioDriver::~AudioDriver() noexcept {
    SDL_CloseAudioDevice(device);
    SDL_Log("AudioDevice Closed");
}

void AudioDriver::beep(bool should_beep) const {
    if (should_beep) {
        SDL_PauseAudioDevice(device, 0);
        return;
    }
    SDL_PauseAudioDevice(device, 1);
}
