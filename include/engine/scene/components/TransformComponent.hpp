#pragma once

#include "../../core.hpp"
#include <libs/glm/glm.hpp>

namespace engine{
	struct Transform{
		glm::vec2 translation = {0.f, 0.f};
		glm::vec2 scale = {1.f, 1.f};
		float rotation = 0.f;

		bool operator==(const Transform &other){
			if (translation == other.translation){
				if (scale == other.scale){
					if (rotation == other.rotation){
						return true;
					}
				}
			}
			return false;
		}
		
		bool operator!=(const Transform &other){
			return !(*this == other);
		}

		Transform operator+(const Transform &other){
			Transform out;
			out.translation = translation + other.translation;
			out.scale = scale + other.scale;
			out.rotation = rotation + other.rotation;
			return out;
		}

		Transform operator-(const Transform &other){
			Transform out;
			out.translation = translation - other.translation;
			out.scale = scale - other.scale;
			out.rotation = rotation - other.rotation;
			return out;
		}

		Transform& operator+=(const Transform &other){
			translation += other.translation;
			scale += other.scale;
			rotation += other.rotation;
			return *this;
		}
	};
}

namespace engine::ECS::components{
	struct ENGINE_API Transform{
		engine::Transform transform;

		//
		engine::Transform lastTransform{.rotation=1.f};

		Transform(const Transform &other){
			transform = other.transform;
			lastTransform = other.lastTransform;
		}

		Transform() = default;
		~Transform() = default;

		glm::mat4 transformMat;
	};
}