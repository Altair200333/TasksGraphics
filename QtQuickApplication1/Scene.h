#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <QObject>

#include "Input.h"
#include "MouseInput.h"
#include "Object.h"
#include <QTransform>


#include "Background.h"
#include "MeshLoader.h"
#include "VolumetricCubeMeshRenderer.h"

class Scene final
{
	std::shared_ptr<QObject> parent;
	void createLightSourceBlock()
	{
		lightSourceBlock = std::make_shared<Object>();
		lightSourceBlock->mesh = MeshLoader().loadModel("Assets/Models/cube.obj")[0].mesh;
		lightSourceBlock->renderer = std::make_shared<SimpleMeshRenderer>();
		lightSourceBlock->initRenderer(parent);
		lightSourceBlock->material.shadingMode = Material::materialColor;
	}
	std::shared_ptr<Object> createCloud(const QVector3D& pos)
	{
		auto cloud = std::make_shared<Object>();
		auto model = MeshLoader().loadModel("Assets/Models/cube.obj")[0];
		cloud->mesh = model.mesh;
		cloud->material = model.material;
		cloud->renderer = std::make_shared<VolumetricCubeMeshRenderer>();
		cloud->initRenderer(parent, "Shaders/cloud.fs", "Shaders/cloud.vs");
		cloud->transform.translate(pos);

		return cloud;
	}
	void addModel(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, int shaderType = 1, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			auto object = std::make_shared<Object>();
			object->tag = tag;
			object->mesh = model.mesh;
			object->material = model.material;
			object->renderer = std::make_shared<SimpleMeshRenderer>();
			
			if(shaderType == 0)
				object->initRenderer(parent, "Shaders/triangleG.fs", "Shaders/triangleG.vs", "Shaders/triangleG.gs");
			else if(shaderType == 1)
				object->initRenderer(parent, "Shaders/triangle.fs", "Shaders/triangle.vs");
			
			object->transform.translate(pos);
			objects.push_back(object);
		}

	}
public:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<LightSource>> lights;
	std::vector<std::shared_ptr<Object>> clouds;
	
	Background backround;

	std::shared_ptr<Object> lightSourceBlock;
	GLCamera camera;

	bool drawWireframe = false;

	QVector2D angularVelocity = {0,0};
	
	Scene() = default;

	Scene(std::shared_ptr<QObject> _parent): parent(std::move(_parent))
	{
		const auto cubeModel = MeshLoader().loadModel("Assets/Models/hcube.obj");
		const auto suzModel = MeshLoader().loadModel("Assets/Models/suz.obj");
		for (int i = 0; i < 2; ++i)
		{
			addModel(cubeModel, { i * 3.5f, 0,0 }, 1, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(suzModel, { 7.0f + i * 3.5f, 0,0 },1, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(suzModel, { 7.0f + i * 3.5f, -5,0 }, 0, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(cubeModel, { i * 3.5f, -5,0 }, 0, "modifiable");
		}
		
		addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), {3.5f, 5, 0}, 1, "modifiable");
		addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), {7.5f, 5, 0}, 1, "modifiable");
		addModel(MeshLoader().loadModel("Assets/Models/plane.obj"), {0, -8, 0}, 1);

		clouds.push_back(createCloud({0, 4, -12}));

		createLightSourceBlock();
		
		lights.push_back(std::make_shared<LightSource>(QVector3D{-5, 0, 7}));
		lights.push_back(std::make_shared<LightSource>(QVector3D{30, 9, -7}, QColor{255, 219, 102}));

		backround = Background(parent);
	}

};
