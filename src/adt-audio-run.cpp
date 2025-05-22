#include "adt-audio-run.hpp"
#include "adt-audio-compress.hpp"
#include "adt-audio-tools.hpp"
#include "adt-file-tools.hpp"
#include "adt-flags.hpp"
#include "adt-paths.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace adt {
    void AudioRunner::run(std::atomic<float>* progress) {
        Flags::getInstance().SetCompress(true);
        Flags::getInstance().SetLoadRightLine(true);

        if(Paths::getInstance().GetSizeTempPool() == 0)
            return;

        if(!Flags::getInstance().GetIsCountChangeThreads()) {
            ChangeThreads(Flags::getInstance().GetCountThreads()); // default half of threads processor threads
            Flags::getInstance().SetIsCountChangeThreads(false);
        }

        for(std::size_t i = 0; i < Paths::getInstance().GetSizeTempPool(); ++i) {
            tools = std::make_shared<adt::AudioTools>();

            auto path = Paths::getInstance().getTempPath(std::to_string(i));

            FileTools::getInstance().copyFileAudio(path);

            std::filesystem::path path_o(Paths::getInstance().GetPath("audio_dir")); 
            path_o /= std::filesystem::path(path).filename();

            if(!std::filesystem::exists(path_o)) {
                return;
            }

            tools->splitAudio(path_o);
            auto paths_fragments = tools->GetCutResult();

            FileTools::getInstance().delete_audio(path_o.string());
            path.clear();

            std::vector<std::string> spaths_fragments;
            spaths_fragments.resize(paths_fragments.size());
            
            for(std::size_t i = 0; i < spaths_fragments.size(); ++i) {
                spaths_fragments[i] = paths_fragments[i].string();
            }

            {
                Compress compressor(spaths_fragments, number_threads, progress);
                compressor.run();
            }

            FileTools::getInstance().deleteFragments(spaths_fragments);
            
            std::vector<std::filesystem::path> compressed_paths;
            compressed_paths.resize(spaths_fragments.size());

            for(std::size_t i = 0; i < spaths_fragments.size(); ++i) {
                std::filesystem::path p(spaths_fragments[i]);
                compressed_paths[i] = p.parent_path().string() + "/compressed_" + p.filename().string();
                spaths_fragments[i] = p.parent_path().string() + "/compressed_" + p.filename().string();
            }

            std::filesystem::path compressed_audio = std::string(Paths::getInstance().GetPath("results")) + "/compressed_" + path_o.filename().string();
            auto safe_path = tools->GetUniquePath(compressed_audio);
            
            tools->concatAudio(compressed_paths, safe_path);

            FileTools::getInstance().deleteFragments(spaths_fragments);
        }

        Flags::getInstance().SetCompress(false);
        Flags::getInstance().SetLoadRightLine(false);
    }

    void AudioRunner::ChangeThreads(int n) {
        number_threads = n;
    }
}