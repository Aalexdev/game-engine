#pragma once

#include "../pch.hpp"
#include "../core.hpp"
#include "PhysicMaterial.hpp"

namespace engine{
	class ENGINE_API PhysicMaterialLibrary{
		public:
			static Ref<PhysicMaterialLibrary> create();

			PhysicMaterialLibrary();
			~PhysicMaterialLibrary(){}

			Ref<PhysicMaterial> get(const std::string &name);

			Ref<PhysicMaterial> push();
			Ref<PhysicMaterial> push(const PhysicMaterial &material);

			void pop(const std::string &name);

			std::map<std::string, Ref<PhysicMaterial>> &get();
		
		private:
			std::map<std::string, Ref<PhysicMaterial>> materials;
	};
}