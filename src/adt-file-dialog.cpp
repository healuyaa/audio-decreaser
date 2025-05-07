#include "adt-file-dialog.hpp"
#include "adt-flags.hpp"
#include "adt-paths.hpp"

#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <shobjidl.h>
#include <thread>
#include <vector>
#include <windows.h>

namespace adt {
    const void ADTFileDialog::OpenFileDialog() {
        Flags::getInstance().SetFileDialogOpen(true);

        std::thread( [this]() {
            if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
                Flags::getInstance().SetFileDialogOpen(false);
                return;
            }
    
            IFileDialog* pFileDialog = nullptr;
            if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                                        IID_PPV_ARGS(&pFileDialog)))) {
                CoUninitialize();
                Flags::getInstance().SetFileDialogOpen(false);
                return;
            }
    
            COMDLG_FILTERSPEC filter[] = {
                { L"Audio files (*.wav)", L"*.wav" },
                { L"All files (*.*)", L"*.*" }
            };
            pFileDialog->SetFileTypes(2, filter);
            pFileDialog->SetFileTypeIndex(1);
    
            if (FAILED(pFileDialog->Show(nullptr))) {
                pFileDialog->Release();
                CoUninitialize();
                Flags::getInstance().SetFileDialogOpen(false);
                return;
            }
    
            IShellItem* pItem = nullptr;
            if (FAILED(pFileDialog->GetResult(&pItem))) {
                pFileDialog->Release();
                CoUninitialize();
                Flags::getInstance().SetFileDialogOpen(false);
                return;
            }
    
            PWSTR pszFilePath = nullptr;
            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                std::wstring ws(pszFilePath);
                std::string path(ws.begin(), ws.end());
    
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    Paths::getInstance().addTempPath(path);
                    std::cout << path << std::endl;
                }
    
                CoTaskMemFree(pszFilePath);
            }
    
            pItem->Release();
            pFileDialog->Release();
            CoUninitialize();
            Flags::getInstance().SetFileDialogOpen(false);
        }).detach();
    }

    const void ADTFileDialog::OpenFolderDialog() {
        Flags::getInstance().SetFolderDialog(true);

        std::thread([this]() {
            if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            IFileDialog* pFileDialog = nullptr;
            if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                                        IID_PPV_ARGS(&pFileDialog)))) {
                CoUninitialize();
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            DWORD dwOptions;
            if (FAILED(pFileDialog->GetOptions(&dwOptions))) {
                pFileDialog->Release();
                CoUninitialize();
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);
    
            if (FAILED(pFileDialog->Show(nullptr))) {
                pFileDialog->Release();
                CoUninitialize();
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            IShellItem* pItem = nullptr;
            if (FAILED(pFileDialog->GetResult(&pItem))) {
                pFileDialog->Release();
                CoUninitialize();
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            PWSTR pszFilePath = nullptr;
            std::wstring folderPath;
    
            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                folderPath = pszFilePath;
                CoTaskMemFree(pszFilePath);
            }
    
            pItem->Release();
            pFileDialog->Release();
            CoUninitialize();
    
            if (folderPath.empty()) {
                Flags::getInstance().SetFolderDialog(false);
                return;
            }
    
            std::filesystem::path dirPath(folderPath);
            std::vector<std::filesystem::path> wavFiles;
    
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
                if (entry.is_regular_file() && entry.path().extension() == ".wav") {
                    wavFiles.push_back(entry.path());
                }
            }
    
            {
                std::lock_guard<std::mutex> lock(mutex_);
                for (const auto& wav : wavFiles) {
                    Paths::getInstance().addTempPath(wav.string());

                    std::cout << wav.string() << std::endl;
                }
            }
    
            Flags::getInstance().SetFolderDialog(false);
        }).detach();
    }

    const void ADTFileDialog::OpenOutputDir(const std::string_view& path) {
        Flags::getInstance().SetOpenOutput(true);

        std::filesystem::path f_path(path);
        std::wstring w_path(std::filesystem::absolute(path));

        if(!std::filesystem::exists(path)) {
            std::wcout << "not exist";
            return;
        }

        std::thread( [this, w_path] ()  {
            auto result = ShellExecuteW(
                nullptr,
                L"open",
                w_path.c_str(),
                nullptr,
                nullptr,
                SW_SHOW
            );

            {
                std::lock_guard<std::mutex> lock(mutex_);
                if((INT_PTR)result <= 32) {
                    std::wcout << "shell err code " << (INT_PTR)result << std::endl;
                }
            }

            Flags::getInstance().SetOpenOutput(false);
        }).detach();
    }

    const void ADTFileDialog::OpenOutputDirAndSelect(const std::string_view& path) {
        
    }
}