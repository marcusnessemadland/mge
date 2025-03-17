/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/scene.h"
#include "engine/entity.h"
#include "engine/mesh.h"
#include "engine/material.h"
#include "engine/model.h"
#include "engine/world.h"

namespace vr 
{
	void Scene::update()
	{
		if (m_writeBuffer == NULL || m_readBuffer == NULL)
		{
			// Don't update if not linked.
			return;
		}

		// Will always read this data. Maya writes this.
		m_writeBuffer->read(&m_shared, sizeof(mb::SharedData));

		uint32_t status = UINT32_MAX;
		m_readBuffer->read(&status, sizeof(uint32_t));

		// Read events...
		if (status == MAYABRIDGE_MESSAGE_NONE)
		{
			// ...
			m_shared.proj;
			m_shared.view;

			//for (uint32_t ii = 0; ii < m_shared.numModels; ++ii)
			//{
			//	auto& model = m_shared.models[ii];
			//
			//	auto it = m_models.find(model.name);
			//	if (it != m_models.end())
			//	{
			//		modelChanged(model);
			//	}
			//	else
			//	{
			//		modelAdded(model);
			//	}
			//}

			// Update status.
			status = MAYABRIDGE_MESSAGE_RECEIVED;
			m_readBuffer->write(&status, sizeof(uint32_t));
		}
	}

	void Scene::modelAdded(const mb::Model& _model)
	{
		std::shared_ptr<Model> myModel = createModel(m_world);

		myModel->setPosition(Vec3(
			_model.position[0],
			_model.position[1],
			_model.position[2]));

		myModel->setRotation(Quat( // @todo Lol why have I messed up the order.
			_model.rotation[3],
			_model.rotation[0],
			_model.rotation[1],
			_model.rotation[2]));

		myModel->setScale(Vec3(
			_model.scale[0],
			_model.scale[1],
			_model.scale[2]));

		for (auto& mesh : _model.meshes)
		{
			std::vector<Vertex> vertices;
			for (auto& vertex : mesh.vertices)
			{
				vertices.push_back(*(Vertex*)&vertex);
			}

			std::vector<uint32_t> indices;
			for (auto& index : mesh.indices)
			{
				indices.push_back(index);
			}

			std::shared_ptr<Mesh> myMesh = createMesh(Primitive::Triangles, vertices, indices);

			const mb::Material& material = mesh.material;

			std::shared_ptr<Material> myMaterial = createMaterial();

			myMaterial->setColor(Vec3(
				material.baseColorFactor[0],
				material.baseColorFactor[1],
				material.baseColorFactor[2]));

			myMaterial->setMetallic(material.metallicFactor);
			myMaterial->setMetallic(material.roughnessFactor);
			myMaterial->setMetallic(material.normalScale);
			myMaterial->setMetallic(material.occlusionStrength);

			myMaterial->setEmissive(Vec3(
				material.emissiveFactor[0],
				material.emissiveFactor[1],
				material.emissiveFactor[2]));

			myMesh->setMaterial(myMaterial);

			myModel->addMesh(myMesh);
		}

		m_models[_model.name] = myModel;
	}

	void Scene::modelRemoved(const mb::Model& _model)
	{
		// @todo
	}

	void Scene::modelChanged(const mb::Model& _model)
	{
		// @todo
	}

	Scene::Scene(std::shared_ptr<World> _world)
		: m_world(_world)
	{
		m_world->m_scenes.push_back(this);

		// Initialize the shared memory
		m_writeBuffer = new mb::SharedBuffer();
		m_writeBuffer->init("maya-bridge-write", sizeof(mb::SharedData));

		m_readBuffer = new mb::SharedBuffer();
		m_readBuffer->init("maya-bridge-read", sizeof(uint32_t));

		// Begin sync
		uint32_t status = MAYABRIDGE_MESSAGE_RECEIVED;
		m_readBuffer->write(&status, sizeof(uint32_t));
	}

	Scene::Scene(std::shared_ptr<World> _world, const char* _filepath)
		: m_world(_world)
		, m_writeBuffer(NULL)
		, m_readBuffer(NULL)
	{
		// @todo
	}

	Scene::~Scene()
	{
		if (m_writeBuffer != NULL)
		{
			m_writeBuffer->shutdown();
			delete m_writeBuffer;
		}

		if (m_readBuffer != NULL)
		{
			m_readBuffer->shutdown();
			delete m_readBuffer;
		}
	}

	std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world)
	{
		return std::make_shared<Scene>(_world);
	}

	std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath)
	{
		return std::make_shared<Scene>(_world, _filepath);
	}

	void Scene::save(const char* _filepath)
	{
		// @todo
	}

} // namespace vr
