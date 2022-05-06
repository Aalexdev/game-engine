#pragma once

#include "Panel.hpp"
#include "../Event/EditorEvents.hpp"

namespace engine{
	class ToolbarPanel : public Panel{
		public:
			ToolbarPanel();
			~ToolbarPanel(){}
			
			virtual void OnImGuiRender() override;
			virtual void OnEvent(EditorEvent &e) override;

			virtual void serialize(YAML::Emitter &out) override;
			virtual void deserialize(YAML::Node data) override;

			virtual PanelType getStaticType() const override {return PanelType::TOOLBAR_PANEL;}

			static Ref<ToolbarPanel> create();
		
		private:

			enum class PanelRotation{
				VERTICAL,
				HORIZONTAL,
			};

			enum class RuntimeState{
				STOP,
				PLAY,
				PAUSE
			};

			// === Events ===
			bool OnEngineEvent(EngineEvent &e);
			bool OnKeyPressed(KeyPressedEvent &e);
			bool OnRuntimePaused(RuntimePausedEvent &e);
			bool OnRuntimePlayed(RuntimePlayedEvent &e);
			bool OnRuntimeStopped(RuntimeStoppedEvent &e);
			bool OnRuntimeStarted(RuntimeStartedEvent &e);

			// === ImGui ===
			void drawStartRuntimeButton();

			// === Members ===
			RuntimeState runtimeState = RuntimeState::STOP; // the current state of the runtime
			PanelRotation rotation = PanelRotation::HORIZONTAL; // the current rotation of the panel, used to draw items verticaly or horizontaly

			// = icons =
			Ref<Texture2D> playIcon;
			Ref<Texture2D> pauseIcon;
			Ref<Texture2D> stopIcon;

			// = Keys =
			KeyInput startKey = {{Key::KEY_F5}};
			KeyInput pauseKey = {{Key::KEY_F6}};
			KeyInput stopKey = {{Key::KEY_F10}};

			// === methodes ===
			void pause();
			void play();
			void stop();
			void start();

	};
}