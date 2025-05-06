/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/object.h"
#include "engine/math.h"

#include <maya-bridge/shared_buffer.h>
#include <maya-bridge/shared_data.h>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace mge
{
	class World;
	class Model;
	class Material;
	class Texture;

	/// Maya Bridge Session.
	/// 
	class MayaSession
	{
		friend class Scene;

		void begin(std::unordered_map<std::string, std::shared_ptr<Model>>& _models);
		void update(std::unordered_map<std::string, std::shared_ptr<Model>>& _models);
		void end();
		bool isValid();

		void modelAdded(std::unordered_map<std::string, std::shared_ptr<Model>>& _models, const mb::Model& _model);
		void materialAdded(std::unordered_map<std::string, std::shared_ptr<Model>>& _models, const mb::Material& _material);

	public:
		MayaSession(const char* _filepath);
		~MayaSession();

	private:
		const char* m_filepath;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials; 
		std::unique_ptr<mb::SharedBuffer> m_writeBuffer;
		std::unique_ptr<mb::SharedBuffer> m_readBuffer;
		mb::SharedData m_shared;
	};

	/// Scene.
	/// 
	class Scene : public Object
	{
		friend class GBuffer;
		friend class ShadowMapping;

		void write(FILE* _file);
		void read(FILE* _file);

		void writeTexture(FILE* _file, std::shared_ptr<Texture> _texture);
		std::shared_ptr<Texture> readTexture(FILE* _file);

		void writeString(FILE* _file, const std::string& _str);
		std::string readString(FILE* _file);

	public:
		Scene();
		Scene(const char* _filepath);
		~Scene();

		/// Create a scene.
		/// 
		/// @param[in] _world World to create scene in.
		/// 
		/// @remark A scene can only hold entities of type Model.
		/// 
		/// @returns Shared Scene.
		/// 
		friend std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world);

		/// Load a scene.
		/// 
		/// @param[in] _world World to create scene in.
		/// @param[in] _filepath Path to load saved scene from.
		/// 
		/// @remark A scene can only hold entities of type Model.
		/// 
		/// @returns Shared Scene.
		/// 
		friend std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath);

		/// Save the scene.
		/// 
		/// @param[in] _filepath Path to save scene to.
		/// 
		/// @remark If a path is passed as nullptr, it will save to loaded path.
		/// 
		void save(const char* _filepath);

		/// Begin maya session.
		/// 
		/// @remark Maya plugin should be enabled before calling this.
		/// 
		void beginMayaSession();

		/// End maya session.
		/// 
		void endMayaSession();

		/// Is maya session valid.
		/// 
		/// @return If the maya session is valid.
		///
		bool isSessionValid();

	protected:
		void update(double _dt) override;

	private:
		const char* m_filepath;
		std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
		std::unique_ptr<MayaSession> m_mayaSession;
	};

} // namespace mge
