#include <engine/engine.hpp>

int main(int argc, char **argv){
	engine::Filesystem::init(argc, argv);
	engine::Application::Definition def;

	def.displayCount = 1;
	std::vector<engine::Display::Definition> displayDefinitions(def.displayCount);

	for (int i=0; i<def.displayCount; i++){
		auto &def = displayDefinitions[i];
		def.position.x = 100 * i + 100;
		def.position.y = 100 * i + 100;
	}

	def.displaysDefinitions = displayDefinitions.data();

	engine::Ref<engine::Application> app;
	try {
		app = engine::Application::create(def);
	} catch (const engine::exceptions::Exception &e){
		ENGINE_ERROR(e.what());
		return 1;
	}

	try {
		app->run();
	} catch (const engine::exceptions::Exception &e){
		ENGINE_ERROR(e.what());
		return 1;
	}

	return 0;
}