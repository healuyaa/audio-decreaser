#pragma once 

#include <mutex>
#include <string_view>

namespace adt {
    class ADTFileDialog {
        public:
        const void OpenFileDialog();
        const void OpenFolderDialog();
        const void OpenOutputDir(const std::string_view& path);
        const void OpenOutputDirAndSelect(const std::string_view& path);
        
        private:
        std::mutex mutex_;        
    };
}