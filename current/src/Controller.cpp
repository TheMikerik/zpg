#include "Controller.h"
#include "Transformation.h"
#include <iostream>

Controller::Controller(GLFWwindow* window, const std::vector<std::shared_ptr<Scene>>& scenes)
    : window(window), scenes(scenes), currentSceneIndex(0), selectedDrawableIndex(0)
{
    if (!scenes.empty()) {
        std::cout << "Selected Drawable Index: " << selectedDrawableIndex << std::endl;
    }
}

int Controller::getCurrentSceneIndex() const
{
    return currentSceneIndex;
}

void Controller::handleInput()
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

void Controller::switchScene(int index)
{
    if (index >= 0 && index < scenes.size()) {
        currentSceneIndex = index;
        selectedDrawableIndex = 0;
    } else {
        std::cerr << "Invalid scene index: " << index << std::endl;
    }
}
