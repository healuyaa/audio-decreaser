#pragma once
#include "miniaudio.h"

#include <filesystem>

namespace adt {
    class Audio {
        public:
        Audio();
        ~Audio();

        void playAudioFile(const std::filesystem::path &path, bool &is_playing);
        void pauseAudioFile(bool &is_playing);
        void stopAudioFile(bool &is_playing);

        void SetTime(bool &is_playing, float seconds);
        float GetTime(bool &is_playing);
        void SetVolume(float volume);
        float GetVolume();

        float GetAudioLength();

        private:
        ma_engine engine;
        ma_result result;
        ma_sound sound;
        ma_decoder decoder;
        ma_uint32 sample_rate = 0;
        ma_uint32 channels = 0;

        std::filesystem::path cur_path;
        bool is_paused = false;

        float volume = 0.5f;

        void clearAudio();        
    };
}