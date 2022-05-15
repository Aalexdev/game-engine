#pragma once

#include "Panel.hpp"
#include "../Event/EditorEvents.hpp"

#include <engine/engine.hpp>

#include <filesystem>

namespace engine{
	class ContentBrowserPanel : public Panel{
		public:
			ContentBrowserPanel();
			~ContentBrowserPanel(){}

			// delete copy
			ContentBrowserPanel(const ContentBrowserPanel &) = delete;
			ContentBrowserPanel &operator=(const ContentBrowserPanel &) = delete;

			virtual void OnImGuiRender() override;
			virtual void OnEvent(EditorEvent &e) override;

			virtual void serialize(YAML::Emitter &out) override;
			virtual void deserialize(YAML::Node data) override;

			virtual PanelType getStaticType() const override {return PanelType::CONTENT_BROWSER_PANEL;}

			static Ref<ContentBrowserPanel> create();

		private:

			// === types ===
			enum class ViewType{
				VIEWTYPE_ICON,
				VIEWTYPE_HIERARCHY
			};

			// === events ===
			// editor
			bool OnNewSelection(ContentBrowserNewSelectionEvent e);
			bool OnFileOpened(ContentBrowserFileOpenedEvent e);
			bool OnDirectoryOpened(ContentBrowserDirectoryOpenedEvent e);
			bool OnCreated(ContentBrowserCreatedEvent e);

			// engine
			bool OnEngineEvent(EngineEvent e);
			bool OnKeyPressed(KeyPressedEvent e);

			// === methodes ===
			void select(std::filesystem::path path = "");
			void createFile(const std::filesystem::path &path);
			void createFile(const std::filesystem::path &parentPath, const std::string &filename, bool rename = true);
			bool createDirectory(std::filesystem::path path, bool rename = true);
			void createDirectory(std::filesystem::path parentPath, const std::string &filename, bool rename = true);
			void createDirectory();
			void openSelsection();
			void openDirectory(std::filesystem::path path);
			void renameSelection(); // open the renaming text input under the selected file / directory if there is
			void deleteSelection();
			void zoomIn();
			void zoomOut();
			void goToParent();

			// === imgui ===
			void drawIconsView();
			// void drawHierarchyView();

			void fileContextItem(const std::filesystem::path &path);
			void directoryContextItem(const std::filesystem::path &path);
			void voidContextItem();

			// === members ===
			std::filesystem::path selection;
			std::filesystem::path currentFolder;
			std::filesystem::path browserRoot;

			bool renaming = false;
			char renameBuffer[255]; // this is used as a buffer for the text input when renaming a file / directory

			bool isMouseHover = false;

			// icons
			std::string folderIcon;
			std::string fileIcon;

			// UI
			ViewType viewType = ViewType::VIEWTYPE_ICON;
			float padding = 16.f;
			float iconSize = 150.f;

			// keys 
			KeyInput renameKey = {{Key::KEY_F2}};
			KeyInput removeKey = {{Key::KEY_DELETE}};
			KeyInput createFileKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_N}};
			KeyInput createDirectoryKey = {{Key::KEY_N}, {Key::KEY_LEFT_SHIFT, Key::KEY_RIGHT_SHIFT}, {Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}};
			KeyInput openKey = {{Key::KEY_ENTER}};
			KeyInput zoomInKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_KP_ADD, Key::KEY_EQUAL}};
			KeyInput zoomOutKey = {{Key::KEY_LEFT_CONTROL, Key::KEY_RIGHT_CONTROL}, {Key::KEY_KP_SUBTRACT, Key::KEY_MINUS}};
			KeyInput goToParentKey = {{Key::KEY_ESCAPE, Key::KEY_BACKSPACE}};

			// === tools ===

			// return false whene there is an error
			bool absolute(std::filesystem::path &path); // transform the given path into a absolute path

			inline static bool exist(const std::filesystem::path &path) {return std::filesystem::exists(path);}

			// get if the absolute path 'child' is in the absolute path 'parent'
			inline static bool isChildOf(const std::filesystem::path &parent, const std::filesystem::path &child){
				return child.string().find_first_of(parent.string()) != std::string::npos;
			}

			
	};
}
