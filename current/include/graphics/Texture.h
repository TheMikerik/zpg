#pragma once

#include <string>
#include <GL/glew.h>

class Texture {
public:
    explicit Texture(const std::string& path);
    Texture(Texture&& other) noexcept;
    ~Texture();

    void bind(GLuint unit) const;

private:
    GLuint textureID;
};
