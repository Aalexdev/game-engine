#pragma once
#include "ImGuiLayer.hpp"

namespace engine{
	class ENGINE_API OpenGlImGuiLayer : public ImGuiLayer{
		public:
			OpenGlImGuiLayer() : ImGuiLayer(){}
			virtual ~OpenGlImGuiLayer() {}

			static Ref<OpenGlImGuiLayer> create();
	};

	class ENGINE_API GLFWOpenGlImGuiLayer : public OpenGlImGuiLayer{
		public:
			GLFWOpenGlImGuiLayer() : OpenGlImGuiLayer(){}
			~GLFWOpenGlImGuiLayer(){}

			virtual void OnAttach() override;
			virtual void OnDetach() override;

			virtual void begin() override;
			virtual void end() override;
	};

	class ENGINE_API SDL2OpenGlImGuiLayer : public OpenGlImGuiLayer{
		public:
		 	SDL2OpenGlImGuiLayer() : OpenGlImGuiLayer(){}
			~SDL2OpenGlImGuiLayer(){}

			virtual void OnAttach() override;
			virtual void OnDetach() override;

			virtual void begin() override;
			virtual void end() override;
	};
}