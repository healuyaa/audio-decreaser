#define MINIAUDIO_IMPLEMENTATION
#include "adt-audio.hpp"

#include <iostream>

namespace adt {
    Audio::Audio() {
        result = ma_engine_init(NULL, &engine);

        if(result != MA_SUCCESS) {
            throw std::runtime_error("Failed to initialize audio engine.");
        }
    }

    Audio::~Audio() {
        ma_engine_uninit(&engine);
    }

    void Audio::playAudioFile(const std::filesystem::path &path, bool &is_playing) {
        if (path != cur_path) {
            if (is_playing) {
                stopAudioFile(is_playing);                
                clearAudio();
            }
            cur_path = path;
            is_playing = false;
        }

        if (!std::filesystem::exists(path)) {
            std::cerr << "File does not exist: " << path << std::endl;
            is_playing = false;
            return;
        }

        if (cur_path == path && !is_playing && is_paused) {
            ma_result result = ma_sound_start(&sound);
            if (result != MA_SUCCESS) {
                std::cerr << "Failed to resume sound: " << path << std::endl;
                is_paused = false;
                return;
            }
            is_playing = true;
            is_paused = false;
            std::cout << "Resumed: " << path << std::endl;
            return;
        }

        ma_result result = ma_sound_init_from_file(&engine, path.string().c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, &sound);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to initialize sound: " << path << std::endl;
            is_playing = false;
            return;
        }
        is_playing = true;

        result = ma_sound_start(&sound);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to play sound: " << path << std::endl;
            ma_sound_uninit(&sound);
            is_playing = false;
            return;
        }

        std::cout << "Playing: " << path << std::endl;
    }

    void Audio::pauseAudioFile(bool& is_playing) {
        if (is_playing) {
            ma_result result = ma_sound_stop(&sound);
            if (result != MA_SUCCESS) {
                std::cerr << "Failed to pause sound." << std::endl;
                return;
            }

            is_playing = false;
            is_paused = true;
            std::cout << "Audio paused." << std::endl;
        }
    }

    void Audio::stopAudioFile(bool &is_playing) {
        if (is_playing || is_paused) {
            ma_sound_stop(&sound);
            ma_sound_get_cursor_in_pcm_frames(&sound, 0);
            ma_sound_uninit(&sound);
            is_playing = false;
            is_paused = false;
            std::cout << "Audio playback stopped." << std::endl;
        }
    }

    void Audio::SetTime(bool &is_playing, float seconds) {
        if (!is_playing && !is_paused) {
            std::cerr << "No audio is currently playing or paused." << std::endl;
            return;
        }

        float total_length = GetAudioLength();
        if (seconds > total_length || seconds < 0) {
            std::cerr << "Invalid time: " << seconds << " seconds. Total length: " << total_length << " seconds." << std::endl;
            return;
        }

        ma_uint64 target_frame = static_cast<ma_uint64>(seconds * sample_rate);

        ma_result result = ma_sound_seek_to_pcm_frame(&sound, target_frame);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to seek to " << seconds << " seconds." << std::endl;
            return;
        }

        std::cout << "Seeked to " << seconds << " seconds." << std::endl;
    }

    float Audio::GetTime(bool &is_playing) {
        ma_uint64 cursor_in_frames = 0;

        ma_result result = ma_sound_get_cursor_in_pcm_frames(&sound, &cursor_in_frames);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to get the current position of the audio." << std::endl;
            return 0.0f;
        }

        float current_time_seconds = static_cast<float>(cursor_in_frames) / sample_rate;

        return current_time_seconds;
    }

    void Audio::SetVolume(float volume) {
        if (volume < 0.0f || volume > 1.0f) {
            std::cerr << "Volume must be between 0.0 and 1.0." << std::endl;
            return;
        }
        this->volume = volume;
        ma_sound_set_volume(&sound, volume);
    }

    float Audio::GetVolume() {
        return volume;
    }

    void Audio::clearAudio() {
        cur_path.clear();
    }

    float Audio::GetAudioLength() {
        ma_decoder decoder;
        ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_f32, 2, 44100); 

        ma_result result = ma_decoder_init_file(cur_path.string().c_str(), &decoderConfig, &decoder);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to initialize decoder for file: " << cur_path << std::endl;
            return 0.0f;
        }
        ma_uint64 total_frames = 0;

        result = ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);
        if (total_frames == 0) {
            std::cerr << "Failed to get audio length for file: " << cur_path << std::endl;
            ma_decoder_uninit(&decoder);
            return 0.0f;
        }

        sample_rate = decoder.outputSampleRate;

        float total_length = static_cast<float>(total_frames) / sample_rate;

        ma_decoder_uninit(&decoder);

        return total_length;
    }
}