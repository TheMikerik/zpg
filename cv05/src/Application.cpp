#include "Application.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include <iostream>
#include <cstdlib>

void Application::errorCallback(int error, const char* description)
{
    // Forward to the instance's error callback if needed
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

Application::Application()
    : window(nullptr), currentSceneIndex(0) {}

Application::~Application()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization()
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "OpenGL Application", nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR: could not create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "\n==========INIT INFO==========" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "Using GLFW " << major << "." << minor << "." << revision << std::endl;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    std::cout << "Framebuffer size: " << width << "x" << height << std::endl;
    std::cout << "===============================\n" << std::endl;
}

void Application::createScenes()
{
    scenes.push_back(Scene1::create());
    scenes.push_back(Scene2::create());
    scenes.push_back(Scene3::create());
    scenes.push_back(Scene4::create());
    scenes.push_back(Scene5::create());
}

void Application::run()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.59f, 0.76f, 0.92f, 1.0f);
    createScenes();

    if (!scenes.empty()) {
        selectedDrawableIndex = 0;
        std::cout << "Selected Drawable Index: " << selectedDrawableIndex << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) {
            scenes[currentSceneIndex]->update(static_cast<float>(glfwGetTime()));
            scenes[currentSceneIndex]->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        handleInput();
    }

    glDisable(GL_DEPTH_TEST);
}


void Application::handleInput()
{
    if (currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
        return;
    }

    auto& currentScene = scenes[currentSceneIndex];
    auto& drawables = currentScene->getDrawables();

    if (drawables.empty()) {
        return;
    }

    // Clamp the selectedDrawableIndex
    if (selectedDrawableIndex >= drawables.size()) {
        selectedDrawableIndex = 0;
    }

    auto selectedDrawable = drawables[selectedDrawableIndex];

    float translationStep = 0.05f;
    float rotationStep = 5.0f; // degrees
    float scaleStep = 0.05f;

    // Translation Controls
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        // Move on positive X axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(translationStep, 0.0f, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated +X" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // Move on negative X axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(-translationStep, 0.0f, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated -X" << std::endl;
    }


    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        // Move on positive Y axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(0.0f, translationStep, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated +Y" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // Move on negative Y axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(0.0f, -translationStep, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated -Y" << std::endl;
    }


    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        // Move on positive Z axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(0.0f, 0.0f, translationStep));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated +Z" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // Move on negative Z axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.translate(glm::vec3(0.0f, 0.0f, -translationStep));
        selectedDrawable->setTransformation(trans);
        std::cout << "Translated -Z" << std::endl;
    }

    // Scaling Controls
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        // Make the object bigger
        Transformation trans = selectedDrawable->getTransformation();
        trans.scale(glm::vec3(1.0f + scaleStep));
        selectedDrawable->setTransformation(trans);
        std::cout << "Scaled Up" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // Make the object smaller
        Transformation trans = selectedDrawable->getTransformation();
        trans.scale(glm::vec3(1.0f - scaleStep));
        selectedDrawable->setTransformation(trans);
        std::cout << "Scaled Down" << std::endl;
    }

    // Rotation Controls
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        // Rotate on positive X axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.rotate(rotationStep, glm::vec3(1.0f, 0.0f, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Rotated +X" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        // Rotate on negative X axis
        Transformation trans = selectedDrawable->getTransformation();
        trans.rotate(-rotationStep, glm::vec3(1.0f, 0.0f, 0.0f));
        selectedDrawable->setTransformation(trans);
        std::cout << "Rotated -X" << std::endl;
    }

    // Switching to the next object
    static bool enterPressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (!enterPressedLastFrame) { // Detect key press event
            selectedDrawableIndex = (selectedDrawableIndex + 1) % drawables.size();
            std::cout << "Switched to Drawable Index: " << selectedDrawableIndex << std::endl;
            enterPressedLastFrame = true;
        }
    } else {
        enterPressedLastFrame = false;
    }

    // Switching between scenes
    static bool key1PressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!key1PressedLastFrame) {
            switchScene(0);
            std::cout << "Switched to Scene 1 (Triangle)" << std::endl;
            key1PressedLastFrame = true;
        }
    } else {
        key1PressedLastFrame = false;
    }

    static bool key2PressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!key2PressedLastFrame) {
            switchScene(1);
            std::cout << "Switched to Scene 2 (Sphere)" << std::endl;
            key2PressedLastFrame = true;
        }
    } else {
        key2PressedLastFrame = false;
    }

    static bool key3PressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (!key3PressedLastFrame) {
            switchScene(2);
            std::cout << "Switched to Scene 3 (Forest)" << std::endl;
            key3PressedLastFrame = true;
        }
    } else {
        key3PressedLastFrame = false;
    }

    static bool key4PressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        if (!key4PressedLastFrame) {
            switchScene(3);
            std::cout << "Switched to Scene 4 (Login)" << std::endl;
            key4PressedLastFrame = true;
        }
    } else {
        key4PressedLastFrame = false;
    }

    static bool key5PressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (!key5PressedLastFrame) {
            switchScene(4);
            std::cout << "Switched to Scene 5 (Solar system)" << std::endl;
            key5PressedLastFrame = true;
        }
    } else {
        key5PressedLastFrame = false;
    }
}

void Application::switchScene(int index)
{
    if (index >=0 && index < scenes.size()) {
        currentSceneIndex = index;
        selectedDrawableIndex = 0;
    } else {
        std::cerr << "Invalid scene index: " << index << std::endl;
    }
}