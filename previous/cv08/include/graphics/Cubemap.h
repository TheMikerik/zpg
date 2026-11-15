#pragma once

#include <string>
#include <GL/glew.h>

class Cubemap {
public:
    // Ve složce musí být posx, negx, posy, negy, posz, negz (.jpg)
    explicit Cubemap(const std::string& folder);
    Cubemap(Cubemap&& other) noexcept;
    ~Cubemap();

    void bind(GLuint unit) const;

private:
    GLuint textureID;
};
