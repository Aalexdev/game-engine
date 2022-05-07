#include "panels/ToolbarPanel.hpp"

namespace engine{
	Ref<ToolbarPanel> ToolbarPanel::create(){
		return createRef<ToolbarPanel>();
	}

	ToolbarPanel::ToolbarPanel(){
		playIcon = Texture2D::create((Filesystem::getDataFolderPath() / "icons/toolbar/play.png").string());
		pauseIcon = Texture2D::create((Filesystem::getDataFolderPath() / "icons/toolbar/pause.png").string());
		stopIcon = Texture2D::create((Filesystem::getDataFolderPath() / "icons/toolbar/stop.png").string());
	}


	void ToolbarPanel::OnImGuiRender(){
		if (ImGui::Begin("toolbar")){
			
			{
				ImVec2 size = ImGui::GetWindowSize();

				if (size.x > size.y){
					rotation = PanelRotation::HORIZONTAL;
				} else {
					rotation = PanelRotation::VERTICAL;
				}
			}

			switch (rotation){
				case PanelRotation::HORIZONTAL:{
					drawStartRuntimeButton();
					// ImGui::SameLine();
					break;
				}

				case PanelRotation::VERTICAL:{
					drawStartRuntimeButton();
					break;
				}
			}
		}
		ImGui::End();
	}

	void ToolbarPanel::OnEvent(EditorEvent &e){
		EditorEventDispatcher dispatcher(e);

		dispatcher.dispatch<EngineEvent>(ENGINE_BIND_EVENT_FN(OnEngineEvent));
		dispatcher.dispatch<RuntimePausedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimePaused));
		dispatcher.dispatch<RuntimePlayedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimePlayed));
		dispatcher.dispatch<RuntimeStartedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimeStarted));
		dispatcher.dispatch<RuntimeStoppedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimeStopped));
	}

	void ToolbarPanel::serialize(YAML::Emitter &out){
		out << YAML::Key << "StartKey" << YAML::Value << startKey;
		out << YAML::Key << "PauseKey" << YAML::Value << pauseKey;
		out << YAML::Key << "StopKey" << YAML::Value << stopKey;
	}

	void ToolbarPanel::deserialize(YAML::Node data){
		startKey = data["StartKey"].as<KeyInput>();
		pauseKey = data["PauseKey"].as<KeyInput>();
		stopKey = data["StopKey"].as<KeyInput>();
	}

	// === Events ===

	bool ToolbarPanel::OnEngineEvent(EngineEvent &e){
		EventDispatcher dispatcher(e.getEvent());

		dispatcher.dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
		return false;
	}

	bool ToolbarPanel::OnKeyPressed(KeyPressedEvent &e){
		
		switch (runtimeState){
			case RuntimeState::PAUSE:{
				if (inputs->isDown(startKey, e)){
					play();
				}

				if (inputs->isDown(stopKey, e)){
					stop();
				}
			}

			case RuntimeState::PLAY:{
				if (inputs->isDown(stopKey, e)){
					stop();
				}

				if (inputs->isDown(pauseKey, e)){
					pause();
				}
			}

			case RuntimeState::STOP:{
				if (inputs->isDown(startKey, e)){
					start();
				}
			}
		}

		return false;
	}

	
	bool ToolbarPanel::OnRuntimePaused(RuntimePausedEvent &e){
		runtimeState = RuntimeState::PAUSE;
		return false;
	}

	bool ToolbarPanel::OnRuntimePlayed(RuntimePlayedEvent &e){
		runtimeState = RuntimeState::PLAY;
		return false;
	}

	bool ToolbarPanel::OnRuntimeStopped(RuntimeStoppedEvent &e){
		runtimeState = RuntimeState::STOP;
		return false;
	}

	bool ToolbarPanel::OnRuntimeStarted(RuntimeStartedEvent &e){
		runtimeState = RuntimeState::PLAY;
		return false;
	}


	// === ImGui ===

	void ToolbarPanel::drawStartRuntimeButton(){
		auto region = ImGui::GetContentRegionAvail();

		ImVec2 buttonSize;
		if (rotation == PanelRotation::HORIZONTAL){
			buttonSize = {region.y-10, region.y-10};
		} else {
			buttonSize = {region.x-10, region.x-10};
		}

		ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});

		switch (runtimeState){
			case RuntimeState::PAUSE:{

				// if the panel is horizontal, we center the buttons
				if (rotation == PanelRotation::HORIZONTAL){
					ImGui::SetCursorPosX(region.x / 2 - buttonSize.x);
				}

				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(playIcon->getTexture()), buttonSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y})){
					play();
				}

				if (ImGui::IsItemHovered()){
					ImGui::BeginTooltip();
					ImGui::Text(startKey.toString().c_str());
					ImGui::EndTooltip();
				}

				if (rotation == PanelRotation::HORIZONTAL) ImGui::SameLine();

				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(stopIcon->getTexture()), buttonSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y})){
					stop();
				}

				if (ImGui::IsItemHovered()){
					ImGui::BeginTooltip();
					ImGui::Text(stopKey.toString().c_str());
					ImGui::EndTooltip();
				}
				break;

			}

			case RuntimeState::PLAY:{
				if (rotation == PanelRotation::HORIZONTAL){
					ImGui::SetCursorPosX(region.x / 2 - buttonSize.x);
				}

				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(pauseIcon->getTexture()), buttonSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y})){
					pause();
				}

				if (ImGui::IsItemHovered()){
					ImGui::BeginTooltip();
					ImGui::Text(pauseKey.toString().c_str());
					ImGui::EndTooltip();
				}

				if (rotation == PanelRotation::HORIZONTAL) ImGui::SameLine();

				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(stopIcon->getTexture()), buttonSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y})){
					stop();
				}

				if (ImGui::IsItemHovered()){
					ImGui::BeginTooltip();
					ImGui::Text(stopKey.toString().c_str());
					ImGui::EndTooltip();
				}

				break;
			}

			case RuntimeState::STOP:{
				if (rotation == PanelRotation::HORIZONTAL){
					ImGui::SetCursorPosX((region.x / 2) - (buttonSize.x / 2));
				}

				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(playIcon->getTexture()), buttonSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y})){
					start();
				}
				if (ImGui::IsItemHovered()){
					ImGui::BeginTooltip();
					ImGui::Text(startKey.toString().c_str());
					ImGui::EndTooltip();
				}

				break;
			}
		}

		ImGui::PopStyleColor();
	}

	// === methodes ===

	void ToolbarPanel::pause(){
		RuntimePausedEvent e;
		callback(e);
	}

	void ToolbarPanel::play(){
		RuntimePlayedEvent e;
		callback(e);
	}

	void ToolbarPanel::stop(){
		RuntimeStoppedEvent e;
		callback(e);
	}

	void ToolbarPanel::start(){
		RuntimeStartedEvent e;
		callback(e);
	}
}