#pragma once

#include <engine/engine.hpp>
#include "panels/PanelLibrary.hpp"
#include "EditorSerializer.hpp"
#include "Event/EditorEvents.hpp"

namespace engine{
	class EditorLayer : public Layer{
		friend class Panel;
		friend class SceneHierarchyPanel;
		friend class EditorSerializer;
		friend class ScenePanel;

		public:
			EditorLayer() : Layer(){}
			~EditorLayer(){}

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnUpdate(Timestep timestep) override;
			virtual void OnImGuiRender() override;
			virtual void OnEvent(Event &event) override;

			void serialize(YAML::Emitter &out);
			void deserialize(YAML::Node data);

			const glm::vec4& getIcon(const std::string &iconTag) const {
				auto it = icons.find(iconTag);
				if (it == icons.end()){
					return glm::vec4(0, 0, 1, 1);
				}
				return iconUVs[it->second];
			}

			Ref<Texture2D> getIcons() {return iconAtlas;}

			glm::vec4 pushIcon(const std::string &tag, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY);

		private:

			// === Events ===
			void OnEditorEvent(EditorEvent &e);

			bool OnKeyPressed(KeyPressedEvent &e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent &e);
			bool OnMouseMoved(MouseMovedEvent &e);
			bool OnEntitySelected(EntitySelectedEvent &e);
			bool OnEntityRemoved(EntityRemovedEvent &e);
			bool OnRuntimePaused(RuntimePausedEvent &e);
			bool OnRuntimePlayed(RuntimePlayedEvent &e);
			bool OnRuntimeStopped(RuntimeStoppedEvent &e);
			bool OnRuntimeStarted(RuntimeStartedEvent &e);

			// === ImGui ===
			// guizmo
			void drawGuizmo();
			void guizmoTranslate() {guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;}
			void guizmoRotate() {guizmoOperation = ImGuizmo::OPERATION::ROTATE_Z;}
			void guizmoScale() {guizmoOperation = ImGuizmo::OPERATION::SCALE;}

			// popups
			void drawStartupMenu();

			// etc
			void drawMenuBar();
			void drawViewport();

			// file dialog
			void drawFileDialogs();
			void drawOpenFileDialog();
			void drawSaveAsFileDialog();

			// === Other methodes ===
			void pushRecentProject(const std::filesystem::path &path); // push the given pathg into the mast project history
			void setProjectName(std::string name); // set the name of the current progect
			void open();
			void openProject(const std::filesystem::path &path);
			void save();
			void saveAs();
			Entity createEntity(const std::string &tag = "");
			void selectEntity(Entity entity);
			void deleteSelectedEntity();

			// === members ===
			Ref<EditorCamera> camera;
			Ref<Framebuffer> framebuffer;
			Ref<Scene> activeScene;
			Ref<Scene> editorScene;
			Ref<SceneSerializer> serializer;

			glm::vec2 viewportSize = {1080.f, 720.f}; // the current size of the viewport
			glm::vec2 viewportBounds[2];  // the start and end position of the viewport relative to the window
			std::string currentProjectPath;

			PanelLibrary panels; // the panels
			Timestep delta; // the time in seconds between the current and the last frame

			bool mouseOnViewport = false; // if the mouse is hovering the viewport
			bool freezeInput = true;
			bool startupPopup = true;
			bool runtime = false; // if the reuntime is currently activated

			Entity selectedEntity = {}; // the currently selected entity
			ImGuizmo::OPERATION guizmoOperation = ImGuizmo::OPERATION::TRANSLATE; // the current operation of the guizmo
			
			Ref<EditorSerializer> editorSerializer;
			std::array<std::filesystem::path, 10> recentProjects; // the last 10 opened projects

			// keys
			KeyInput saveProjectKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_S}};
			KeyInput saveProjectAsKey = {{Key::KEY_LEFT_SHIFT, Key::KEY_RIGHT_SHIFT}, {Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_S}};
			KeyInput openProjectKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_O}};
			KeyInput exitKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_Q}};
			KeyInput rotateGizmoKey = {{Key::KEY_LEFT_SHIFT, Key::KEY_RIGHT_SHIFT}, {Key::KEY_R}};
			KeyInput scaleGizmoKey = {{Key::KEY_LEFT_SHIFT, Key::KEY_RIGHT_SHIFT}, {Key::KEY_S}};
			KeyInput translateGizmoKey = {{Key::KEY_LEFT_SHIFT, Key::KEY_RIGHT_SHIFT}, {Key::KEY_T}};
			KeyInput newEntityKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_N}};
			KeyInput deleteSelectedEntityKey = {{Key::KEY_DELETE, Key::KEY_BACKSPACE}};

			std::vector<glm::vec4> iconUVs;
			std::vector<glm::vec4> iconRelativeUVs;
			std::map<std::string, size_t> icons;
			Ref<Texture2D> iconAtlas;
			std::filesystem::path iconAtlasPath;
	};
}