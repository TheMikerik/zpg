#include "SceneClassroom.h"
#include "PositionNormalUVModel.h"
#include "OBJLoader.h"
#include "Texture.h"
#include "Cubemap.h"
#include "PositionNormalModel.h"
#include "Light.h"
#include "../../models/plain_uv.h"
#include <memory>

namespace {

const Light ROOM_LIGHT(glm::vec3(0.0f, 3.0f, 4.0f));

const float WALL_X = -3.75f;
const float BOARD_Z = 3.4f;
const int TABLE_ROWS = 3;
const int TABLE_COLUMNS = 3;
const float CHAIR_DISTANCE = 0.6f; // vzdálenost židle od středu stolu
const float TOILET_SCALE = 0.25f;

const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);

DrawableObject& addObject(Scene& scene, const std::vector<float>& vertices, std::shared_ptr<Texture> texture) {
    scene.drawables.emplace_back(PositionNormalUVModel(vertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& object = scene.drawables.back();
    object.texture = texture;
    object.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("normalFromAttr", 1);
        shader.setUniform("useTexture", 1);
        shader.setUniform("textureUnitID", 0);
        ROOM_LIGHT.sendTo(shader);
    };
    return object;
}

}

Scene SceneClassroom::create() {
    std::shared_ptr<Texture> roomTex = std::make_shared<Texture>("./models/EB408/T_EB408.png");
    std::shared_ptr<Texture> doorTex = std::make_shared<Texture>("./models/door/T_door_basecolor.png");
    std::shared_ptr<Texture> boardTex = std::make_shared<Texture>("./models/Whiteboard/T_Whiteboard_BaseColor.png");
    std::shared_ptr<Texture> boardTextTex = std::make_shared<Texture>("./images/whiteboard_phong.png");
    std::shared_ptr<Texture> tableTex = std::make_shared<Texture>("./models/EB408Models/T_Table_BaseColor.png");
    std::shared_ptr<Texture> chairTex = std::make_shared<Texture>("./models/EB408Models/T_OrangeChair_BaseColor.png");
    std::shared_ptr<Texture> shrekTex = std::make_shared<Texture>("./models/shrek/shrek.png");
    std::shared_ptr<Texture> fionaTex = std::make_shared<Texture>("./models/shrek/fiona.png");
    std::shared_ptr<Texture> toiletTex = std::make_shared<Texture>("./models/shrek/toiled.jpg");

    std::vector<float> table = OBJLoader::loadOBJ("./models/EB408Models/table.obj", true);
    std::vector<float> chair = OBJLoader::loadOBJ("./models/EB408Models/OrangeChair.obj", true);
    std::vector<float> plane(std::begin(plainUV), std::end(plainUV));

    Scene scene;
    scene.hasCameraStart = true;
    scene.cameraStart = glm::vec3(3.2f, 1.6f, 6.8f);
    scene.cameraStartFi = -2.6f;

    // Skybox se kreslí první a nezapisuje do hloubky
    scene.drawables.emplace_back(PositionNormalModel(OBJLoader::loadOBJ("./models/skybox/skybox.obj")),
                                 ShaderProgram("./shaders/vertex_shader_skybox.glsl", "./shaders/fragment_shader_skybox.glsl"));
    DrawableObject& sky = scene.drawables.back();
    sky.cubemap = std::make_shared<Cubemap>("./models/skybox/cubemap");
    sky.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("skybox", 0);
    };

    addObject(scene, OBJLoader::loadOBJ("./models/EB408/EB408.obj", true), roomTex);

    DrawableObject& door = addObject(scene, OBJLoader::loadOBJ("./models/door/Door.obj", true), doorTex);
    door.transformation.translate(glm::vec3(-3.475f, 0.0f, 7.5f));
    door.transformation.rotate(180.0f, Y_AXIS);

    DrawableObject& board = addObject(scene, OBJLoader::loadOBJ("./models/Whiteboard/Whiteboard.obj", true), boardTex);
    board.transformation.translate(glm::vec3(WALL_X, 0.0f, BOARD_Z));

    // Text na tabuli: svislá rovina těsně před tabulí
    DrawableObject& boardText = addObject(scene, plane, boardTextTex);
    boardText.transformation.translate(glm::vec3(WALL_X + 0.02f, 1.6f, BOARD_Z + 0.02f));
    boardText.transformation.rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    boardText.transformation.scale(glm::vec3(0.63f, 1.0f, 1.11f));

    DrawableObject& teacherTable = addObject(scene, table, tableTex);
    teacherTable.transformation.translate(glm::vec3(-3.1f, 0.0f, BOARD_Z));
    teacherTable.transformation.rotate(90.0f, Y_AXIS);

    for (int i = 0; i < TABLE_COLUMNS; i++) {
        for (int j = 0; j < TABLE_ROWS; j++) {
            float x = -1.6f + i * 1.7f;
            float z = 1.6f + j * 2.0f;

            DrawableObject& t = addObject(scene, table, tableTex);
            t.transformation.translate(glm::vec3(x, 0.0f, z));
            t.transformation.rotate(-90.0f, Y_AXIS);

            for (int k = -1; k <= 1; k += 2) {
                DrawableObject& c = addObject(scene, chair, chairTex);
                c.transformation.translate(glm::vec3(x + CHAIR_DISTANCE, 0.0f, z + k * 0.4f));
                c.transformation.rotate(-90.0f, Y_AXIS);
            }
        }
    }

    DrawableObject& shrek = addObject(scene, OBJLoader::loadOBJ("./models/shrek/shrek.obj", true), shrekTex);
    shrek.transformation.translate(glm::vec3(-3.0f, 0.0f, 1.5f));
    shrek.transformation.rotate(90.0f, Y_AXIS);

    DrawableObject& fiona = addObject(scene, OBJLoader::loadOBJ("./models/shrek/fiona.obj", true), fionaTex);
    fiona.transformation.translate(glm::vec3(-3.0f, 0.0f, 5.3f));
    fiona.transformation.rotate(90.0f, Y_AXIS);

    DrawableObject& toilet = addObject(scene, OBJLoader::loadOBJ("./models/shrek/toiled.obj", true), toiletTex);
    toilet.transformation.translate(glm::vec3(3.2f, 0.0f, 6.9f));
    toilet.transformation.scale(glm::vec3(TOILET_SCALE));

    return scene;
}
