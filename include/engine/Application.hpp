#pragma once

#include "core.hpp"
#include "windows/Window.hpp"
#include "layer/LayerStack.hpp"
#include "layer/ImGuiLayer/ImGuiLayer.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "renderer/Texture2DLibrary.hpp"
#include "scene/PhysicMaterialLibrary.hpp"

#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/ApplicationEvent.hpp"

namespace engine{
	class ENGINE_API Application{
		public:
			Application();
			~Application();

			void run();

			inline Window& getDisplay() const {return *display;}

			void pushLayer(const Ref<Layer> &layer);
			void pushOverlay(const Ref<Layer> &overlay);

			template<typename T, typename... Args>
			void pushLayer(Args&&... args){
				pushLayer(createRef<T>(args...));
			}

			template<typename T, typename... Args>
			void pushOverlay(Args&&... args){
				pushLayer(createRef<T>(args...));
			}

			inline const Ref<Renderer>& getRenderer() const {return renderer;}

			inline void loadShader(const std::string &name, const std::string &filepath) {shaders.push(name, filepath);}
			inline void loadShader(const std::string &filepath) {shaders.push(filepath);}

			Ref<Texture2D> loadTexture(const std::filesystem::path &path);

			inline void close() {running = false;}

			Ref<Texture2DLibrary> getTextures() {return textures;}
			Ref<PhysicMaterialLibrary> getPhysicMaterials() {return physicMaterials;}
		
		protected:
			ShaderLibrary shaders;
			Ref<Texture2DLibrary> textures;
			Ref<PhysicMaterialLibrary> physicMaterials;
			Ref<Renderer> renderer;
			Ref<Input> input;
		
		private:
			void OnEvent(Event &e);
			bool OnWindowResized(WindowResizedEvent &e);
			bool OnWindowClosed(WindowCloseEvent &e);

			void clearLayers();
			
			Ref<Window> display;

			LayerStack layerStack;
			Ref<ImGuiLayer> imGuiLayer;

			bool running = true;
			bool minimized = false;
	};
}