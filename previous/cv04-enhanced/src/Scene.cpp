#include "Scene.h"

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
