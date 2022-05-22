#include "panels/ContentBrowserPanel.hpp"
#include "EditorLayer.hpp"
#include <libs/ImGui/imgui_internal.h>
#include <cstring>

namespace engine{

	static const std::list<std::string> IMAGE_EXTENTIONS = {".png", ".jpg", ".bmp", ".gif", ".jpe"};

	void toLower(std::string &str){
		for (auto &c : str){
			c = std::tolower(c);
		}
	}

	static bool isImage(const std::filesystem::path &path){
		std::string ext = path.extension().string();
		toLower(ext);
		
		for (auto &e : IMAGE_EXTENTIONS){
			if (ext == e) return true;
		}
		return false;
	}

	Ref<ContentBrowserPanel> ContentBrowserPanel::create(){
		return createRef<ContentBrowserPanel>();
	}

	ContentBrowserPanel::ContentBrowserPanel(){
		fileIcon = "file";
		folderIcon = "folder";

		currentFolder = Filesystem::getDataFolderPath();
		browserRoot = Filesystem::getDataFolderPath();
	}

	void ContentBrowserPanel::OnImGuiRender(){
		ImGui::Begin("contentBrowserPanel", nullptr, ImGuiWindowFlags_MenuBar);

		// the menu bar where the basic settings belong
		if (ImGui::BeginMenuBar()){
			if (ImGui::BeginMenu("view")){
				if (ImGui::BeginMenu("view style")){
					if (ImGui::RadioButton("icon", viewType == ViewType::VIEWTYPE_ICON)) viewType = ViewType::VIEWTYPE_ICON;
					if (ImGui::RadioButton("hierarchy", viewType == ViewType::VIEWTYPE_HIERARCHY)) viewType= ViewType::VIEWTYPE_HIERARCHY;

					if (ImGui::IsItemHovered()){
						ImGui::BeginTooltip();
						ImGui::TextColored({1, 0, 0, 1}, "/!\\ not available now /!\\");
						ImGui::EndTooltip();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Zoom In", zoomInKey.toString().c_str())) zoomIn();
				if (ImGui::MenuItem("Zoom Out", zoomOutKey.toString().c_str())) zoomOut();

				if (ImGui::MenuItem("Create File", createFileKey.toString().c_str())) createFile(currentFolder);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// content drawing

		switch (viewType){
			case ViewType::VIEWTYPE_ICON: drawIconsView(); break;
		}

		ImGui::End();
	}

	void ContentBrowserPanel::OnEvent(EditorEvent &e){
		EditorEventDispatcher dispatcher(e);

		dispatcher.dispatch<ContentBrowserNewSelectionEvent>(ENGINE_BIND_EVENT_FN(OnNewSelection));
		dispatcher.dispatch<ContentBrowserFileOpenedEvent>(ENGINE_BIND_EVENT_FN(OnFileOpened));
		dispatcher.dispatch<ContentBrowserDirectoryOpenedEvent>(ENGINE_BIND_EVENT_FN(OnDirectoryOpened));
		dispatcher.dispatch<EngineEvent>(ENGINE_BIND_EVENT_FN(OnEngineEvent));
	}

	void ContentBrowserPanel::serialize(YAML::Emitter &out){
		out << YAML::Key << "ViewType" << YAML::Value << static_cast<int>(viewType);
		out << YAML::Key << "Padding" << YAML::Value << padding;
		out << YAML::Key << "IconSize" << YAML::Value << iconSize;
		
		out << YAML::Key << "RenameKey" << YAML::Value << renameKey;
		out << YAML::Key << "RemoveKey" << YAML::Value << removeKey;
		out << YAML::Key << "CreateFileKey" << YAML::Value << createFileKey;
		out << YAML::Key << "CreateDirectoryKey" << YAML::Value << createDirectoryKey;
		out << YAML::Key << "OpenKey" << YAML::Value << openKey;
		out << YAML::Key << "ZoomInKey" << YAML::Value << zoomInKey;
		out << YAML::Key << "ZoomOutKey" << YAML::Value << zoomOutKey;
		out << YAML::Key << "GoToParentKey" << YAML::Value << goToParentKey;

		out << YAML::Key << "FileIcon" << YAML::Value << fileIcon;
		out << YAML::Key << "FolderIcon" << YAML::Value << folderIcon;
	}

	void ContentBrowserPanel::deserialize(YAML::Node data){
		viewType = static_cast<ViewType>(data["ViewType"].as<int>());
		padding = data["Padding"].as<float>();
		iconSize = data["IconSize"].as<float>();

		renameKey = data["RenameKey"].as<KeyInput>();
		removeKey = data["RemoveKey"].as<KeyInput>();
		createFileKey = data["CreateFileKey"].as<KeyInput>();
		createDirectoryKey = data["CreateDirectoryKey"].as<KeyInput>();
		openKey = data["OpenKey"].as<KeyInput>();
		zoomInKey = data["ZoomInKey"].as<KeyInput>();
		zoomOutKey = data["ZoomOutKey"].as<KeyInput>();
		goToParentKey = data["GoToParentKey"].as<KeyInput>();

		fileIcon = data["FileIcon"].as<std::string>();
		folderIcon = data["FolderIcon"].as<std::string>();
	}

	// ========================================================================= imgui

	void ContentBrowserPanel::drawIconsView(){
		
		const static ImGuiInputTextFlags pathInputFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly;

		if (currentFolder != browserRoot){
			// back to parent button
			if (ImGui::Button("<-")){
				openDirectory(currentFolder.parent_path());
			}
			ImGui::SameLine();
		}

		std::filesystem::path relativePath = std::filesystem::relative(currentFolder, browserRoot);

		ImGui::InputText("##path", relativePath.string().data(), relativePath.string().size(), pathInputFlags);

		// calculate the count of column available for the current item with
		float cellSize = padding + iconSize;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = static_cast<int>(panelWidth / cellSize);

		if (columnCount < 1){
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});

		for (auto &entry : std::filesystem::directory_iterator(currentFolder)){
			ImGui::PushID(entry.path().string().c_str());

			glm::vec4 UVs;
			if (entry.is_directory()){
				UVs = editor->getIcon(folderIcon);
			} else {
				UVs = editor->getIcon(fileIcon);
			}

			ImVec2 pos = ImGui::GetCursorPos();
			ImGuiID id;

			ImVec4 bgCol = {0, 0, 0, 0};

			bool isSelected = selection == entry.path();

			if (isSelected){
				bgCol = {1, 1, 1, 0.2};
			}

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()) , {iconSize, iconSize}, {UVs.x, UVs.y}, {UVs.z, UVs.w}, -1, bgCol)){
				if (isSelected){
					if (entry.is_directory()){
						openDirectory(entry.path());
					} else {
						// openFile(entry.path());
					}
				} else {
					select(entry.path());
				}
			}

			if (ImGui::BeginDragDropSource()){
				
				std::string path;
				path.clear();
				path = entry.path().string();
			
				if (isImage(entry.path())){
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM_IMAGE", path.data(), path.size());
				} else {
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", path.data(), path.size());
				}
				
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {75, 75}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
				
				ImGui::EndDragDropSource();
			}

			// select if the item is right clicked
			if (ImGui::IsItemClicked(1)){
				select(entry.path());
			}

			if (ImGui::BeginPopupContextItem()){
				if (entry.is_directory()){
					directoryContextItem(entry.path());
				} else {
					fileContextItem(entry.path());
				}
				ImGui::EndPopup();
			}

			if (renaming && isSelected){
				if (ImGui::InputText("##filename", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)){
					renaming = false;

					std::filesystem::path to = entry.path();
					to.replace_filename(renameBuffer);

					if (!exist(to)){
						std::filesystem::rename(entry.path(), to);
						ContentBrowserRenamedEvent event(to, renameBuffer);
						callback(event);
					}
				}

			} else {
				ImGui::TextWrapped(entry.path().filename().string().c_str());
			}
			ImGui::PopID();
			ImGui::NextColumn();
		}

		ImGui::PopStyleColor();
		ImGui::Columns(1);

		isMouseHover = false;
		if (ImGui::IsWindowHovered()){
			if (ImGui::IsMouseDown(0) && !ImGui::IsAnyItemHovered()){
				select();

			} else if (ImGui::IsMouseDown(1) && !ImGui::IsAnyItemHovered()){
				ImGui::OpenPopup("ContentBrowserPanelVoidContextItemPopup");
			}

			isMouseHover = true;
		}

		if (ImGui::BeginPopup("ContentBrowserPanelVoidContextItemPopup")){
			voidContextItem();
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::fileContextItem(const std::filesystem::path &path){

		if (ImGui::MenuItem("rename", renameKey.toString().c_str())) renameSelection();

		if (ImGui::MenuItem("remove", removeKey.toString().c_str())){
			deleteSelection();
			return;
		}

		// used for extentions
		ContentBrowserFileOptionPopupOpenedEvent event(path);
		callback(event);
	}

	void ContentBrowserPanel::directoryContextItem(const std::filesystem::path &path){
		
		if (ImGui::MenuItem("open", openKey.toString().c_str())) openDirectory(path);
		if (ImGui::MenuItem("rename", renameKey.toString().c_str())) renameSelection();

		if (ImGui::MenuItem("remove", removeKey.toString().c_str())){
			deleteSelection();
		}

		ContentBrowserDirectoryOptionPopupOpenedEvent event(path);
		callback(event);
	}

	void ContentBrowserPanel::voidContextItem(){
		if (ImGui::MenuItem("Create File", createFileKey.toString().c_str())) createFile(currentFolder);
		if (ImGui::MenuItem("Create Folder", createDirectoryKey.toString().c_str())) createDirectory();

		ContentBrowserVoidOptionPopupOpenedEvent event(currentFolder);
		callback(event);
	}

	// ========================================================================= events

	bool ContentBrowserPanel::OnNewSelection(ContentBrowserNewSelectionEvent e){
		renaming = false;
		std::filesystem::path path = e.getPath();

		if (!path.empty()){
			if (!absolute(path)) return true;
		}

		selection = path;
		return false;
	}

	bool ContentBrowserPanel::OnFileOpened(ContentBrowserFileOpenedEvent e){
		renaming = false;
		return false;
	}

	bool ContentBrowserPanel::OnDirectoryOpened(ContentBrowserDirectoryOpenedEvent e){
		renaming = false;
		if (e.getPath().is_relative()){
			// if the path is relative, we expect it to be relative to the '.data/' folder

			std::filesystem::path fullPath = Filesystem::getDataFolderPath() / e.getPath();
			if (!exist(fullPath)){
				ENGINE_WARN("invalid path \"", fullPath, "\"");
				return true; // the event get stoped if the path is not valid
			}

			currentFolder = fullPath;
			return false;
		}

		if (!isChildOf(Filesystem::getDataFolderPath(), e.getPath())){
			ENGINE_WARN("the given path is outside the engine folder : \"", e.getPath(), "\"");
			return true; // the event get stoped if the path is not valid
		}

		currentFolder = e.getPath();
		return false;
	}

	bool ContentBrowserPanel::OnCreated(ContentBrowserCreatedEvent e){
		renaming = false;
		if (e.isDirectory()){

		} else {
			std::filesystem::path path = e.getPath().parent_path();

			if (!absolute(path)){
				ENGINE_WARN("invalide path : \"", path, "\"");
				return true; // stop the event
			}

			path /= e.getPath().filename();

			if (!exist(path)){
				createFile(path);
				select(path);
				renameSelection();
			}
		}
		return false;
	}

	bool ContentBrowserPanel::OnEngineEvent(EngineEvent e){
		EventDispatcher dispatcher(e.getEvent());

		dispatcher.dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
		return false;
	}
	
	bool ContentBrowserPanel::OnKeyPressed(KeyPressedEvent e){

		if (isMouseHover){
			if (renameKey.OnKeyPressed(e, inputs.get())) renameSelection();
			if (removeKey.OnKeyPressed(e, inputs.get())) deleteSelection();

			if (createDirectoryKey.OnKeyPressed(e, inputs.get())) createDirectory();
			else if (createFileKey.OnKeyPressed(e, inputs.get())) createFile(currentFolder);

			if (openKey.OnKeyPressed(e, inputs.get())) openSelsection();
			if (zoomInKey.OnKeyPressed(e, inputs.get())) zoomIn();
			if (zoomOutKey.OnKeyPressed(e, inputs.get())) zoomOut();
			if (goToParentKey.OnKeyPressed(e, inputs.get())) goToParent();
		}
		
		return false;
	}


	// =================================================================== methodes


	void ContentBrowserPanel::select(std::filesystem::path path){
		ContentBrowserNewSelectionEvent event(path);
		callback(event);
	}

	void ContentBrowserPanel::createFile(const std::filesystem::path &path){
		std::filesystem::directory_entry entry(path);

		renaming = false;
		if (path.has_filename() && !entry.is_directory()){
			createFile(path.parent_path(), path.filename().string(), false);
		} else {
			createFile(path, "new File.txt");
		}
	}

	void ContentBrowserPanel::createFile(const std::filesystem::path &parentPath, const std::string &filename, bool rename){
		static int nameRetry = 255; // the number of times the programe will try to create a file if there is one with the same name

		std::filesystem::path parent = parentPath; // copy the parent path, because it's const

		if (!absolute(parent)){
			ENGINE_WARN("invalide path ", parentPath);
			return;
		}
		
		// if there is a file with the same name, it will try to create a file with a number at the end
		// file.txt
		// >> file(2).txt
		
		std::filesystem::path filenamePath = filename;
		std::string filenameStr = filenamePath.filename().replace_extension().string();
		std::string fileExt = filenamePath.extension().string();

		std::string name = filenameStr + fileExt;
		bool failed = true;

		for (int i=0; i<nameRetry; i++){
			if (i!=0){
				name = filenameStr + '(' + std::to_string(i+1) + ")" + fileExt;
			}

			if (!exist(parent / name)){
				failed = false;
				break;
			}
		}
		
		if (failed) return; 

		std::filesystem::path fullpath = parent / name;
		std::ofstream file(fullpath);

		if (!file.is_open()){
			ENGINE_WARN("failed to create ", fullpath);
			return;
		}

		file.close();

		select(fullpath);
		if (rename) renameSelection();
	}

	bool ContentBrowserPanel::createDirectory(std::filesystem::path path, bool rename){
		renaming = false;
		static int nameRetry = 255;

		if (!isChildOf(Filesystem::getDataFolderPath(), path.parent_path())){
			ENGINE_WARN("path out of range ", path);
			return false;
		}

		bool failed = true;
		std::string finalPath = path.string();

		for (int i=0; i<nameRetry; i++){
			if (i != 0){
				finalPath = path.string() + "(" + std::to_string(i+1) + ")";
			}

			if (!exist(finalPath)){
				failed = false;
				break;
			}
		}

		if (failed){
			ENGINE_ERROR("failed to create ", finalPath);
			return false;
		}

		std::error_code error;
		if (!std::filesystem::create_directories(finalPath, error)){
			ENGINE_ERROR("failed to create ", finalPath, " | reason : ", error.message());
			return false;
		}

		ContentBrowserCreatedEvent event(finalPath, true, std::filesystem::path(finalPath).filename().string());
		callback(event);

		select(finalPath);
		if (rename) renameSelection();

		return true;
	}

	void ContentBrowserPanel::createDirectory(std::filesystem::path path, const std::string &filename, bool rename){
		if (!absolute(path)){
			ENGINE_WARN("invalid path ", path);
			return;
		}

		path /= filename;
		if (!createDirectory(path, rename)) return;
	}

	void ContentBrowserPanel::createDirectory(){
		createDirectory(currentFolder, "New Folder", true);
	}

	void ContentBrowserPanel::openSelsection(){
		if (!absolute(selection)) return;
		std::filesystem::directory_entry entry(selection);

		if (entry.is_directory()){
			openDirectory(selection);
		}
	}


	void ContentBrowserPanel::openDirectory(std::filesystem::path path){
		if (!absolute(path)) return;

		ContentBrowserDirectoryOpenedEvent event(path);
		callback(event);
	}

	void ContentBrowserPanel::renameSelection(){
		if (!exist(selection)) return;

		// clear the buffer
		memset(renameBuffer, 0, sizeof(renameBuffer));

		// set the buffer to the current selection name and avoid errors
		size_t charToCopy = std::min(selection.filename().string().size(), sizeof(renameBuffer));
		selection.filename().string().copy(renameBuffer, charToCopy);

		renaming = true;
	}

	void ContentBrowserPanel::deleteSelection(){
		if (!exist(selection) || selection.empty() || selection == Filesystem::getDataFolderPath()) return;

		// send the event before deleting the file to avoid other panels / extentions to open a deleted file
		// others programe shouldn't delete the file themself but there is still a security in case of

		ContentBrowserRemovedEvent event(selection);
		callback(event);

		// and then delete
		if (exist(selection)) std::filesystem::remove(selection);
	}


	void ContentBrowserPanel::zoomIn(){
		iconSize += 10;
	}

	void ContentBrowserPanel::zoomOut(){
		iconSize -= 10;
		iconSize = std::max(iconSize, 15.f);
	}

	void ContentBrowserPanel::goToParent(){
		if (currentFolder != Filesystem::getDataFolderPath()){
			currentFolder = currentFolder.parent_path();
		}
	}


	// =================================================================== tools

	bool ContentBrowserPanel::absolute(std::filesystem::path &path){

		// if the path is already absolute, we check if it in the .data folder
		if (path.is_absolute()){
			return isChildOf(Filesystem::getDataFolderPath(), path);
		}

		std::filesystem::path fullPath = Filesystem::getDataFolderPath() / path;
		if (!exist(fullPath)) return false;

		path = fullPath;
		return true;
	}

}