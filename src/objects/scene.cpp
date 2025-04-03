/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/objects/scene.h"
#include "engine/objects/model.h"
#include "engine/components/mesh_component.h"
#include "engine/mesh.h"
#include "engine/material.h"
#include "engine/texture.h"
#include "engine/world.h"
#include "engine/camera.h"

#include <filesystem>

namespace mge 
{
	void MayaSession::begin(std::unordered_map<std::string, std::shared_ptr<Model>>& _models)
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
			_models.clear();

			status = MAYABRIDGE_MESSAGE_RELOAD_SCENE;
			m_readBuffer->write(&status, sizeof(uint32_t));
		}
	}

	void MayaSession::update(std::unordered_map<std::string, std::shared_ptr<Model>>& _models)
	{
		m_writeBuffer->read(&m_shared,
			sizeof(mb::Camera) +
			sizeof(uint32_t) + //numModels
			sizeof(uint32_t)); //numMaterials

		uint32_t status = UINT32_MAX;
		m_readBuffer->read(&status, sizeof(uint32_t));

		if (status == MAYABRIDGE_MESSAGE_NONE)
		{
			if (m_shared.numMaterials != 0 || m_shared.numModels != 0)
			{
				// Only read entire data if we have any materials or models queued.
				m_writeBuffer->read(&m_shared, sizeof(mb::SharedData));
			}

			// Materials
			for (uint32_t ii = 0; ii < m_shared.numMaterials; ++ii)
			{
				const mb::Material& material = m_shared.materials[ii];

				auto it = m_materials.find(material.name);
				if (it != m_materials.end())
				{
					//materialChanged(material);
				}
				else
				{
					materialAdded(_models, material);
				}
			}

			// Models
			for (uint32_t ii = 0; ii < m_shared.numModels; ++ii)
			{
				const mb::Model& model = m_shared.models[ii];

				auto it = _models.find(model.name);
				if (it != _models.end())
				{
					//modelChanged(model);
				}
				else
				{
					modelAdded(_models, model);
				}
			}

			status = MAYABRIDGE_MESSAGE_RECEIVED;
			m_readBuffer->write(&status, sizeof(uint32_t));
		}
	}

	void MayaSession::end()
	{
		if (isValid())
		{
			m_writeBuffer->shutdown();
			m_readBuffer->shutdown();
		}
	}

	bool MayaSession::isValid()
	{
		return m_writeBuffer && m_readBuffer;
	}

	void MayaSession::modelAdded(std::unordered_map<std::string, std::shared_ptr<Model>>& _models, const mb::Model& _model)
	{
		_models[_model.name] = std::make_shared<Model>();
		std::shared_ptr<Model>& model = _models[_model.name];

		// Transform
		model->setPosition(Vec3(
			_model.position[0],
			_model.position[1],
			_model.position[2]));

		model->setRotation(Quat(
			_model.rotation[0], 
			_model.rotation[1], 
			_model.rotation[2],
			_model.rotation[3]));

		model->setScale(Vec3(
			_model.scale[0],
			_model.scale[1],
			_model.scale[2]));

		// Mesh
		auto& mesh = _model.mesh;

		std::vector<Vertex> vertices;
		for (uint32_t jj = 0; jj < mesh.numVertices; ++jj)
		{
			auto& vertex = mesh.vertices[jj];
			vertices.push_back(*(Vertex*)&vertex);
		}

		std::vector<std::shared_ptr<SubMesh>> subMeshes;
		for (uint32_t jj = 0; jj < mesh.numSubMeshes; ++jj)
		{
			auto& subMesh = mesh.subMeshes[jj];

			std::vector<uint32_t> indices;
			for (uint32_t jj = 0; jj < subMesh.numIndices; ++jj)
			{
				auto& index = subMesh.indices[jj];
				indices.push_back(index);
			}

			if (m_materials.find(subMesh.material) != m_materials.end())
			{
				subMeshes.push_back(createSubMesh(indices, m_materials[subMesh.material]));
			}
			else
			{
				subMeshes.push_back(createSubMesh(indices, nullptr));
			}
		}

		model->addMesh(createMesh(vertices, subMeshes));
	}

	void MayaSession::materialAdded(std::unordered_map<std::string, std::shared_ptr<Model>>& _models, const mb::Material& _material)
	{
		m_materials[_material.name] = std::make_shared<Material>(MGE_MATERIAL_NONE);
		std::shared_ptr<Material> material = m_materials[_material.name];

		// Textures
		std::string sceneName = std::filesystem::path(m_filepath).stem().string();
		std::string resourcePath = "scenes/" + sceneName + "/";
		
		std::string baseColorTexture = std::filesystem::path(_material.baseColorTexture).filename().string();
		std::string metallicTexture = std::filesystem::path(_material.metallicTexture).filename().string();
		std::string roughnessTexture = std::filesystem::path(_material.roughnessTexture).filename().string();
		std::string normalTexture = std::filesystem::path(_material.normalTexture).filename().string();
		std::string occlusionTexture = std::filesystem::path(_material.occlusionTexture).filename().string();
		std::string emissiveTexture = std::filesystem::path(_material.emissiveTexture).filename().string();
		
		material->setColor(loadTexture((resourcePath + baseColorTexture).c_str()));
		material->setMetallic(loadTexture((resourcePath + metallicTexture).c_str()));
		material->setRoughness(loadTexture((resourcePath + roughnessTexture).c_str()));
		material->setNormal(loadTexture((resourcePath + normalTexture).c_str()));
		material->setOcclusion(loadTexture((resourcePath + occlusionTexture).c_str()));
		material->setEmissive(loadTexture((resourcePath + emissiveTexture).c_str()));

		// Factors
		material->setColor(Vec3(_material.baseColorFactor[0], _material.baseColorFactor[1], _material.baseColorFactor[2]));
		material->setMetallic(_material.metallicFactor);
		material->setRoughness(_material.roughnessFactor);
		material->setNormal(_material.normalScale);
		material->setOcclusion(_material.occlusionStrength);
		material->setEmissive(Vec3(_material.emissiveFactor[0], _material.emissiveFactor[1], _material.emissiveFactor[2]));
	}

	MayaSession::MayaSession(const char* _filepath)
		: m_filepath(_filepath)
	{
	}

	MayaSession::~MayaSession()
	{
		end();
	}

	void Scene::write(FILE* _file)
	{
		// Write models
		uint32_t numModels = (uint32_t)m_models.size();
		fwrite(&numModels, sizeof(uint32_t), 1, _file);

		for (auto& key : m_models)
		{
			auto& name = key.first;
			auto& model = key.second;

			// Write name
			writeString(_file, name);

			// Write transform
			fwrite(&model->getPosition(), sizeof(Vec3), 1, _file);
			fwrite(&model->getRotation(), sizeof(Quat), 1, _file);
			fwrite(&model->getScale(), sizeof(Vec3), 1, _file);

			// Write vertices
			std::shared_ptr<Mesh> mesh = model->getComponent<MeshComponent>()->m_mesh;
			uint32_t numVertices = (uint32_t)mesh->m_vertices.size();
			fwrite(&numVertices, sizeof(uint32_t), 1, _file);
			fwrite(mesh->m_vertices.data(), numVertices * sizeof(Vertex), 1, _file);

			// Write submeshes
			uint32_t numSubmeshes = (uint32_t)mesh->m_submeshes.size();
			fwrite(&numSubmeshes, sizeof(uint32_t), 1, _file);

			for (auto& submesh : mesh->m_submeshes)
			{
				// Write indices
				uint32_t numIndices = (uint32_t)submesh->m_indices.size();
				fwrite(&numIndices, sizeof(uint32_t), 1, _file);
				fwrite(submesh->m_indices.data(), numIndices * sizeof(uint32_t), 1, _file);

				// Write material properties
				std::shared_ptr<Material> material = submesh->m_material;
				fwrite(&material->blend, sizeof(bool), 1, _file);
				fwrite(&material->doubleSided, sizeof(bool), 1, _file);
				fwrite(&material->baseColorFactor, sizeof(Vec3), 1, _file);
				fwrite(&material->metallicFactor, sizeof(float), 1, _file);
				fwrite(&material->roughnessFactor, sizeof(float), 1, _file);
				fwrite(&material->normalScale, sizeof(float), 1, _file);
				fwrite(&material->occlusionStrength, sizeof(float), 1, _file);
				fwrite(&material->emissiveFactor, sizeof(Vec3), 1, _file);

				// Write textures for the material
				writeTexture(_file, material->baseColorTexture);
				writeTexture(_file, material->metallicTexture);
				writeTexture(_file, material->roughnessTexture);
				writeTexture(_file, material->normalTexture);
				writeTexture(_file, material->occlusionTexture);
				writeTexture(_file, material->emissiveTexture);
			}
		}
	}

	void Scene::read(FILE* _file)
	{
		uint32_t numModels = 0;
		fread(&numModels, sizeof(uint32_t), 1, _file);

		for (uint32_t ii = 0; ii < numModels; ++ii)
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();

			// Read name
			std::string name = readString(_file);

			// Read transform
			Vec3 position;
			Quat rotation;
			Vec3 scale;
			fread(&position, sizeof(Vec3), 1, _file);
			fread(&rotation, sizeof(Quat), 1, _file);
			fread(&scale, sizeof(Vec3), 1, _file);
			model->setPosition(position);
			model->setRotation(rotation);
			model->setScale(scale);

			// Read vertices
			std::vector<Vertex> vertices;
			uint32_t numVertices = 0;
			fread(&numVertices, sizeof(uint32_t), 1, _file);
			vertices.resize(numVertices);
			fread(vertices.data(), numVertices * sizeof(Vertex), 1, _file);

			// Read submeshes
			std::vector<std::shared_ptr<SubMesh>> subMeshes;
			uint32_t numSubMeshes = 0;
			fread(&numSubMeshes, sizeof(uint32_t), 1, _file);

			for (uint32_t jj = 0; jj < numSubMeshes; ++jj)
			{
				// Read indices
				std::vector<uint32_t> indices;
				uint32_t numIndices = 0;
				fread(&numIndices, sizeof(uint32_t), 1, _file);
				indices.resize(numIndices);
				fread(indices.data(), numIndices * sizeof(uint32_t), 1, _file);

				// Read material properties
				std::shared_ptr<Material> material = std::make_shared<Material>(MGE_MATERIAL_NONE);
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

				std::shared_ptr<SubMesh> subMesh = std::make_shared<SubMesh>(indices, material);
				subMeshes.push_back(subMesh);
			}

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, subMeshes);

			m_models[name] = model;
			m_models[name]->addComponent<MeshComponent>(mesh);
		}
	}

	void Scene::writeTexture(FILE* _file, std::shared_ptr<Texture> _texture)
	{
		bool hasTexture = _texture != nullptr;
		fwrite(&hasTexture, sizeof(bool), 1, _file);

		if (hasTexture)
		{
			writeString(_file, _texture->m_filepath);
		}
	}

	std::shared_ptr<Texture> Scene::readTexture(FILE* _file)
	{
		bool hasTexture = false;
		fread(&hasTexture, sizeof(bool), 1, _file); 

		if (hasTexture)
		{
			std::string filepath = readString(_file);
			return loadTexture(filepath.c_str()); 
		}
		else
		{
			return nullptr;
		}
	}

	void Scene::writeString(FILE* _file, const std::string& _str)
	{
		uint32_t size = (uint32_t)_str.length() + 1;
		fwrite(&size, sizeof(uint32_t), 1, _file);
		fwrite(_str.c_str(), size, 1, _file);
	}

	std::string Scene::readString(FILE* _file)
	{
		uint32_t size = 0;
		fread(&size, sizeof(uint32_t), 1, _file);
		char* buffer = new char[size];
		fread(buffer, size, 1, _file);
		std::string str(buffer);
		delete[] buffer;

		return str;
	}

	Scene::Scene()
		: m_filepath(nullptr)
	{
	}

	Scene::Scene(const char* _filepath)
		: m_filepath(_filepath)
	{
		FILE* file = nullptr;
		fopen_s(&file, _filepath, "rb");
		if (file != nullptr)
		{
			read(file);
			fclose(file);
		}
	}

	Scene::~Scene()
	{
		endMayaSession();
	}

	void Scene::update(double _dt)
	{
		if (isSessionValid())
		{
			m_mayaSession->update(m_models);
		}
	}

	std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world)
	{
		return _world->makeObject<Scene>();
	}

	std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath)
	{
		return _world->makeObject<Scene>(_filepath);
	}

	void Scene::save(const char* _filepath)
	{
		const char* filepath = nullptr;
		if (_filepath)
		{
			filepath = _filepath;
		}
		else
		{
			filepath = m_filepath;
		}

		if (filepath)
		{
			FILE* file = nullptr;
			fopen_s(&file, filepath, "wb");
			write(file);
			fclose(file);
		}
	}

	void Scene::beginMayaSession()
	{
		if (m_mayaSession = std::make_unique<MayaSession>(m_filepath))
		{
			m_mayaSession->begin(m_models);
		}
	}

	void Scene::endMayaSession()
	{
		m_mayaSession->end();
		m_mayaSession.release();
	}

	bool Scene::isSessionValid()
	{
		if (m_mayaSession)
		{
			return m_mayaSession->isValid();
		}
		else
		{
			return nullptr;
		}
	}

} // namespace mge
