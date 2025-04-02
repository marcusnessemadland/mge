/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "maya-bridge/shared_buffer.h"
#include "maya-bridge/shared_data.h"

#include "engine/math.h"
#include "engine/entity.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace vr
{
	class World;
	class Model;
	class Mesh;
	class SubMesh;
	class Material;
	class Texture;

	/// Can only hold models
	class Scene : public Entity
	{
		friend class Renderer;
		friend class GBuffer;

		void initBridge();
		void shutdownBridge();
		bool isUsingBridge();

		void modelAdded(const mb::Model& _model);
		void materialAdded(const mb::Material& _material);

		void write(FILE* _file);
		void read(FILE* _file);

		void writeTexture(FILE* _file, std::shared_ptr<Texture> _texture);
		std::shared_ptr<Texture> readTexture(FILE* _file);

		void writeString(FILE* _file, const std::string& _str);
		std::string readString(FILE* _file);

	public:
		Scene(bool _usingMaya);
		Scene(const char* _filepath, bool _usingMaya);
		~Scene();

		void update(double _dt) override;

		/// 
		friend std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world, bool _usingMaya = false);

		/// 
		friend std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath, bool _usingMaya = false);

		/// 
		void save();

	private:
		const char* m_filepath;
		std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials; // only used in editor

		std::unique_ptr<mb::SharedBuffer> m_writeBuffer;
		std::unique_ptr<mb::SharedBuffer> m_readBuffer;
		mb::SharedData m_shared;
	};
}
