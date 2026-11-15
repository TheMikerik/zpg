#include "Controller.h"
#include "Transformation.h"
#include "PositionColorModel.h"
#include "../../models/tree.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace {

const int SCENE_COUNT = 8;
const float MOUSE_SENSITIVITY = 0.005f;
const float WALK_STEP = 0.05f;
const float NEW_TREE_SCALE = 0.1f;
const glm::vec3 NEW_TREE_LEAF_COLOR(0.2f, 0.6f, 0.2f);

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
    : window(window), scenes(scenes), camera(camera) {
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);

    if (!scenes.empty()) {
        std::cout << "Selected Drawable Index: " << selectedDrawableIndex << std::endl;
    }
}

int Controller::getCurrentSceneIndex() const {
    return currentSceneIndex;
}

bool Controller::isResizeBugMode() const {
    return resizeBugMode;
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

    if (pressedOnce(GLFW_KEY_R)) {
        resizeBugMode = !resizeBugMode;
        std::cout << (resizeBugMode ? "Resize BUG mode ON (viewport a perspektiva zamrzle)"
                                    : "Resize BUG mode OFF (viewport a perspektiva se aktualizuji)") << std::endl;
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
        if (scenes[index].hasCameraStart) {
            camera.setPosition(scenes[index].cameraStart, scenes[index].cameraStartFi);
        }
        selectedDrawableIndex = 0;
        std::cout << "Switched to Scene " << index + 1 << std::endl;
    } else {
        std::cerr << "Invalid scene index: " << index << std::endl;
    }
}

void Controller::pickObject() {
    bool down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool clicked = down && !wasMouseDown;
    wasMouseDown = down;
    if (!clicked) {
        return;
    }

    GLint x, y;
    getCursorPixel(x, y);

    GLuint index = 0;
    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    std::cout << "Clicked: " << x << ", " << y << ", stencil: " << index << std::endl;

    if (index > 0 && index <= scenes[currentSceneIndex].drawables.size()) {
        selectedDrawableIndex = index - 1;
        std::cout << "Selected Drawable Index: " << selectedDrawableIndex << "\n\n" << std::endl;
    }
}

void Controller::spawnTree() {
    bool down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    bool clicked = down && !wasRightMouseDown;
    wasRightMouseDown = down;
    if (!clicked || currentSceneIndex != 0) {
        return;
    }

    GLint x, y;
    getCursorPixel(x, y);

    GLfloat depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    if (depth >= 1.0f) {
        return;
    }
    int fbW, fbH;
    glfwGetFramebufferSize(window, &fbW, &fbH);
    glm::vec4 viewport(0, 0, fbW, fbH);
    glm::vec3 pos = glm::unProject(glm::vec3(x, y, depth), camera.getViewMatrix(), camera.getProjectionMatrix(), viewport);
    std::cout << "unProject [" << pos.x << ", " << pos.y << ", " << pos.z << "]" << std::endl;

    addTree(pos);
}

void Controller::getCursorPixel(GLint& x, GLint& y) const {
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    int winW, winH, fbW, fbH;
    glfwGetWindowSize(window, &winW, &winH);
    glfwGetFramebufferSize(window, &fbW, &fbH);
    x = static_cast<GLint>(cursorX * fbW / winW);
    y = fbH - static_cast<GLint>(cursorY * fbH / winH);
}

void Controller::addTree(const glm::vec3& position) {
    std::vector<DrawableObject>& drawables = scenes[currentSceneIndex].drawables;
    drawables.emplace_back(PositionColorModel(std::vector<float>(std::begin(tree), std::end(tree))),
                           ShaderProgram("./shaders/vertex_shader_tree.glsl", "./shaders/fragment_shader_tree.glsl"));
    DrawableObject& newTree = drawables.back();
    newTree.id = drawables.size();
    newTree.shader.setCamera(&camera);
    newTree.transformation.translate(position);
    newTree.transformation.scale(glm::vec3(NEW_TREE_SCALE));
    newTree.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("leafColor", NEW_TREE_LEAF_COLOR);
    };
}
