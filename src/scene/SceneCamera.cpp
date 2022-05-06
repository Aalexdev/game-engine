#include "engine/Scene/SceneCamera.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

namespace engine{
	
	void SceneCamera::updateView(){
		float left = -0.5f * aspecRatio * zoomLevel;
		camera.orthographic(left, -left, -zoomLevel / 2.f, zoomLevel / 2.f, -1.f, 1.f);
	}

	void SceneCamera::updateProjection(){
		camera.update();
	}

}