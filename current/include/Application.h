#pragma once

#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

class Application {
public:
    Application();
    ~Application();

    void initialization();
    void createScenes();
    void run();

private:
    GLFWwindow* window;
    std::vector<std::shared_ptr<Scene>> scenes;

    static void errorCallback(int error, const char* description);
};
