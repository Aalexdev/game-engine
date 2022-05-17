#include "engine/pch.hpp"
#include "engine/Application.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/filesystem/Filesystem.hpp"

namespace engine{
	Application::Application(){
		ENGINE_PROFILE_RECORD();
		ENGINE_PROFILE_BEGIN_SESSION("startup", "EngineProfilingStartup.json");

		ENGINE_PROFILE_FUNCTION();

		display = Window::create();
		display->setEventCallback(ENGINE_BIND_EVENT_FN(OnEvent));
		display->setVSync(false);
		input = display->getInputs();

		renderer = createRef<Renderer>();
		renderer->init();

		textures = createRef<Texture2DLibrary>();
		physicMaterials = PhysicMaterialLibrary::create();

		imGuiLayer = ImGuiLayer::create();
		pushLayer(imGuiLayer);
	}

	Application::~Application(){
		ENGINE_PROFILE_RECORD();
		ENGINE_PROFILE_BEGIN_SESSION("shutdown", "EngineProfilingShutdown.json");

		clearLayers();

		renderer->shutdown();
		imGuiLayer = nullptr;

		ENGINE_PROFILE_END_SESSION();
	}

	void Application::clearLayers(){
		for (auto &layer : layerStack){
			layerStack.popLayer(layer, layer != imGuiLayer);
		}
		layerStack.clear();
	}

	void Application::run(){
		ENGINE_PROFILE_END_SESSION();
		ENGINE_PROFILE_STOP_RECORD();
		ENGINE_PROFILE_BEGIN_SESSION("runtime", "EngineProfilingRuntime.json");

		Timestep timestep;
		Timestep lastTime;

		while (running){
			float time = display->getTime();
			timestep = time - lastTime;
			lastTime = time;

			if (!minimized){
				for (const Ref<Layer> &layer : layerStack)
					layer->OnUpdate(timestep);
			}
			
			imGuiLayer->begin();
			for (const Ref<Layer> &layer : layerStack)
				layer->OnImGuiRender();
			imGuiLayer->end();
			
			display->update();
		}

		ENGINE_PROFILE_END_SESSION();
	}

	void Application::OnEvent(Event &e){
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(OnWindowClosed));
		dispatcher.dispatch<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(OnWindowResized));

		for (auto it = layerStack.end(); it != layerStack.begin();){
			(*--it)->OnEvent(e);
			if (e.isHandled()) break;
		}
	}

	bool Application::OnWindowResized(WindowResizedEvent &e){
		if (e.getWidth() == 0 || e.getHeight() == 0){
			minimized = true;
			return false;
		}
		minimized = false;

		return false;
	}

	bool Application::OnWindowClosed(WindowCloseEvent &e){
		running = false;
		return true;
	}

	void Application::pushLayer(const Ref<Layer> &layer){
		layer->app = this;
		layer->renderer = renderer;
		layer->input = input;
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(const Ref<Layer> &overlay){
		overlay->app = this;
		overlay->renderer = renderer;
		overlay->input = input;
		layerStack.pushOverlay(overlay);
	}

	Ref<Texture2D> Application::loadTexture(const std::filesystem::path &path){
		Ref<Texture2D> texture;

		if (path.is_absolute()){
			texture = textures->load(path);
		} else{
			texture = textures->load(Filesystem::getDataFolderPath() / path);
		}

		return texture;
	}
}