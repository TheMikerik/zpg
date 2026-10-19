#include "Controller.h"
#include "Transformation.h"
#include <iostream>

namespace {

const int SCENE_COUNT = 3;
const float MOUSE_SENSITIVITY = 0.005f;
const float WALK_STEP = 0.05f;

void translateBy(DrawableObject& d, glm::vec3 offset) {
    d.transformation.translate(offset);
}

void rotateBy(DrawableObject& d, float degrees, glm::vec3 axis) {
    d.transformation.rotate(degrees, axis);
}

void scaleBy(DrawableObject& d, float factor) {
    d.transformation.scale(glm::vec3(factor));
}

}

Controller::Controller(GLFWwindow* window, std::vector<Scene>& scenes, Camera& camera)
    : window(window), scenes(scenes), camera(camera)
{
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);

    if (!scenes.empty()) {
        std::cout << "Selected Drawable Index: " << selectedDrawableIndex << std::endl;
    }
}

int Controller::getCurrentSceneIndex() const {
    return currentSceneIndex;
}

bool Controller::isDown(int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Controller::pressedOnce(int key) {
    bool down = isDown(key);
    bool pressed = down && !wasDown[key];
    wasDown[key] = down;
    return pressed;
}

void Controller::handleMouse() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        float dx = static_cast<float>(x - lastMouseX);
        float dy = static_cast<float>(y - lastMouseY);
        camera.rotate(dy * MOUSE_SENSITIVITY, dx * MOUSE_SENSITIVITY);
    }

    lastMouseX = x;
    lastMouseY = y;
}

void Controller::handleInput() {
    handleMouse();

    if (pressedOnce(GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (isDown(GLFW_KEY_W)) { camera.moveForward(WALK_STEP); }
    if (isDown(GLFW_KEY_S)) { camera.moveForward(-WALK_STEP); }
    if (isDown(GLFW_KEY_A)) { camera.moveRight(-WALK_STEP); }
    if (isDown(GLFW_KEY_D)) { camera.moveRight(WALK_STEP); }

    std::vector<DrawableObject>& drawables = scenes[currentSceneIndex].drawables;

    if (!drawables.empty()) {
        if (selectedDrawableIndex >= drawables.size()) {
            selectedDrawableIndex = 0;
        }
        DrawableObject& selected = drawables[selectedDrawableIndex];

        const float translationStep = 0.05f;
        const float rotationStep = 5.0f;
        const float scaleStep = 0.05f;

        if (isDown(GLFW_KEY_Z)) { translateBy(selected, glm::vec3(translationStep, 0.0f, 0.0f)); std::cout << "Translated +X" << std::endl; }
        if (isDown(GLFW_KEY_X)) { translateBy(selected, glm::vec3(0.0f, translationStep, 0.0f)); std::cout << "Translated +Y" << std::endl; }
        if (isDown(GLFW_KEY_C)) { translateBy(selected, glm::vec3(0.0f, 0.0f, translationStep)); std::cout << "Translated +Z" << std::endl; }

        if (isDown(GLFW_KEY_UP)) { scaleBy(selected, 1.0f + scaleStep); std::cout << "Scaled Up" << std::endl; }
        if (isDown(GLFW_KEY_DOWN)) { scaleBy(selected, 1.0f - scaleStep); std::cout << "Scaled Down" << std::endl; }

        if (isDown(GLFW_KEY_RIGHT)) { rotateBy(selected, rotationStep, glm::vec3(1.0f, 0.0f, 0.0f)); std::cout << "Rotated +X" << std::endl; }
        if (isDown(GLFW_KEY_LEFT)) { rotateBy(selected, -rotationStep, glm::vec3(1.0f, 0.0f, 0.0f)); std::cout << "Rotated -X" << std::endl; }

        if (pressedOnce(GLFW_KEY_ENTER)) {
            selectedDrawableIndex = (selectedDrawableIndex + 1) % drawables.size();
            std::cout << "Switched to Drawable Index: " << selectedDrawableIndex << std::endl;
        }
    }

    for (int i = 0; i < SCENE_COUNT; ++i) {
        if (pressedOnce(GLFW_KEY_1 + i)) {
            switchScene(i);
        }
    }
}

void Controller::switchScene(int index) {
    if (index >= 0 && index < static_cast<int>(scenes.size())) {
        currentSceneIndex = index;
        selectedDrawableIndex = 0;
        std::cout << "Switched to Scene " << index + 1 << std::endl;
    } else {
        std::cerr << "Invalid scene index: " << index << std::endl;
    }
}
