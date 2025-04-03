/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/object.h"

#include <memory>

namespace mge
{
	class World;
	class Mesh;

	/// Model.
	/// 
	class Model : public Object
	{
	public:
		Model();
		~Model();

		/// Create a model.
		/// 
		/// @param[in] _world The world the model should exist in.
		/// 
		/// @returns Shared Model.
		/// 
		friend std::shared_ptr<Model> createModel(std::shared_ptr<World> _world);

		/// Add a mesh to the model.
		/// 
		/// @param[in] _mesh The mesh to be added.
		/// 
		void addMesh(std::shared_ptr<Mesh> _mesh);
	};

} // namespace mge