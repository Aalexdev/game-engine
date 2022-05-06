#include "EditorLayer.hpp"
#include "widgets/Widgets.hpp"

#include <libs/glm/gtx/matrix_decompose.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <libs/ImGui/imgui_internal.h>
#include <libs/ImGuiFileDialog/ImGuiFileDialog.h>

namespace engine{

	// ============================================== public

	void EditorLayer::OnAttach(){
		FramebufferLayout layout;
		
		layout.width = 1080;
		layout.height = 720;
		layout.attachments = {{FramebufferTextureFormat::RGBA8, FramebufferTextureFiltering::NEAREST, FramebufferTextureFiltering::NEAREST}, FramebufferTextureFormat::R32UI};

		framebuffer = Framebuffer::create(layout);

		renderer->setClearColor({0.2f, 0.2f, 0.2f, 1.f});

		editorScene = Scene::create(renderer);
		activeScene = editorScene;

		camera = EditorCamera::create(input);
		
		createEntity("camera").addComponent<components::Camera>();
		createEntity().addComponent<components::Sprite>();

		serializer = SceneSerializer::create(editorScene, app->getTextures());

		panels.init(ENGINE_BIND_EVENT_FN(OnEditorEvent), input, this);

		editorSerializer = createRef<EditorSerializer>(this);
		editorSerializer->deserialize((Filesystem::getDataFolderPath() / "config.yaml").string());
	}

	void EditorLayer::OnDetach(){
		editorSerializer->serialize((Filesystem::getDataFolderPath() / "config.yaml").string());
	}

	void EditorLayer::OnUpdate(Timestep timestep){
		delta = timestep;
		camera->OnUpdate(timestep);

		if (viewportSize.x > 0.f && viewportSize.y > 0.f && (static_cast<uint32_t>(viewportSize.x) != framebuffer->getWidth() || static_cast<uint32_t>(viewportSize.y) != framebuffer->getHeight())){
			
			const uint32_t viewportWidth = static_cast<uint32_t>(viewportSize.x);
			const uint32_t viewportHeight = static_cast<uint32_t>(viewportSize.y);

			framebuffer->resize(viewportWidth, viewportHeight);
			camera->OnViewportResized(viewportWidth, viewportHeight);
			activeScene->setViewportSize(viewportWidth, viewportHeight);
		}
		

		framebuffer->bind();
		renderer->clear();

		// uint32_t entityAttachemntClearValue = 0;
		// framebuffer->clear(1, &entityAttachemntClearValue);

		if (runtime){
			activeScene->OnUpdateRuntime(timestep);
		} else {
			activeScene->OnUpdateEditor(*camera.get());
		}
		

		if (input->isKeyDown(Key::KEY_F10)){
			ENGINE_WARN("start profil record");
		}
		
		framebuffer->unBind();

	}

	void EditorLayer::OnImGuiRender(){
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen){
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		} else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode){
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Dockspace", nullptr, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle &style = ImGui::GetStyle();
		style.WindowMinSize.x = 303.f;
		
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		drawMenuBar();
		
		style.WindowMinSize.x = 32.f;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		drawViewport();
		panels.OnImGuiRender();

		ImGui::PopStyleVar();
		ImGui::End();

		if (startupPopup){
			ImGui::OpenPopup("startup");
			startupPopup = false;
			freezeInput = true;
		}
		
		drawStartupMenu();
		drawFileDialogs();
	}

	//  ======================================================= Events

	void EditorLayer::OnEvent(Event &event){
		EngineEvent e(event);
		panels.OnEvent(e);

		if (!freezeInput){
			EventDispatcher dispatcher(event);
			dispatcher.dispatch<MouseMovedEvent>(ENGINE_BIND_EVENT_FN(OnMouseMoved));
			dispatcher.dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
			dispatcher.dispatch<MouseButtonPressedEvent>(ENGINE_BIND_EVENT_FN(OnMouseButtonPressed));

			if (mouseOnViewport) camera->OnEvent(event);
		}
	}

	bool EditorLayer::OnMouseMoved(MouseMovedEvent &e){
		
		// check if the mouse is hovering teh viewport
		{
			auto [mx, my] = ImGui::GetMousePos();
			mx -= viewportBounds[0].x;
			my -= viewportBounds[0].y;

			float viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
			float viewportHeight = viewportBounds[1].y - viewportBounds[0].y;

			int mouseX = static_cast<int>(mx);
			int mouseY = static_cast<int>(my);

			mouseOnViewport = (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportWidth) && mouseY < static_cast<int>(viewportHeight));
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent &e){
		switch (e.getMouseButton()){
			case ENGINE_MOUSE_BUTTON_LEFT:{
				if (e.isDoubleClicked()){
					framebuffer->bind();
					auto [mx, my] = ImGui::GetMousePos();
					mx -= viewportBounds[0].x;
					my -= viewportBounds[0].y;

					float viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
					float viewportHeight = viewportBounds[1].y - viewportBounds[0].y;

					int mouseX = static_cast<int>(mx);
					int mouseY = static_cast<int>(my);

					if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportWidth) && mouseY < static_cast<int>(viewportHeight)){
						
						uint32_t entityIndex;
						framebuffer->getPixel(1, static_cast<uint32_t>(mouseX), static_cast<uint32_t>(mouseY), &entityIndex);

						// substract one because the list is moved by one value, so 0 is never used and can be used as a non entity value

						if (entityIndex != 0){
							selectEntity({--entityIndex, activeScene.get()});
						} else {
							selectEntity({}); // clear the selected entity
						}
					}
					framebuffer->unBind();
					return true;
				}
			}
		}
		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent &e){
		
		if (input->isDown(saveProjectAsKey, e)) saveAs();
		else if (input->isDown(saveProjectKey, e)) save();
		if (input->isDown(openProjectKey, e)) open();
		if (input->isDown(exitKey, e)) app->close();

		if (input->isDown(rotateGizmoKey, e)) guizmoRotate();
		if (input->isDown(scaleGizmoKey, e)) guizmoScale();
		if (input->isDown(translateGizmoKey, e)) guizmoTranslate();
		if (input->isDown(newEntityKey, e)) createEntity();
		if (input->isDown(deleteSelectedEntityKey, e)) deleteSelectedEntity();

		if (e.getKeyCode() == Key::KEY_F10){
			ENGINE_WARN("PROFILE NEXT 5 FRAMES");
			ENGINE_PROFILE_RECORD_FRAMES(5);
		}

		return false;
	}

	bool EditorLayer::OnEntityRemoved(EntityRemovedEvent &e){
		if (e.getEntity() == selectedEntity){
			selectedEntity.release();
		}

		return false;
	}


	bool EditorLayer::OnEntitySelected(EntitySelectedEvent &e){
		selectedEntity = e.getEntity();
		return false;
	}

	void EditorLayer::OnEditorEvent(EditorEvent &e){
		panels.OnEvent(e);

		EditorEventDispatcher dispatcher(e);

		dispatcher.dispatch<EntitySelectedEvent>(ENGINE_BIND_EVENT_FN(OnEntitySelected));
		dispatcher.dispatch<EntityRemovedEvent>(ENGINE_BIND_EVENT_FN(OnEntityRemoved));
		dispatcher.dispatch<RuntimePausedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimePaused));
		dispatcher.dispatch<RuntimePlayedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimePlayed));
		dispatcher.dispatch<RuntimeStartedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimeStarted));
		dispatcher.dispatch<RuntimeStoppedEvent>(ENGINE_BIND_EVENT_FN(OnRuntimeStopped));
	}
	
	bool EditorLayer::OnRuntimePaused(RuntimePausedEvent &e){
		runtime = false;
		activeScene->OnRuntimePause();
		selectedEntity.release();
		return false;
	}

	bool EditorLayer::OnRuntimePlayed(RuntimePlayedEvent &e){
		runtime = true;
		activeScene->OnRuntimePlay();
		selectedEntity.release();
		return false;
	}

	bool EditorLayer::OnRuntimeStopped(RuntimeStoppedEvent &e){
		runtime = false;
		activeScene->OnRuntimeStop();
		activeScene = editorScene;
		selectedEntity.release();

		return false;
	}

	bool EditorLayer::OnRuntimeStarted(RuntimeStartedEvent &e){
		runtime = true;
		activeScene = Scene::copy(editorScene);
		activeScene->OnRuntimeStart();
		selectedEntity.release();
		return false;
	}


	//  ======================================================================== ImGui

	void EditorLayer::drawMenuBar(){
		if (ImGui::BeginMenuBar()){
			if (ImGui::BeginMenu("File")){

				if (ImGui::MenuItem("save", saveProjectKey.toString().c_str())) save();
				if (ImGui::MenuItem("save as", saveProjectAsKey.toString().c_str())) saveAs();
				if (ImGui::MenuItem("open", openProjectKey.toString().c_str())) this->open();
				
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", exitKey.toString().c_str())) app->close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::drawViewport(){
		ImGui::Begin("Viewport", nullptr);
		ImVec2 viewportOffset = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		uint32_t texture = framebuffer->getColorAttachment(0);
		viewportSize = {size.x, size.y};

		ImGui::Image(reinterpret_cast<void*>(texture), size, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;
		ImVec2 maxBound = {minBound.x + windowSize.x, minBound.y + windowSize.y};

		viewportBounds[0] = {minBound.x, minBound.y};
		viewportBounds[1] = {maxBound.x, maxBound.y};
		
		if (!runtime) drawGuizmo();

		ImGui::End();
	}

	void EditorLayer::drawGuizmo(){

		// check if there is a selected entity if it has a transform component
		if (selectedEntity && selectedEntity.hasComponent<components::Transform>()){
			ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
			float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			const glm::mat4& projection = camera->getCamera().getProjectionMatrix();
			glm::mat4 cameraView = camera->getCamera().getViewMatrix();

			auto& entityTransform = selectedEntity.getComponent<components::Transform>();
			glm::mat4 transformMatrix = entityTransform.getTransform();


			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projection), guizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(transformMatrix));

			if (ImGuizmo::IsUsing()){
				entityTransform.translation = glm::vec3(transformMatrix[3]);

				math::decomposeTransform2D(transformMatrix, entityTransform.translation, entityTransform.scale, entityTransform.rotation);

			}
		}
	}

	void EditorLayer::drawStartupMenu(){
		if (ImGui::BeginPopupModal("startup")){
			ImGui::Text("welcome !");
			ImGui::TextColored({0.7, 0.7, 0.7, 1.0}, "Made by Aalexdev (aaleex3984@gmail.com)");

			ImGui::TextColored({0.7, 0.7, 0.7, 1.0}, "recent projects ...");

			ImVec2 size = ImGui::CalcTextSize(" ");
			size.y *= 1.25;
			size.x = 0;

			size_t i=0;
			if (ImGui::BeginListBox("##recent projects")){

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

				for (auto &project : recentProjects){
					if (project.empty()) break;

					ImGui::PushID(i);

					if (ImGui::Button(project.filename().string().c_str(), size)){
						openProject(project);
						ImGui::CloseCurrentPopup();
						freezeInput = false;
					}


					i++;
					ImGui::PopID();
				}

				ImGui::EndListBox();
				ImGui::PopStyleColor();
			}

			ImGui::Separator();

			if (ImGui::Button("create new project") || (ImGui::IsMouseDown(0) && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())){
				ImGui::CloseCurrentPopup();
				freezeInput = false;
			}			
			
			ImGui::EndPopup();
		}
	}

	void EditorLayer::drawFileDialogs(){
		drawOpenFileDialog();
		drawSaveAsFileDialog();
	}

	void EditorLayer::drawOpenFileDialog(){
		if (ImGuiFileDialog::Instance()->Display("openFile")){
			if (ImGuiFileDialog::Instance()->IsOk()){
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				openProject(filePathName);
			}
			
			ImGuiFileDialog::Instance()->Close();
		}
	}

	void EditorLayer::drawSaveAsFileDialog(){
		if (ImGuiFileDialog::Instance()->Display("saveAs")){
			if (ImGuiFileDialog::Instance()->IsOk()){
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

				// save at the location
				serializer->serializeText(filePathName);

				// change the current project path and the window's title to the file name
				currentProjectPath = filePathName;
				setProjectName(filePathName);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}

	// ============================================== Other methodes

	void EditorLayer::setProjectName(std::string name){

		// get only the filename without the path and the extention | code from OpenGlShader.cpp by the cherno
		size_t lastSlash = name.find_last_of("/\\");

		if (lastSlash == std::string::npos){
			lastSlash = 0;
		} else {
			lastSlash++;
		}

		size_t lastDot = name.find_last_of(".");

		if (lastDot == std::string::npos)
			lastDot = name.size();

		std::string title = "engine | " + name.substr(lastSlash, lastDot - lastSlash);

		// set the window title to the filname
		app->getDisplay().setTitle(title);
	}

	void EditorLayer::save(){
		// save the current project, if the project dosen't have a path, it will ask for
		if (currentProjectPath.empty())
			saveAs();
		else
			serializer->serializeText(currentProjectPath);
	}

	void EditorLayer::saveAs(){
		ImGuiFileDialog::Instance()->OpenDialog("saveAs", "Choose File", ".yaml", ".");
	}

	void EditorLayer::open(){
		ImGuiFileDialog::Instance()->OpenDialog("openFile", "Choose File", ".yaml", ".");
	}

	

	Entity EditorLayer::createEntity(const std::string &tag){
		// create the actual entity
		Entity entity;

		if (tag.empty()){
			entity = editorScene->createIDEntity();
		} else  {
			entity = editorScene->createIDEntity(tag);
		}

		// send the event
		EntityCreatedEvent event(entity);
		OnEditorEvent(event);

		selectEntity(entity);

		return entity;
	}

	void EditorLayer::selectEntity(Entity entity){
		EntitySelectedEvent event(entity);
		OnEditorEvent(event);
	}

	void EditorLayer::openProject(const std::filesystem::path &path){
		activeScene->OnRuntimeStop();
		runtime = false;

		activeScene->clear();
		serializer->deserializeText(path.string());

		currentProjectPath = path.string();
		setProjectName(path.filename().string());
		pushRecentProject(path);
	}

	void EditorLayer::deleteSelectedEntity(){
		if (!selectedEntity) return;
		activeScene->destroyEntity(selectedEntity);
	}

	void EditorLayer::pushRecentProject(const std::filesystem::path &path){
		for (size_t i=recentProjects.size()-1; i>0; i--){
			recentProjects[i] = recentProjects[i-1];
		}

		recentProjects[0] = path;
	}
}
