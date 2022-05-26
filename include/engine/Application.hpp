#pragma once

#include "core.hpp"
#include "windows/Window.hpp"
#include "layer/LayerStack.hpp"
#include "layer/ImGuiLayer/ImGuiLayer.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "renderer/Texture2DLibrary.hpp"
#include "scene/PhysicMaterialLibrary.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneSerializer.hpp"

#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/ApplicationEvent.hpp"

// std
#include <condition_variable>
#include <mutex>

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

			/**
			 * @brief load the image and the given path
			 * @param path th image to load as a texture
			 * @return Ref<Texture2D> 
			 */
			Ref<Texture2D> loadTexture(const std::filesystem::path &path);

			/**
			 * @brief close the application
			 * 
			 */
			inline void close() {running = false;}

			Ref<Scene> getScene();

			Ref<Texture2DLibrary> getTextures() {return textures;}
			Ref<PhysicMaterialLibrary> getPhysicMaterials() {return physicMaterials;}
			
			bool __gameThreadFinished() const {return gameThreadFinished;}
			bool __renderThreadFinished() const {return renderThreadFinished;}
		
		private:
			ShaderLibrary shaders;
			Ref<Texture2DLibrary> textures;
			Ref<PhysicMaterialLibrary> physicMaterials;
			Ref<Renderer> renderer;
			Ref<Input> input;
			Ref<Window> display;
			Ref<ImGuiLayer> imGuiLayer;
			Ref<Scene> scene;
			Ref<SceneSerializer> sceneSerializer;

			LayerStack layerStack;

			bool running = true;
			bool minimized = false;

			bool gameThreadFinished = false;
			bool renderThreadFinished = false;
			std::condition_variable gameThreadCond;
			std::condition_variable renderThreadCond;
			std::mutex gameThreadLock;
			std::mutex renderThreadLock;

			static void gameThread(Application* app);

			static void gameThreadBegin(Application* app);
			static void renderThreadBegin(Application* app);
			static void gameThreadEnd(Application* app);
			static void renderThreadEnd(Application* app);
			static void waitForGameThread(Application* app);
			static void waitForrRenderThread(Application* app);
	
			void OnEvent(Event &e);
			bool OnWindowResized(WindowResizedEvent &e);
			bool OnWindowClosed(WindowCloseEvent &e);

			void clearLayers();
			
	};
}