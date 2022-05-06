#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Entity.hpp"



namespace engine{
	Entity::Entity(entt::entity handle, Scene *scene) : handle{handle}, scene{scene}{
		ENGINE_PROFILE_FUNCTION();
	}
}