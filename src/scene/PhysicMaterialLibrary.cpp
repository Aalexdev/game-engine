#include "engine/pch.hpp"
#include "engine/scene/PhysicMaterialLibrary.hpp"

namespace engine{
	Ref<PhysicMaterialLibrary> PhysicMaterialLibrary::create(){
		return createRef<PhysicMaterialLibrary>();
	}

	PhysicMaterialLibrary::PhysicMaterialLibrary(){
		PhysicMaterial material; // the default material
		material.name = "default";
		push(material);
	}

	Ref<PhysicMaterial> PhysicMaterialLibrary::get(const std::string &name){
		if (name.empty()){
			return get("default");
		}
		
		auto it = materials.find(name);
		if (it == materials.end()) return get("default");
		return it->second;
	}

	Ref<PhysicMaterial> PhysicMaterialLibrary::push(){
		Ref<PhysicMaterial> materialPtr = createRef<PhysicMaterial>();
		materials[materialPtr->name] = materialPtr;
		return materialPtr;
	}

	Ref<PhysicMaterial> PhysicMaterialLibrary::push(const PhysicMaterial &material){
		Ref<PhysicMaterial> materialPtr = createRef<PhysicMaterial>(material);
		materials[material.name] = materialPtr;
		return materialPtr;
	}
	
	std::map<std::string, Ref<PhysicMaterial>> &PhysicMaterialLibrary::get(){
		return materials;
	}

	void PhysicMaterialLibrary::pop(const std::string &name){
		auto it = materials.find(name);
		if (it == materials.end() || name == "default") return;
		materials.erase(it);
	}
}