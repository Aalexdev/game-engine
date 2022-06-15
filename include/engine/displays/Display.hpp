#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "../events/Event.hpp"
#include "../renderer/RenderAPI.hpp"
#include <libs/glm/glm.hpp>

namespace engine{
	class Application;

	class ENGINE_API Display{
		public:
			static constexpr uint32_t POSITION_CENTERED = static_cast<uint32_t>(-1);
			static constexpr uint32_t POSITION_UNKNOWN = static_cast<uint32_t>(-2);

			struct ENGINE_API Definition{
				glm::u32vec2 size = {1080, 720};
				glm::u32vec2 position = {POSITION_UNKNOWN, POSITION_UNKNOWN};
				glm::u32vec2 minimalSize = {0, 0};
				glm::u32vec2 maximalSize = {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1)};
				std::string title = "Game";
				std::filesystem::path iconPath;
				uint8_t monitorIndex = 0;
				bool bordered = true;
				bool fullscreen = false;
				bool resizable = true;
				float opacity = 1.f;
				RenderAPI renderAPI = RenderAPI::OPENGL;
				EventFn eventCallback = nullptr;
				Application *app = nullptr;
			};

			static Ref<Display> create(const Definition &def);
			virtual ~Display() = default;

			virtual glm::u32vec2 getSize() const = 0;
			virtual glm::u32vec2 getMinimalSize() const = 0;
			virtual glm::u32vec2 getMaximalSize() const = 0;
			virtual glm::u32vec2 getPosition() const = 0;
			virtual uint8_t getMonitorIndex() const = 0;
			virtual float getOpacity() const = 0;
			virtual bool isFullscreen() const = 0;
			virtual bool isBordered() const = 0;
			virtual const std::string &getTitle() const = 0;

			virtual void resize(const glm::u32vec2 &size) = 0;
			virtual void setMinimalSize(const glm::u32vec2 &size) = 0;
			virtual void setMaximalSize(const glm::u32vec2 &size) = 0;
			virtual void move(const glm::u32vec2 &position) = 0;
			virtual void setMonitor(uint8_t monitorIndex) = 0;
			virtual void setTitle(const std::string &title) = 0;
			virtual void setEventCallback(const EventFn &callback) = 0;

			virtual void* getNativeWindow() const = 0;
			virtual float getTime() const = 0; // time since the initialization of the first display in seconds
			virtual float getAspectRatio() const = 0;

			virtual void update() = 0;
	};
}