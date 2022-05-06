#include "engine/pch.hpp"
#include "engine/math/Math.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <libs/glm/gtx/matrix_decompose.hpp>
#include <libs/glm/gtc/epsilon.hpp>

namespace engine::math{
	ENGINE_API bool decomposeTransform(const glm::mat4 &mat, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation){
		glm::mat4 localMatrix(mat);

		if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
			return false;
		
		if (
			glm::epsilonNotEqual(localMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[2][3], static_cast<float>(0), glm::epsilon<float>())){
				localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
				localMatrix[3][3];
			}
		
		translation = glm::vec3(localMatrix[3]);
		localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

		glm::vec3 row[3], Pdum3;

		for (glm::length_t i=0; i<3; i++){
			for (glm::length_t j=0; j<3; j++){
				row[i][j] = localMatrix[i][j];
			}
		}

		scale.x = glm::length(row[0]);
		row[0] = glm::detail::scale(row[0], static_cast<float>(1));
		scale.y = glm::length(row[1]);
		row[1] = glm::detail::scale(row[1], static_cast<float>(1));
		scale.z = glm::length(row[2]);
		row[2] = glm::detail::scale(row[2], static_cast<float>(1));

		rotation.y = glm::asin(-row[0][2]);

		if (glm::cos(rotation.y) != 0){
			rotation.x = atan2(row[1][2], row[2][2]);
			rotation.z = atan2(row[0][1], row[0][0]);
		} else {
			rotation.x = atan2(-row[2][0], row[1][1]);
			rotation.z = 0;
		}

		return true;
	}

	ENGINE_API bool decomposeTransform2D(const glm::mat4 &mat, glm::vec2 &translation, glm::vec2 &scale, float &rotation){
		glm::vec3 translation3D;
		glm::vec3 scale3D;
		glm::vec3 rotation3D;

		if (!decomposeTransform(mat, translation3D, scale3D, rotation3D)) return false;

		translation = glm::vec2(translation3D);
		scale = glm::vec2(scale3D);
		rotation = rotation3D.z;
		return true;
	}
}