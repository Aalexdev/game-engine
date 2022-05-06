#pragma once

#include "../core.hpp"
#include "../Events/Event.hpp"
#include "../Timestep.hpp"
#include "../renderer/Renderer.hpp"
#include "../Events/Input.hpp"

namespace engine{

	class Application;

    class ENGINE_API Layer{
        public:
            Layer(const std::string& name = "Layer") : name{name}{}
            virtual ~Layer() {}

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate(Timestep timestep) {}
            virtual void OnImGuiRender() {}
            virtual void OnEvent(Event &event) {}

            inline const std::string &getName() const {return name;}
        
        private:
            std::string name;

			friend class Application;
		
		protected:
			Application* app;
			Ref<Renderer> renderer;
			Ref<Input> input;
    };
}