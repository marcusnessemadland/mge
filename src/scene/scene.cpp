/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/scene.h"
#include "engine/entity.h"
#include "engine/mesh.h"
#include "engine/material.h"
#include "engine/texture.h"
#include "engine/world.h"
#include "engine/camera.h"
#include "engine/entities/model.h"
#include "engine/components/mesh_component.h"

#include <cassert>

namespace vr 
{
	void Scene::initBridge()
	{
		m_writeBuffer = std::make_unique<mb::SharedBuffer>();
		m_writeBuffer->init("maya-bridge-write", sizeof(mb::SharedData));

		m_readBuffer = std::make_unique<mb::SharedBuffer>();
		m_readBuffer->init("maya-bridge-read", sizeof(uint32_t));

		// Begin sync
		uint32_t status = UINT32_MAX;
		m_readBuffer->read(&status, sizeof(uint32_t));
		if (status != MAYABRIDGE_MESSAGE_RELOAD_SCENE)
		{
			m_models.clear();

			status = MAYABRIDGE_MESSAGE_RELOAD_SCENE;
			m_readBuffer->write(&status, sizeof(uint32_t));
		}
	}

	void Scene::shutdownBridge()
	{
		if (isUsingBridge())
		{
			m_writeBuffer->shutdown();
			m_readBuffer->shutdown();
		}
	}

	bool Scene::isUsingBridge()
	{
		return m_writeBuffer && m_readBuffer;
	}

	void Scene::modelAdded(const mb::Model& _model)
	{
		std::shared_ptr<Model> myModel = m_world->makeEntity<Model>();

		myModel->setPosition(Vec3(
			_model.position[0],
			_model.position[1],
			_model.position[2]));

		myModel->setRotation(Quat(
			_model.rotation[0], 
			_model.rotation[1], 
			_model.rotation[2],
			_model.rotation[3]));

		myModel->setScale(Vec3(
			_model.scale[0],
			_model.scale[1],
			_model.scale[2]));

		for (uint32_t ii = 0; ii < _model.numMeshes; ++ii)
		{
			auto& mesh = _model.meshes[ii];

			std::vector<Vertex> vertices;
			for (uint32_t jj = 0; jj < mesh.numVertices; ++jj)
			{
				auto& vertex = mesh.vertices[jj];
				vertices.push_back(*(Vertex*)&vertex);
			}

			std::vector<uint32_t> indices;
			for (uint32_t jj = 0; jj < mesh.numIndices; ++jj)
			{
				auto& index = mesh.indices[jj];
				indices.push_back(index);
			}

			std::shared_ptr<Mesh> myMesh = createMesh(Primitive::Triangles, vertices, indices);
			std::shared_ptr<Material> myMaterial = createMaterial();

			const mb::Material& material = mesh.material;

			myMaterial->setColor(loadTexture(material.baseColorTexture));
			myMaterial->setMetallic(loadTexture(material.metallicTexture));
			myMaterial->setRoughness(loadTexture(material.roughnessTexture));
			myMaterial->setNormal(loadTexture(material.normalTexture));
			myMaterial->setOcclusion(loadTexture(material.occlusionTexture));
			myMaterial->setEmissive(loadTexture(material.emissiveTexture));

			myMaterial->setColor(Vec3(
				material.baseColorFactor[0],
				material.baseColorFactor[1],
				material.baseColorFactor[2]));
			myMaterial->setMetallic(material.metallicFactor);
			myMaterial->setRoughness(material.roughnessFactor);
			myMaterial->setNormal(material.normalScale);
			myMaterial->setOcclusion(material.occlusionStrength);
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

	void Scene::write(FILE* _file)
	{
		uint32_t numModels = (uint32_t)m_models.size();
		fwrite(&numModels, sizeof(uint32_t), 1, _file);

		for (auto& key : m_models) 
		{
			std::string name = key.first;

			uint32_t sizeName = (uint32_t)name.length() + 1;  
			fwrite(&sizeName, sizeof(uint32_t), 1, _file);
			fwrite(name.c_str(), sizeName, 1, _file);

			std::shared_ptr<Model> model = key.second;

			Vec3 position = model->getPosition();
			Quat rotation = model->getRotation();
			Vec3 scale = model->getScale();
			fwrite(&position, sizeof(Vec3), 1, _file);
			fwrite(&rotation, sizeof(Quat), 1, _file);
			fwrite(&scale, sizeof(Vec3), 1, _file);

			auto components = model->getAllComponents<MeshComponent>();
			uint32_t numComponents = (uint32_t)components.size();
			fwrite(&numComponents, sizeof(uint32_t), 1, _file);

			for (auto& component : components)
			{
				std::shared_ptr<Mesh> mesh = component->m_mesh;

				uint32_t numVertices = (uint32_t)mesh->m_vertices.size();
				fwrite(&numVertices, sizeof(uint32_t), 1, _file);
				fwrite(mesh->m_vertices.data(), numVertices * sizeof(Vertex), 1, _file);

				uint32_t numIndices = (uint32_t)mesh->m_indices.size();
				fwrite(&numIndices, sizeof(uint32_t), 1, _file);
				fwrite(mesh->m_indices.data(), numIndices * sizeof(uint32_t), 1, _file);

				std::shared_ptr<Material> material = mesh->m_material;

				fwrite(&material->blend, sizeof(bool), 1, _file);
				fwrite(&material->doubleSided, sizeof(bool), 1, _file);
				fwrite(&material->baseColorFactor, sizeof(Vec3), 1, _file);
				fwrite(&material->metallicFactor, sizeof(float), 1, _file);
				fwrite(&material->roughnessFactor, sizeof(float), 1, _file);
				fwrite(&material->normalScale, sizeof(float), 1, _file);
				fwrite(&material->occlusionStrength, sizeof(float), 1, _file);
				fwrite(&material->emissiveFactor, sizeof(Vec3), 1, _file);

				writeTexture(_file, material->baseColorTexture);
				writeTexture(_file, material->metallicTexture);
				writeTexture(_file, material->roughnessTexture);
				writeTexture(_file, material->normalTexture);
				writeTexture(_file, material->occlusionTexture);
				writeTexture(_file, material->emissiveTexture);
			}
		}
	}

	void Scene::writeTexture(FILE* _file, std::shared_ptr<Texture> _texture)
	{
		bool hasTexture = _texture != nullptr;
		fwrite(&hasTexture, sizeof(bool), 1, _file);

		if (hasTexture)
		{
			uint32_t size = (uint32_t)_texture->m_filepath.length() + 1;
			fwrite(&size, sizeof(uint32_t), 1, _file);
			fwrite(_texture->m_filepath.c_str(), size, 1, _file);
		}
	}

	void Scene::read(FILE* _file)
	{
		uint32_t numModels = 0;
		fread(&numModels, sizeof(uint32_t), 1, _file);

		for (uint32_t ii = 0; ii < numModels; ++ii)
		{
			uint32_t sizeName;
			fread(&sizeName, sizeof(uint32_t), 1, _file); 
			char* buffer = new char[sizeName]; 
			fread(buffer, sizeName, 1, _file); 
			std::string name(buffer); 
			delete[] buffer; 

			m_models[name] = createModel(m_world);

			Vec3 position;
			Quat rotation;
			Vec3 scale;
			fread(&position, sizeof(Vec3), 1, _file);
			fread(&rotation, sizeof(Quat), 1, _file);
			fread(&scale, sizeof(Vec3), 1, _file);
			m_models[name]->setPosition(position);
			m_models[name]->setRotation(rotation);
			m_models[name]->setScale(scale);

			uint32_t numComponents = 0;
			fread(&numComponents, sizeof(uint32_t), 1, _file);

			for (uint32_t jj = 0; jj < numComponents; ++jj)
			{
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;

				uint32_t numVertices = 0;
				fread(&numVertices, sizeof(uint32_t), 1, _file);
				vertices.resize(numVertices);
				fread(vertices.data(), numVertices * sizeof(Vertex), 1, _file);

				uint32_t numIndices = 0;
				fread(&numIndices, sizeof(uint32_t), 1, _file);
				indices.resize(numIndices);
				fread(indices.data(), numIndices * sizeof(uint32_t), 1, _file);

				std::shared_ptr<Mesh> mesh = createMesh(Primitive::Triangles, vertices, indices);

				std::shared_ptr<Material> material = createMaterial();

				// Read material properties
				fread(&material->blend, sizeof(bool), 1, _file);
				fread(&material->doubleSided, sizeof(bool), 1, _file);
				fread(&material->baseColorFactor, sizeof(Vec3), 1, _file);
				fread(&material->metallicFactor, sizeof(float), 1, _file);
				fread(&material->roughnessFactor, sizeof(float), 1, _file);
				fread(&material->normalScale, sizeof(float), 1, _file);
				fread(&material->occlusionStrength, sizeof(float), 1, _file);
				fread(&material->emissiveFactor, sizeof(Vec3), 1, _file);

				// Read textures for the material
				material->baseColorTexture = readTexture(_file);
				material->metallicTexture = readTexture(_file);
				material->roughnessTexture = readTexture(_file);
				material->normalTexture = readTexture(_file);
				material->occlusionTexture = readTexture(_file);
				material->emissiveTexture = readTexture(_file);

				mesh->setMaterial(material);
				m_models[name]->addComponent<MeshComponent>(mesh);
			}
		}
	}

	std::shared_ptr<Texture> Scene::readTexture(FILE* _file)
	{
		bool hasTexture = false;
		fread(&hasTexture, sizeof(bool), 1, _file); 

		if (hasTexture)
		{
			uint32_t size = 0;
			fread(&size, sizeof(uint32_t), 1, _file);  
			char* buffer = new char[size];
			fread(buffer, size, 1, _file); 
			std::string filepath(buffer);  
			delete[] buffer;

			return loadTexture(filepath.c_str()); 
		}
		else
		{
			return nullptr;
		}
	}

	Scene::Scene(std::shared_ptr<World> _world, bool _usingMaya)
		: m_world(_world)
		, m_filepath(nullptr)
		, m_writeBuffer(nullptr)
		, m_readBuffer(nullptr)
	{
		if (_usingMaya)
		{
			initBridge();
		}
	}

	Scene::Scene(std::shared_ptr<World> _world, const char* _filepath, bool _usingMaya)
		: m_world(_world)
		, m_filepath(_filepath)
		, m_writeBuffer(nullptr)
		, m_readBuffer(nullptr)
	{
		if (_usingMaya)
		{
			initBridge();
		}
		else
		{
			FILE* file = fopen(_filepath, "rb");
			if (file != nullptr)
			{
				read(file);
				fclose(file);
			}
		}
	}

	Scene::~Scene()
	{
		shutdownBridge();
	}

	void Scene::update(double _dt)
	{
		if (isUsingBridge())
		{
			m_writeBuffer->read(&m_shared, sizeof(mb::Camera) + sizeof(uint32_t));

			uint32_t status = UINT32_MAX;
			m_readBuffer->read(&status, sizeof(uint32_t));

			if (status == MAYABRIDGE_MESSAGE_NONE)
			{
				if (m_shared.numModels != 0)
				{
					m_writeBuffer->read(&m_shared, sizeof(mb::SharedData));
				}

				for (uint32_t ii = 0; ii < m_shared.numModels; ++ii)
				{
					const mb::Model& model = m_shared.models[ii];

					auto it = m_models.find(model.name);
					if (it != m_models.end())
					{
						//modelChanged(model);
					}
					else
					{
						modelAdded(model);
					}
				}

				status = MAYABRIDGE_MESSAGE_RECEIVED;
				m_readBuffer->write(&status, sizeof(uint32_t));
			}
			else if (status == MAYABRIDGE_MESSAGE_SAVE_SCENE)
			{
				if (m_filepath != nullptr)
				{
					save(m_filepath);
				}

				status = MAYABRIDGE_MESSAGE_RECEIVED;
				m_readBuffer->write(&status, sizeof(uint32_t));
			}
		}
	}

	std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world, bool _usingMaya)
	{
		return _world->makeEntity<Scene>(_world, _usingMaya);
	}

	std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath, bool _usingMaya)
	{
		return _world->makeEntity<Scene>(_world, _filepath, _usingMaya);
	}

	void Scene::save(const char* _filepath)
	{
		FILE* file = fopen(_filepath, "wb");
		assert(file != NULL);
		write(file);
		fclose(file);
	}

} // namespace vr
