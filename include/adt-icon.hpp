#pragma once

#include <filesystem>

#include "glad/glad.h"

#include "GL/gl.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

namespace adt {
    class Icon {
        public:
        explicit Icon(const std::filesystem::path path);
        ~Icon();

        Icon(const Icon&) = delete;
        Icon& operator=(const Icon&) = delete;
        Icon(Icon&& other) noexcept;
        Icon& operator=(Icon&& other) noexcept;

        GLuint GetID() const;

        private:
        GLuint IconID = 0;
    };
}