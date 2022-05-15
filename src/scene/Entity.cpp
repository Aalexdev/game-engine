#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Entity.hpp"

namespace engine{
	void Entity::setParent(Entity parent){
		parent.pushChild(*this);
	}

	void Entity::pushChild(Entity child){
		auto &data = child.data();
		if (data.parent){
			Entity parent{data.parent, scene};
			auto parentData = parent.data();
			if (parentData.childs.size() > 0){
				auto it = std::find(parentData.childs.begin(), parentData.childs.end(), static_cast<ECS::Entity>(child));
				if (it != parentData.childs.end()){
					parentData.childs.erase(it);
				}
			}
		}

		data.parent = *this;
		this->data().childs.push_back(static_cast<ECS::Entity>(child));
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