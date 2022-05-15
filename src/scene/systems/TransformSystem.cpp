#include "engine/scene/systems/TransformSystem.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/Scene.hpp"
#include <libs/glm/gtc/epsilon.hpp>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtx/matrix_transform_2d.hpp>

// https://www.youtube.com/watch?v=7tCrRrf9CrM

namespace engine::ECS::systems{
	Transform::Transform(Scene *scene) : scene{scene}{}
	void Transform::init(){
		auto &registry = scene->getRegistry();
		Signature signature{};
		signature.set(registry.getComponentType<components::Transform>());
		registry.setSystemSignature<Transform>(signature);
	}

	void Transform::update(){
		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &transform = entity.getComponent<components::Transform>();

			if (transform.lastTransform != transform.transform){
				auto &mat = transform.transformMat;
				auto &tr = transform.transform;

				if (entity.hasChildren()){
					for (auto child : entity){
						auto &childTransform = child.getComponent<components::Transform>();
						childTransform.transform += (transform.transform - transform.lastTransform);

						glm::vec2 u = childTransform.transform.translation - tr.translation;
						glm::vec2 w;

						u *= tr.scale / transform.lastTransform.scale;

						float alpha = tr.rotation - transform.lastTransform.rotation;
						const float c = cos(alpha);
						const float s = sin(alpha);
						
						w.x = c * u.x - s * u.y;
						w.y = s * u.x + c * u.y;

						childTransform.transform.translation = w + tr.translation;

						// childTransform.transform.translation = glm::vec3(childTransform.transform.translation, 0.f) * childTransformMat;

					}
				}

				// if the rotation is 0, it's useless to calculate the rotation
				if (glm::epsilonNotEqual(tr.rotation, 0.f, glm::epsilon<float>())){
					mat = glm::translate(glm::mat4(1.f), glm::vec3(tr.translation, 0.f)) * glm::rotate(glm::mat4(1.f), tr.rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), glm::vec3(tr.scale, 1.f));
				} else {
					mat = glm::translate(glm::mat4(1.f), glm::vec3(tr.translation, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(tr.scale, 1.f));
				}

				transform.lastTransform = transform.transform;
			}
		}
	}
}