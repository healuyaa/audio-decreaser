#pragma once

#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace adt {
    class Paths {
        public:
        static Paths &getInstance();

        std::string_view GetPath(const std::string& name);

        void addTempPath(const std::string& id, const std::string& path);
        void removeTempPath(const std::string& id);
        std::string getTempPath(const std::string& id);

        void clearTempPath();

        private:
        Paths();
        ~Paths() = default;
        Paths(const Paths&) = delete;
        Paths& operator=(const Paths&) = delete;         

        mutable std::mutex mutex_;

        std::unordered_map<std::string, std::string_view> named_paths;
        std::unordered_set<std::string> path_pool;

        std::unordered_map<std::string, std::string> temp_pool;

        void registerPath(const std::string& name, const std::string& path);
    };
}