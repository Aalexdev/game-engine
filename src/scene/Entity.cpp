#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Entity.hpp"

namespace engine{
	void Entity::setParent(Entity parent){
		parent.pushChild(*this);
	}

	void Entity::pushChild(Entity child){
		child.resetParent();
		child.data().parent = *this;
		data().childs.push_back(static_cast<ECS::Entity>(child));
	}

	void Entity::resetParent(){
		Entity parent = {data().parent, scene};
		if (parent){
			auto &data = parent.data();
			auto it = std::find(data.childs.begin(), data.childs.end(), handle);
			if (it != data.childs.end()){
				data.childs.erase(it);
			}
		}
		data().parent = ECS::null;
	}
	
	bool Entity::isParent(Entity parent){
		return data().parent == parent.handle;
	}

	bool Entity::isChild(Entity child){
		auto &data = this->data();
		auto it = std::find(data.childs.begin(), data.childs.end(), static_cast<ECS::Entity>(child));
		return it != data.childs.end();
	}
}