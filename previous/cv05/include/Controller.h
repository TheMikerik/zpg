#pragma once

#include "Scene.h"
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <vector>

class Controller {
public:
    Controller(GLFWwindow* window, std::vector<Scene>& scenes, Camera& camera);

    void handleInput();
    int getCurrentSceneIndex() const;

private:
    bool isDown(int key) const;
    bool pressedOnce(int key);
    void switchScene(int index);
    void handleMouse();

    GLFWwindow* window;
    std::vector<Scene>& scenes;
    Camera& camera;
    int currentSceneIndex = 0;
    int selectedDrawableIndex = 0;
    bool wasDown[GLFW_KEY_LAST + 1] = {};
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
};
