#pragma once

#include "../core.hpp"
#include "Camera.hpp"
#include "../Events/Input.hpp"
#include "../Events/MouseEvent.hpp"
#include "../Timestep.hpp"

namespace engine{
	class ENGINE_API EditorCamera{
		public:
			EditorCamera(const Ref<Input> &inputs) : input{inputs}{}

			static Ref<EditorCamera> create(const Ref<Input> &input) {return createRef<EditorCamera>(input);}

			void OnUpdate(Timestep ts);
			void OnEvent(Event &e);

			void OnViewportResized(uint32_t width, uint32_t height);

			inline const Camera& getCamera() const {return camera;}

			inline float getSpeed() const {return speed;}

		private:
			bool OnMouseScrolled(MouseScrolledEvent &e);
			void updateProjection();

			Ref<Input> input;
			Camera camera;
			float zoom = 1.f;
			float aspectRatio = 1.f;
			float speed = 0.002f;

			glm::vec2 lastMousePos;
	};
}