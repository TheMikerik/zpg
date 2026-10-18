#pragma once

#include "Scene.h"
#include <GLFW/glfw3.h>
#include <vector>

class Controller {
public:
    Controller(GLFWwindow* window, std::vector<Scene>& scenes);

    void handleInput();
    int getCurrentSceneIndex() const;

private:
    bool isDown(int key) const;
    bool pressedOnce(int key);
    void switchScene(int index);

    GLFWwindow* window;
    std::vector<Scene>& scenes;
    int currentSceneIndex = 0;
    int selectedDrawableIndex = 0;
    bool wasDown[GLFW_KEY_LAST + 1] = {};
};
