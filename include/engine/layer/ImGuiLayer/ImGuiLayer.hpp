#pragma once

#include "../../core.hpp"
#include "../Layer.hpp"

namespace engine{
    class ENGINE_API ImGuiLayer : public Layer{
        public:
            ImGuiLayer() : Layer("ImGuiLayer") {}
			virtual ~ImGuiLayer(){}

            virtual void begin() = 0;
            virtual void end() = 0;

			static Ref<ImGuiLayer> create();
			static void setStyle();
    };
}