#pragma once

#include "../core.hpp"


namespace engine{
	class Window;

	class ENGINE_API Context{
		public:
			virtual ~Context() {}

			virtual void init() = 0;
			virtual void swapBuffers() = 0;
			virtual void enableVSync(bool enable) = 0;

			static Ref<Context> create(Window *window);
	};
}