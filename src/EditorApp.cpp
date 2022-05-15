#include <engine/engine.hpp>

#include "EditorLayer.hpp"
#include <engine/dataStruct/Set.hpp>

namespace engine{
	class EditorApp : public Application{
		public:
			EditorApp() : Application(){
				pushLayer<EditorLayer>();
			}
			~EditorApp() {}
	};
}

int main(int argc, char **argv){
	engine::Window::setWindowAPI(engine::Window::WindowAPI::GLFW);
	engine::Renderer::setRenderAPI(engine::Renderer::RenderAPI::OpenGL);
	engine::Filesystem::init(argc, argv);

	engine::EditorApp app;
	app.run();
	return EXIT_SUCCESS;
}