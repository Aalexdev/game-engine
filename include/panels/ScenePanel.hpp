#pragma once

#include "Panel.hpp"

namespace engine{
	class ScenePanel : public Panel{
		public:
			ScenePanel() = default;
			ScenePanel(const ScenePanel &) = default;
			~ScenePanel() = default;

			virtual void OnImGuiRender() override;
			virtual void OnEvent(EditorEvent &e) override;

			virtual void serialize(YAML::Emitter &out) override;
			virtual void deserialize(YAML::Node data) override;

			virtual PanelType getStaticType() const override {return PanelType::SCENE_PANEL;}

			static Ref<ScenePanel> create();

	};
}