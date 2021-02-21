#pragma once
#include "Scene.h"

class SceneRenderer final
{
public:
	bool drawWireframe = false;
	void renderLights(Scene& scene)
	{
		for (auto& light : scene.lights)
		{
			scene.lightSourceBlock->transform.translate(-scene.lightSourceBlock->transform.position);
			scene.lightSourceBlock->transform.translate(light->position);
			scene.lightSourceBlock->material.shadingMode = Material::materialColor;
			scene.lightSourceBlock->material.color = light->color;
			scene.lightSourceBlock->material.isLightSource = true;
			scene.lightSourceBlock->renderer->render(scene.camera, scene.lights);
		}
	}

	void renderTransparentObjects(Scene& scene)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		for (auto& cloud : scene.transparentObjects)
			cloud->renderer->render(scene.camera, scene.lights);
		glCullFace(GL_BACK);
		for (auto& cloud : scene.transparentObjects)
			cloud->renderer->render(scene.camera, scene.lights);
	}

	void render(Scene& scene)
	{
		glPolygonMode(GL_FRONT_AND_BACK, drawWireframe ? GL_LINE : GL_FILL);
		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		for (size_t i = 0; i < scene.objects.size(); ++i)
		{
			scene.objects[i]->renderer->render(scene.camera, scene.lights, &scene.backround);
		}
		
		renderLights(scene);
		scene.backround.render(scene.camera);

		renderTransparentObjects(scene);
	}
};