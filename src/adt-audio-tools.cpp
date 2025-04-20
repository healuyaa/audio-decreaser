#include "adt-audio-tools.hpp"

#include "sndfile.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

namespace adt {
    const std::vector<std::filesystem::path> AudioTools::get_cut_result() {
        return fragment_paths;
    }

    void AudioTools::split_audio(const std::filesystem::path &path_to_cut) {
        std::string split_path = utf16to1251(path_to_cut);

        SF_INFO sfinfo;        
        SNDFILE* in_file = sf_open(split_path.c_str(), SFM_READ, &sfinfo);

        if(!in_file) {
            std::cerr << "err open sndfile(audio)" << std::endl;
            return;
        }

        int sample_rate = sfinfo.samplerate;
        int channels = sfinfo.channels;
        sf_count_t total_frames = sfinfo.frames;

        sf_count_t segment_frames = 5 * sample_rate; // 5 is five second split audio
        int segment_count = total_frames / segment_frames;
        sf_count_t last_segment_frames = total_frames % segment_frames;
        
        fragment_paths.resize(segment_count + 1);

        std::vector<short> buffer(segment_frames * channels);
        
        for(size_t i = 0; i < segment_count; ++i) {
            std::string name_segment = "segment_" + std::to_string(i) + ".wav";
            std::filesystem::path output_file = std::filesystem::path("../temp/fragments") / std::filesystem::path(name_segment);

            fragment_paths[i] = output_file;

            std::string segment_path = utf16to1251(output_file);

            SF_INFO out_info = sfinfo;
            SNDFILE* out_file = sf_open(segment_path.c_str(), SFM_WRITE, &out_info);

            if(!out_file) {
                std::cerr << "err create fragment: " << i << std::endl;
            }

            sf_readf_short(in_file, buffer.data(), segment_frames);
            sf_writef_short(out_file, buffer.data(), segment_frames);

            sf_close(out_file);
            std::cout << "saved fragment: " << i << ' ' << segment_path << std::endl;
        }

        if(last_segment_frames > 0) {
            std::string name_segment = "segment_" + std::to_string(segment_count) + ".wav";
            std::filesystem::path output_file = std::filesystem::path("../temp/fragments") / std::filesystem::path(name_segment);

            std::string segment_path = utf16to1251(output_file);

            fragment_paths[segment_count] = output_file;

            SF_INFO out_info = sfinfo;
            SNDFILE* out_file = sf_open(segment_path.c_str(), SFM_WRITE, &out_info);

            if(!out_file) {
                std::cerr << "err create fragment: " << segment_count << std::endl;
            } else {
                buffer.resize(last_segment_frames * channels);

                sf_readf_short(in_file, buffer.data(), last_segment_frames);
                sf_writef_short(out_file, buffer.data(), last_segment_frames);

                sf_close(out_file);
                std::cout << "saved fragment: " << segment_count << ' ' << segment_path << std::endl;
            }
        }

        sf_close(in_file);
    }

    void AudioTools::concat_audio(const std::vector<std::filesystem::path>& paths, const std::filesystem::path& compressed_audio) {
        SF_INFO sfinfo_in;
        SF_INFO sfinfo_out = {};
        SNDFILE* outfile = nullptr;
    
        std::vector<short> allSamples;
    
        for (size_t i = 0; i < paths.size(); ++i) {
            std::string path = utf16to1251(paths[i]);
            SNDFILE* infile = sf_open(path.c_str(), SFM_READ, &sfinfo_in);
            if (!infile) {
                std::cerr << "Error opening file: " << paths[i].string() << "\n";
                continue;
            }
    
            if (i == 0) {
                sfinfo_out = sfinfo_in;
    
                std::string path_o = utf16to1251(compressed_audio);
                outfile = sf_open(path_o.c_str(), SFM_WRITE, &sfinfo_out);
                if (!outfile) {
                    std::cerr << "Error creating output file: " << compressed_audio << "\n";
                    sf_close(infile);
                    return;
                }
            } else {
                if (sfinfo_in.channels != sfinfo_out.channels || sfinfo_in.samplerate != sfinfo_out.samplerate) {
                    std::cerr << "Incompatible audio file: " << paths[i].string() << "\n";
                    sf_close(infile);
                    continue;
                }
            }
    
            std::vector<short> buffer(sfinfo_in.frames * sfinfo_in.channels);
            sf_count_t read_frames = sf_readf_short(infile, buffer.data(), sfinfo_in.frames);
            buffer.resize(read_frames * sfinfo_in.channels);
            allSamples.insert(allSamples.end(), buffer.begin(), buffer.end());
    
            sf_close(infile);
        }
    
        sf_writef_short(outfile, allSamples.data(), allSamples.size() / sfinfo_out.channels);
        sf_close(outfile);
    
        std::cout << "Concatenation complete: " << compressed_audio << std::endl;
    }    

    std::string AudioTools::utf16to1251(const std::filesystem::path& path) {
        std::wstring wstr = path.wstring();

        int size_needed = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

        std::string str(size_needed - 1, 0);

        WideCharToMultiByte(1251, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

        return str;
    }
}