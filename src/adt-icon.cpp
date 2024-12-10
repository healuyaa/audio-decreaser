#include "adt-icon.hpp"

#include <iostream>

#include "stb_image.h"

namespace adt {
    Icon::Icon(const std::filesystem::path path) {
        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if (!data) {
            throw std::runtime_error("Failed to load texture: " + path.string());
        }

        glGenTextures(1, &IconID);
        glBindTexture(GL_TEXTURE_2D, IconID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        std::cout << "Texture loaded with ID: " << IconID << std::endl;
    }

    Icon::~Icon() {
        if(IconID != 0) {
            glDeleteTextures(1, &IconID);
        }
    }

    Icon::Icon(Icon&& other) noexcept : IconID(other.IconID) {
        other.IconID = 0;
    }

    Icon& Icon::operator=(Icon&& other) noexcept {
        if(this != &other) {
            if(IconID != 0) {
                glDeleteTextures(1, &IconID);
            }
            IconID = other.IconID;
            other.IconID = 0;
        }
        return *this;
    }

    GLuint Icon::GetID() const {
        return IconID;
    }
}