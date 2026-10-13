#pragma once

#include "Scene.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

class Controller {
public:
    Controller(GLFWwindow* window, const std::vector<std::shared_ptr<Scene>>& scenes);

    void handleInput();
    void switchScene(int index);

    int getCurrentSceneIndex() const;

private:
    GLFWwindow* window;
    const std::vector<std::shared_ptr<Scene>>& scenes;
    int currentSceneIndex = 0;

    int selectedDrawableIndex = 0;
};
