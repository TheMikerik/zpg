#include "Scene.h"

void Scene::setCamera(Camera* camera)
{
    for (DrawableObject& drawable : drawables) {
        drawable.shader.setCamera(camera);
    }
}

void Scene::update(float time)
{
    for (DrawableObject& drawable : drawables) {
        drawable.update(time);
    }
}

void Scene::render() const
{
    for (const DrawableObject& drawable : drawables) {
        drawable.render();
    }
}
