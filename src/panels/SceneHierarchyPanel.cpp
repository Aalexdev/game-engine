#include "panels/SceneHierarchyPanel.hpp"
#include "EditorLayer.hpp"
#include "widgets/Widgets.hpp"

#include <libs/glm/gtc/type_ptr.hpp>
#include <libs/ImGui/imgui_internal.h>

namespace engine{

	Ref<SceneHierarchyPanel> SceneHierarchyPanel::create(){
		return createRef<SceneHierarchyPanel>();
	}

	SceneHierarchyPanel::SceneHierarchyPanel(){
		cameraIcon = "camera";
		entityIcon = "cube";
		// lightIcon = "light bulb";
		downArrowIcon = "down arrow";
		rightArrowIcon = "right arrow";
	}


	void SceneHierarchyPanel::OnImGuiRender(){
		currentHoveredPanel = SceneHierarchyPanelType::PANEL_NONE;

		if (ImGui::Begin("activeScene hierarchy")){
			
			ImGui::Spacing();
			for (const auto e : editor->activeScene->getRegistry()){
				Entity entity = {e, editor->activeScene.get()};
				if (entity.hasParent()) continue;
				drawEntityNode(entity);
			}

			ImVec2 size = ImGui::GetContentRegionAvail();
			size.x -= ImGui::GetCursorPosX() + 10;
			size.y -= ImGui::GetCursorPosY();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
			ImGui::InvisibleButton("rest", size);

			if (ImGui::BeginDragDropTarget()){
				if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
					Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
					entity.resetParent();
				}
				ImGui::EndDragDropTarget();
			}
			
			if (ImGui::IsWindowHovered()){
				currentHoveredPanel = SceneHierarchyPanelType::PANEL_SCENE_HIERARCHY;
			}

			if (currentHoveredPanel == SceneHierarchyPanelType::PANEL_SCENE_HIERARCHY){
				if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()){
					select({});
				}
			}

			if (ImGui::BeginPopupContextItem("SceneHierarchyPopupContextVoid")){
					
				if (ImGui::BeginMenu("create entity")){
					if (ImGui::MenuItem("create empty entity", newEntityKey.toString().c_str())){
						createNewEntity();
					}

					if (ImGui::BeginMenu("colliders")){
						if (ImGui::MenuItem("box")){
							auto entity = createNewEntity();
							{
								auto &component = entity.addComponent<ECS::components::RigidBody>();
								ComponentAddedEvent event(entity, component);
								callback(event);
							}

							auto &component = entity.addComponent<ECS::components::BoxCollider>();
							ComponentAddedEvent event(entity, component);
							callback(event);
						}

						if (ImGui::MenuItem("circle")){
							auto entity = createNewEntity();
							{
								auto &component = entity.addComponent<ECS::components::RigidBody>();
								ComponentAddedEvent event(entity, component);
								callback(event);
							}

							auto &component = entity.addComponent<ECS::components::CircleCollider>();
							ComponentAddedEvent event(entity, component);
							callback(event);
						}
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("renderer")){
						if (ImGui::MenuItem("sprite")){
							auto entity = createNewEntity();
							auto &component = entity.addComponent<ECS::components::Sprite>();
							ComponentAddedEvent event(entity, component);
							callback(event);
						}

						if (ImGui::MenuItem("circle")){
							auto entity = createNewEntity();
							auto &component = entity.addComponent<ECS::components::CircleRenderer>();
							ComponentAddedEvent event(entity, component);
							callback(event);
						}

						if (ImGui::MenuItem("triangle")){
							auto entity = createNewEntity();
							auto &component = entity.addComponent<ECS::components::TriangleRenderer>();
							ComponentAddedEvent event(entity, component);
							callback(event);
						}

						ImGui::EndMenu();
					}
					
					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		drawComponentsPanel();
	}

	void SceneHierarchyPanel::serialize(YAML::Emitter &out){
		out << YAML::Key << "NewEntityKey" << YAML::Value << newEntityKey;
		out << YAML::Key << "DeleteEntityKey" << YAML::Value << deleteEntityKey;

		out << YAML::Key << "CameraIcon" << YAML::Value << cameraIcon;
		out << YAML::Key << "EntityIcon" << YAML::Value << entityIcon;
		out << YAML::Key << "RightArrowIcon" << YAML::Value << rightArrowIcon;
		out << YAML::Key << "DownArrowIcon" << YAML::Value << downArrowIcon;
	}

	void SceneHierarchyPanel::deserialize(YAML::Node data){
		newEntityKey = data["NewEntityKey"].as<KeyInput>();
		deleteEntityKey = data["DeleteEntityKey"].as<KeyInput>();

		cameraIcon = data["CameraIcon"].as<std::string>();
		entityIcon = data["EntityIcon"].as<std::string>();
		rightArrowIcon = data["RightArrowIcon"].as<std::string>();
		downArrowIcon = data["DownArrowIcon"].as<std::string>();
	}

	// ========================================================= Events

	void SceneHierarchyPanel::OnEvent(EditorEvent &e){
		EditorEventDispatcher dispatcher(e);

		dispatcher.dispatch<EntitySelectedEvent>(ENGINE_BIND_EVENT_FN(OnEntitySelected));
		dispatcher.dispatch<EngineEvent>(ENGINE_BIND_EVENT_FN(OnEngineEvent));
	}

	bool SceneHierarchyPanel::OnEntitySelected(EntitySelectedEvent &e){
		selectedEntity = e.getEntity();
		return false;
	}

	bool SceneHierarchyPanel::OnEngineEvent(EngineEvent &e){
		EventDispatcher dispatcher(e.getEvent());

		dispatcher.dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(OnKeyPressed));
		return false;
	}

	bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent &e){
		switch (currentHoveredPanel){
			case SceneHierarchyPanelType::PANEL_ENTITY_COMPONENTS:
				return KeyPressedOnComponentPanel(e);

			case SceneHierarchyPanelType::PANEL_SCENE_HIERARCHY:
				return KeyPressedOnHierarchyPanel(e);
		}
		return false;
	}
	
	bool SceneHierarchyPanel::KeyPressedOnComponentPanel(KeyPressedEvent &e){
		
		return false;
	}

	bool SceneHierarchyPanel::KeyPressedOnHierarchyPanel(KeyPressedEvent &e){
		
		if (inputs->isDown(newEntityKey, e)) createNewEntity();
		if (inputs->isDown(deleteEntityKey, e) && currentHoveredPanel == SceneHierarchyPanelType::PANEL_SCENE_HIERARCHY) deleteSelectedEntity();

		return false;
	}

	// ========================================================= ImGui

	/**
	 * @brief draw the given compoent of the given entity, only if the entiy have the component, it will call the given uiFuction and send callbacks if the component get deleted
	 * 
	 * @tparam T the component type
	 * @tparam UIFunction teh fucntion that draw the component parameters
	 * @param sceneHierarchy the pointer to the activeScene hierachy (should be 'this')
	 * @param name the nameof the component, leave blank for auto naming
	 * @param entity the entiy to retrive the component from
	 * @param uiFunction the function that draw the component parameters
	 * @param callback the callback for the events
	 */
	template<typename T, typename UIFunction>
	static void drawComponent(SceneHierarchyPanel *sceneHierarchy, std::string name, Entity entity, UIFunction uiFunction, EditorEventCallbackFn callback){
		if (entity.hasComponent<T>()){
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

			if (name.empty()){
				name = typeid(T).name();
			}

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding, name.c_str());

			ImGui::PopStyleVar();

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))){
				ImGui::OpenPopup("componenSettings");
			}


			bool removeComponent = false;
			if (ImGui::BeginPopup("componenSettings")){
				if (ImGui::MenuItem("remove")){
					ComponentRemovedEvent event(entity, entity.getComponent<T>());
					callback(event);
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			
			if (open){
				(sceneHierarchy->*uiFunction)(entity);
				ImGui::TreePop();
			}

			if (removeComponent){
				entity.removeComponent<T>();
			}
		}
	}

	template<typename T>
	static void addComponent(const std::string& name, Entity entity, EditorEventCallbackFn callback){
		if (!entity.hasComponent<T>()){
			if (ImGui::MenuItem(name.c_str())){
				T& component = entity.addComponent<T>();
				ComponentAddedEvent event(entity, component);
				callback(event);
			}
		}
	}

	enum class EntityType{
		OBJECT,
		CAMERA,
		LIGHT,
	};

	bool SceneHierarchyPanel::drawEntityTreeNode(Entity entity){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		ImGui::PushID(entity.getUUID());
		const char* label = entity.getTag().c_str();
		ImU32 id = ImGui::GetID(label);
		int opened = storage->GetInt(id, 0);
		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::BeginGroup();

		// ImGui::ButtonBehavior()
		float width = ImGui::GetContentRegionAvail().x;
		if (ImGui::InvisibleButton(label, ImVec2(width - 10, ImGui::GetFontSize()+style.FramePadding.y*2))){
			int* p_opened = storage->GetIntRef(id, 0);
			opened = *p_opened = !*p_opened;
			select(entity);
		}

		bool removed = false;
		if (ImGui::BeginPopupContextItem()){

			if (ImGui::MenuItem("select")) select(entity);
			if (ImGui::MenuItem("remove")) removed = true;

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropTarget()){
			if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
				Entity child = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
				entity.pushChild(child);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()){
			uint32_t data = entity;
			ImGui::SetDragDropPayload("SCENE_HIERARCHY_ENTITY", &data, sizeof(data));
			ImGui::EndDragDropSource();
		}

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		if (hovered || active || entity == selectedEntity)
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));


		// Icon, text
		// ImGui::SameLine(x);
		// ImGui::Text(" ");
		// ImGui::SameLine();
		ImGui::SetCursorPos(pos);

		EntityType type = EntityType::OBJECT;
		if (entity.hasComponent<ECS::components::Camera>()){
			type = EntityType::CAMERA;
		}

		glm::vec4 UVs;
		switch (type){
			case EntityType::OBJECT: UVs = editor->getIcon(entityIcon); break;
			case EntityType::CAMERA: UVs = editor->getIcon(cameraIcon); break;
		}

		float iconSize = ImGui::GetFontSize()*1.2;
		ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize, iconSize}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
		ImGui::SameLine();
		ImGui::Text(label);

		// width = ImGui::GetContentRegionMax().x;
		ImGui::SameLine(width - iconSize * 2.2);
		
		if (opened){
			UVs = editor->getIcon(downArrowIcon);
			ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
		} else {
			UVs = editor->getIcon(rightArrowIcon);
			ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
		}

		ImGui::EndGroup();
		ImGui::PopID();

		if (removed){
			deleteEntity(entity);
			return false;
		}

		if (opened)
			ImGui::TreePush(label);
		return opened != 0;
	};

	void SceneHierarchyPanel::drawEntityNode(Entity entity){
			
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
		if (drawEntityTreeNode(entity)){
			for (auto child : entity){
				drawEntityNode(child);
			}
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponentsPanel(){
		if (ImGui::Begin("components")){
			if (selectedEntity) drawEntityComponents(selectedEntity);

			if (ImGui::IsWindowHovered()){
				currentHoveredPanel = SceneHierarchyPanelType::PANEL_ENTITY_COMPONENTS;
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity entity){

		ImGui::Text("ID : 0x%x", entity.getUUID().get());
		{
			auto &tag = entity.getTag();
			static char buffer[255];

			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##tag", buffer, sizeof(buffer))){
				tag = std::string(buffer);

				EntityRenamedEvent event(entity);
				callback(event);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component")){
			ImGui::OpenPopup("SceneHierarchyAddComponentPopup");
		}

		if (ImGui::BeginPopup("SceneHierarchyAddComponentPopup")){
			drawAddComponentMenu(entity);
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		drawComponent<ECS::components::Transform>(this, "transform", entity, drawTransformComponent, callback);
		drawComponent<ECS::components::Sprite>(this, "sprite renderer", entity, drawSpriteComponent, callback);
		drawComponent<ECS::components::CircleRenderer>(this, "circle renderer", entity, drawCircleRendererComponent, callback);
		drawComponent<ECS::components::TriangleRenderer>(this, "triangle renderer", entity, drawTriangleRendererComponent, callback);
		drawComponent<ECS::components::Camera>(this, "camera", entity, drawCameraComponent, callback);
		drawComponent<ECS::components::RigidBody>(this, "rigid Body", entity, drawRigidBodyComponent, callback);
		drawComponent<ECS::components::BoxCollider>(this, "box Bollider", entity, drawBoxColliderComponent, callback);
		drawComponent<ECS::components::CircleCollider>(this, "circle collider", entity, drawCircleColliderComponent, callback);
		drawComponent<ECS::components::DistanceJoint>(this, "distance joint", entity, drawDistanceJointComponent, callback);
		drawComponent<ECS::components::SpringJoint>(this, "spring joint", entity, drawSpringJointComponent, callback);
		drawComponent<ECS::components::RevoluteJoint>(this, "revolute joint", entity, drawRevoluteJointComponent, callback);
		drawComponent<ECS::components::PrismaticJoint>(this, "prismatic joint", entity, drawPrismaticJointComponent, callback);

	}

	void SceneHierarchyPanel::drawAddComponentMenu(Entity entity){
		addComponent<ECS::components::Camera>("camera", entity, callback);
		addComponent<ECS::components::RigidBody>("rigid Body", entity, callback);

		
		if (ImGui::BeginMenu("collider")){
			addComponent<ECS::components::BoxCollider>("box Collider", entity, callback);
			addComponent<ECS::components::CircleCollider>("circle collider", entity, callback);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("renderer")){
			addComponent<ECS::components::Sprite>("sprite renderer", entity, callback);
			addComponent<ECS::components::CircleRenderer>("circle Renderer", entity, callback);
			addComponent<ECS::components::TriangleRenderer>("triangle Renderer", entity, callback);
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("joint")){
			addComponent<ECS::components::DistanceJoint>("distance joint", entity, callback);
			addComponent<ECS::components::SpringJoint>("spring joint", entity, callback);
			addComponent<ECS::components::RevoluteJoint>("revolute joint", entity, callback);
			addComponent<ECS::components::PrismaticJoint>("prismatic joint", entity, callback);
			ImGui::EndMenu();
		}
	}

	// ==================================== components

	void SceneHierarchyPanel::drawSpriteComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::Sprite>()) return;
		auto &sprite = entity.getComponent<ECS::components::Sprite>();
		ImGui::ColorEdit4("color", glm::value_ptr(sprite.color));

		int batchGroup = static_cast<int>(sprite.batchGroup);
		if (ImGui::InputInt("batch group", &batchGroup)){
			sprite.batchGroup = static_cast<uint16_t>(batchGroup);
		}

		if (subTextureEdit(sprite.texture, editor->app->getTextures())){
			ComponentCallEvent event(entity, sprite);
		}
	}

	void SceneHierarchyPanel::drawTransformComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::Transform>()) return;
		
		auto &transform = entity.getComponent<ECS::components::Transform>().transform;

		Vec2Edit("Translation", transform.translation);
		Vec2Edit("Scale", transform.scale, {1.f, 1.f});

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text("Rotation");

		ImGui::NextColumn();

		float degRotation = glm::degrees(transform.rotation);
		if (ImGui::DragFloat("##rotation", &degRotation, 2.f)){
			transform.rotation = glm::radians(degRotation);
		}

		ImGui::Columns(1);
	}

	void SceneHierarchyPanel::drawCameraComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::Camera>()) return;

		auto &camera = entity.getComponent<ECS::components::Camera>();

		ImGui::Text("size : %dx%d", camera.viewportWidth, camera.viewportHeight);

		if (ImGui::Checkbox("fixed viewport size", &camera.fixedViewportAspectRatio)){
			camera.viewportWidth = static_cast<uint32_t>(editor->viewportSize.x);
			camera.viewportHeight = static_cast<uint32_t>(editor->viewportSize.y);
		}

		// if the camera have a fixed viewport, the user can change the width and height
		if (camera.fixedViewportAspectRatio){
			bool update = true;

			int width = static_cast<int>(camera.viewportWidth);
			update |= ImGui::InputInt("width", &width);

			int height = static_cast<int>(camera.viewportHeight);
			update |= ImGui::InputInt("height", &height);

			if (update){
				camera.viewportWidth = static_cast<uint32_t>(width);
				camera.viewportHeight = static_cast<uint32_t>(height);

				camera.camera.setViewportSize(camera.viewportWidth, camera.viewportHeight);
			}
		}

		float zoomLevel = camera.camera.getZoomLevel();
		if (ImGui::DragFloat("zoom level", &zoomLevel, 0.5f, 0.025f, 1000.f, "%.3f")){
			camera.camera.setZoomLevel(zoomLevel);
		}

		ImGui::DragFloat("Priority level", &camera.priorityLevel);
	}

	std::string rigidBodyTypeStr(ECS::components::RigidBody::BodyType type){
		switch (type){
			case ECS::components::RigidBody::BodyType::STATIC: return "Static";
			case ECS::components::RigidBody::BodyType::DYNAMIC: return "Dynamic";
			case ECS::components::RigidBody::BodyType::KINEMATIC: return "Kinematic";
		}
		ENGINE_ASSERT(false, "unknown rigid body type");
		return "None";
	}

	void SceneHierarchyPanel::drawRigidBodyComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::RigidBody>()) return;
		auto &rigidBody = entity.getComponent<ECS::components::RigidBody>();

		if (ImGui::BeginCombo("type", rigidBodyTypeStr(rigidBody.type).c_str())){
			if (ImGui::MenuItem("Static")){rigidBody.type = ECS::components::RigidBody::BodyType::STATIC;}
			if (ImGui::MenuItem("Dynamic")){rigidBody.type = ECS::components::RigidBody::BodyType::DYNAMIC;}
			if (ImGui::MenuItem("Kinematic")){rigidBody.type = ECS::components::RigidBody::BodyType::KINEMATIC;}
			ImGui::EndCombo();
		}

		ImVec2 region = ImGui::GetContentRegionAvail();


		ImGui::Checkbox("fixed rotation", &rigidBody.fixedRotation);
		ImGui::SameLine(region.x / 2);
		ImGui::Checkbox("bullet", &rigidBody.bullet);

		ImGui::Text("gravity scale");
		ImGui::SameLine();
		ImGui::DragFloat("##gravity scale", &rigidBody.gravityScale, 0.125);
	}

	void SceneHierarchyPanel::drawBoxColliderComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::BoxCollider>()) return;
		auto &collider = entity.getComponent<ECS::components::BoxCollider>();

		ImVec2 region = ImGui::GetContentRegionAvail();
		ImGui::Checkbox("scaled size", &collider.scaledSize);
		ImGui::SameLine(region.x / 2);
		ImGui::Checkbox("is sensor", &collider.isSensor);

		Vec2Edit("offset", collider.offset);
		Vec2Edit("size", collider.size, {0.5, 0.5});
		physicMaterialEdit("materials", collider.material, editor->activeScene->getPhysicMaterials());
	}

	void SceneHierarchyPanel::drawCircleRendererComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::CircleRenderer>()) return;
		auto &circle = entity.getComponent<ECS::components::CircleRenderer>();

		ImGui::ColorEdit4("color", glm::value_ptr(circle.color));
		ImGui::DragFloat("thickness", &circle.thickness, 0.01f, 0.f, 1.f, "%.2f");
		ImGui::DragFloat("fade", &circle.fade, 0.0001f, 0.f, 1.f, "%.4f");
	}

	void SceneHierarchyPanel::drawTriangleRendererComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::TriangleRenderer>()) return;
		auto &tri = entity.getComponent<ECS::components::TriangleRenderer>();

		if (ImGui::TreeNode("point 1")){
			ImGui::ColorEdit4("color", glm::value_ptr(tri.p1.color));
			Vec2Edit("local coords", tri.p1.position);
			Vec2Edit("texture coords", tri.p1.textCoord);
			ImGui::DragFloat("tiling factor", &tri.p1.tilingFactor);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("point 2")){
			ImGui::ColorEdit4("color", glm::value_ptr(tri.p2.color));
			Vec2Edit("local coords", tri.p2.position);
			Vec2Edit("texture coords", tri.p2.textCoord);
			ImGui::DragFloat("tiling factor", &tri.p2.tilingFactor);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("point 3")){
			ImGui::ColorEdit4("color", glm::value_ptr(tri.p3.color));
			Vec2Edit("local coords", tri.p3.position);
			Vec2Edit("texture coords", tri.p3.textCoord);
			ImGui::DragFloat("tiling factor", &tri.p3.tilingFactor);
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawCircleColliderComponent(Entity entity){
		if (!entity.hasComponent<ECS::components::CircleCollider>()) return;
		auto &collider = entity.getComponent<ECS::components::CircleCollider>();
		
		ImVec2 region = ImGui::GetContentRegionAvail();
		ImGui::Checkbox("scaled size", &collider.scaledSize);
		ImGui::SameLine(region.x / 2);
		ImGui::Checkbox("is sensor", &collider.isSensor);

		Vec2Edit("offset", collider.position);
		ImGui::DragFloat("radius", &collider.radius);
		physicMaterialEdit("materials", collider.material, editor->activeScene->getPhysicMaterials());
	}

	void SceneHierarchyPanel::drawDistanceJointComponent(Entity entity){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (!entity.hasComponent<ECS::components::DistanceJoint>()) return;
		auto &jointComponent = entity.getComponent<ECS::components::DistanceJoint>();

		if (ImGui::Button("add joint")){
			Joint joint;
			jointComponent.joints.push_back(joint);
		}

		if (ImGui::BeginDragDropTarget()){
			if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
				Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
				if (entity){
					Joint joint;
					joint.entityB = entity.getUUID();
					jointComponent.joints.push_back(joint);
				} else {
					ENGINE_WARN("invalid entity");
				}
			}
			ImGui::EndDragDropTarget();
		}
	
		auto &joints = jointComponent.joints;
		auto it = joints.begin();
		int i=0;
		float width = ImGui::GetContentRegionAvail().x;

		while (it != joints.end()){
			Entity joinedEntity = editor->activeScene->get(it->entityB);

			ImGui::PushID(it->entityB);
			ImU32 id = ImGui::GetID(reinterpret_cast<void*>(it->entityB.get() + i));
			int opened = storage->GetInt(id, 0);
			ImVec2 pos = ImGui::GetCursorPos();
			std::string jointName;
			
			if (it->entityB != UUID::INVALID_UUID){
				jointName = std::to_string(i) + " : " + joinedEntity.getTag();
			} else {
				jointName = std::to_string(i) + " : None";
			}
			
			if (ImGui::InvisibleButton("0", ImVec2(width - 10, ImGui::GetFontSize()+style.FramePadding.y*2))){
				int* p_opened = storage->GetIntRef(id, 0);
				opened = *p_opened = !*p_opened;
			}

			if (ImGui::BeginPopupContextItem(jointName.c_str())){
				if (opened){
					if (ImGui::MenuItem("close")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				} else {
					if (ImGui::MenuItem("open")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				}

				if (ImGui::MenuItem("remove")){
					joints.erase(it);
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}

				if (ImGui::MenuItem("select entity")){
					if (joinedEntity){
						select(joinedEntity);
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()){
				if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
					Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
					it->entityB = entity.getUUID();
				}	
				ImGui::EndDragDropTarget();
			}

			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			if (hovered || active){
				ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));
			}

			float iconSize = ImGui::GetFontSize()*1.2;
			glm::vec4 UVs;

			ImGui::SetCursorPos(pos);
			if (opened){
				UVs = editor->getIcon(downArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			} else {
				UVs = editor->getIcon(rightArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			}

			ImGui::SameLine();
			ImGui::Text(jointName.c_str());

			if (opened){
				ImGui::TreePush();

				Vec2Edit("start", it->anchorOffsetA);
				Vec2Edit("end", it->anchorOffsetB);

				ImGui::TreePop();
			}

			it++;
			i++;
			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::drawSpringJointComponent(Entity entity){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (!entity.hasComponent<ECS::components::SpringJoint>()) return;
		auto &jointComponent = entity.getComponent<ECS::components::SpringJoint>();

		if (ImGui::Button("add joint")){
			ECS::components::SpringJoint::Joint joint;
			jointComponent.joints.push_back(joint);
		}

		if (ImGui::BeginDragDropTarget()){
			if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
				Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
				if (entity){
					ECS::components::SpringJoint::Joint joint;
					joint.joinedEntity = entity.getUUID();
					jointComponent.joints.push_back(joint);
				} else {
					ENGINE_WARN("invalid entity");
				}
			}
			ImGui::EndDragDropTarget();
		}
	
		auto &joints = jointComponent.joints;
		auto it = joints.begin();
		int i=0;
		float width = ImGui::GetContentRegionAvail().x;

		while (it != joints.end()){
			Entity joinedEntity = editor->activeScene->get(it->joinedEntity);

			ImGui::PushID(it->joinedEntity);
			ImU32 id = ImGui::GetID(reinterpret_cast<void*>(it->joinedEntity.get() + i));
			int opened = storage->GetInt(id, 0);
			ImVec2 pos = ImGui::GetCursorPos();
			std::string jointName;
			
			if (it->joinedEntity != UUID::INVALID_UUID){
				jointName = std::to_string(i) + " : " + joinedEntity.getTag();
			} else {
				jointName = std::to_string(i) + " : None";
			}
			
			if (ImGui::InvisibleButton("0", ImVec2(width - 10, ImGui::GetFontSize()+style.FramePadding.y*2))){
				int* p_opened = storage->GetIntRef(id, 0);
				opened = *p_opened = !*p_opened;
			}

			if (ImGui::BeginPopupContextItem(jointName.c_str())){
				if (opened){
					if (ImGui::MenuItem("close")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				} else {
					if (ImGui::MenuItem("open")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				}

				if (ImGui::MenuItem("remove")){
					joints.erase(it);
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}

				if (ImGui::MenuItem("select entity")){
					if (joinedEntity){
						select(joinedEntity);
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()){
				if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
					Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
					it->joinedEntity = entity.getUUID();
				}	
				ImGui::EndDragDropTarget();
			}

			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			if (hovered || active){
				ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));
			}

			float iconSize = ImGui::GetFontSize()*1.2;
			glm::vec4 UVs;

			ImGui::SetCursorPos(pos);
			if (opened){
				UVs = editor->getIcon(downArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			} else {
				UVs = editor->getIcon(rightArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			}

			ImGui::SameLine();
			ImGui::Text(jointName.c_str());

			if (opened){
				ImGui::TreePush();

				Vec2Edit("start", it->anchorA);
				Vec2Edit("end", it->anchorB);

				ImGui::DragFloat("frequency", &it->frequencyHertz, 0.1, 0);
				ImGui::DragFloat("damping ratio", &it->dampingRatio, 0.05);

				ImGui::TreePop();
			}

			it++;
			i++;
			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::drawRevoluteJointComponent(Entity entity){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (!entity.hasComponent<ECS::components::RevoluteJoint>()) return;
		auto &jointComponent = entity.getComponent<ECS::components::RevoluteJoint>();

		if (ImGui::Button("add joint")){
			ECS::components::RevoluteJoint::Joint joint;
			jointComponent.joints.push_back(joint);
		}

		if (ImGui::BeginDragDropTarget()){
			if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
				Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
				if (entity){
					ECS::components::RevoluteJoint::Joint joint;
					joint.joinedEntity = entity.getUUID();
					jointComponent.joints.push_back(joint);
				} else {
					ENGINE_WARN("invalid entity");
				}
			}
			ImGui::EndDragDropTarget();
		}
	
		auto &joints = jointComponent.joints;
		auto it = joints.begin();
		int i=0;
		float width = ImGui::GetContentRegionAvail().x;

		while (it != joints.end()){
			Entity joinedEntity = editor->activeScene->get(it->joinedEntity);

			ImGui::PushID(it->joinedEntity);
			ImU32 id = ImGui::GetID(reinterpret_cast<void*>(it->joinedEntity.get() + i));
			int opened = storage->GetInt(id, 0);
			ImVec2 pos = ImGui::GetCursorPos();
			std::string jointName;
			
			if (it->joinedEntity != UUID::INVALID_UUID){
				jointName = std::to_string(i) + " : " + joinedEntity.getTag();
			} else {
				jointName = std::to_string(i) + " : None";
			}
			
			if (ImGui::InvisibleButton("0", ImVec2(width - 10, ImGui::GetFontSize()+style.FramePadding.y*2))){
				int* p_opened = storage->GetIntRef(id, 0);
				opened = *p_opened = !*p_opened;
			}

			if (ImGui::BeginPopupContextItem(jointName.c_str())){
				if (opened){
					if (ImGui::MenuItem("close")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				} else {
					if (ImGui::MenuItem("open")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				}

				if (ImGui::MenuItem("remove")){
					joints.erase(it);
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}

				if (ImGui::MenuItem("select entity")){
					if (joinedEntity){
						select(joinedEntity);
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()){
				if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
					Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
					it->joinedEntity = entity.getUUID();
				}	
				ImGui::EndDragDropTarget();
			}

			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			if (hovered || active){
				ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));
			}

			float iconSize = ImGui::GetFontSize()*1.2;
			glm::vec4 UVs;

			ImGui::SetCursorPos(pos);
			if (opened){
				UVs = editor->getIcon(downArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			} else {
				UVs = editor->getIcon(rightArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			}

			ImGui::SameLine();
			ImGui::Text(jointName.c_str());

			if (opened){
				ImGui::TreePush();

				Vec2Edit("anchor", it->anchor);

				ImGui::Checkbox("limits", &it->limits);
				if (it->limits){
					ImGui::DragFloat("min angle", &it->minAngle);
					ImGui::DragFloat("max angle", &it->maxAngle);
				}

				ImGui::Checkbox("motor", &it->motor);
				if (it->motor){
					ImGui::DragFloat("max torque", &it->maxTorque);
					ImGui::DragFloat("speed", &it->speed);
				}

				ImGui::TreePop();
			}

			it++;
			i++;
			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::drawPrismaticJointComponent(Entity entity){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (!entity.hasComponent<ECS::components::PrismaticJoint>()) return;
		auto &jointComponent = entity.getComponent<ECS::components::PrismaticJoint>();

		if (ImGui::Button("add joint")){
			ECS::components::PrismaticJoint::Joint joint;
			jointComponent.joints.push_back(joint);
		}

		if (ImGui::BeginDragDropTarget()){
			if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
				Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
				if (entity){
					ECS::components::PrismaticJoint::Joint joint;
					joint.joinedEntity = entity.getUUID();
					jointComponent.joints.push_back(joint);
				} else {
					ENGINE_WARN("invalid entity");
				}
			}
			ImGui::EndDragDropTarget();
		}
	
		auto &joints = jointComponent.joints;
		auto it = joints.begin();
		int i=0;
		float width = ImGui::GetContentRegionAvail().x;

		while (it != joints.end()){
			Entity joinedEntity = editor->activeScene->get(it->joinedEntity);

			ImGui::PushID(it->joinedEntity);
			ImU32 id = ImGui::GetID(reinterpret_cast<void*>(it->joinedEntity.get() + i));
			int opened = storage->GetInt(id, 0);
			ImVec2 pos = ImGui::GetCursorPos();
			std::string jointName;
			
			if (it->joinedEntity != UUID::INVALID_UUID){
				jointName = std::to_string(i) + " : " + joinedEntity.getTag();
			} else {
				jointName = std::to_string(i) + " : None";
			}
			
			if (ImGui::InvisibleButton("0", ImVec2(width - 10, ImGui::GetFontSize()+style.FramePadding.y*2))){
				int* p_opened = storage->GetIntRef(id, 0);
				opened = *p_opened = !*p_opened;
			}

			if (ImGui::BeginPopupContextItem(jointName.c_str())){
				if (opened){
					if (ImGui::MenuItem("close")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				} else {
					if (ImGui::MenuItem("open")){
						int* p_opened = storage->GetIntRef(id, 0);
						opened = *p_opened = !*p_opened;
					}
				}

				if (ImGui::MenuItem("remove")){
					joints.erase(it);
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}

				if (ImGui::MenuItem("select entity")){
					if (joinedEntity){
						select(joinedEntity);
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()){
				if (auto payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")){
					Entity entity = {*reinterpret_cast<uint32_t*>(payload->Data), editor->activeScene.get()};
					it->joinedEntity = entity.getUUID();
				}	
				ImGui::EndDragDropTarget();
			}

			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			if (hovered || active){
				ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));
			}

			float iconSize = ImGui::GetFontSize()*1.2;
			glm::vec4 UVs;

			ImGui::SetCursorPos(pos);
			if (opened){
				UVs = editor->getIcon(downArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			} else {
				UVs = editor->getIcon(rightArrowIcon);
				ImGui::Image(reinterpret_cast<ImTextureID>(editor->getIcons()->getTexture()), {iconSize*0.7f, iconSize*0.7f}, {UVs.x, UVs.y}, {UVs.z, UVs.w});
			}

			ImGui::SameLine();
			ImGui::Text(jointName.c_str());

			if (opened){
				ImGui::TreePush();

				Vec2Edit("translation", it->translation);

				ImGui::Checkbox("limits", &it->limits);
				if (it->limits){
					ImGui::DragFloat("min translation", &it->minTranslation);
					ImGui::DragFloat("max translation", &it->maxTranslation);
				}

				ImGui::Checkbox("motor", &it->motor);
				if (it->motor){
					ImGui::DragFloat("max force", &it->maxForce);
					ImGui::DragFloat("speed", &it->speed);
				}

				ImGui::TreePop();
			}

			it++;
			i++;
			ImGui::PopID();
		}
	}

	// ========================================================= Methods

	void SceneHierarchyPanel::select(Entity entity){
		EntitySelectedEvent event(entity);
		callback(event);
	}

	void SceneHierarchyPanel::deleteSelectedEntity(){
		if (!selectedEntity) return;

		EntityRemovedEvent event(selectedEntity);
		callback(event);

		editor->activeScene->destroyEntity(selectedEntity);
		selectedEntity.release();
	}

	Entity SceneHierarchyPanel::createNewEntity(){
		Entity entity = editor->activeScene->createEntity();

		EntityCreatedEvent event(entity);
		callback(event);

		select(entity);
		return entity;
	}

	void SceneHierarchyPanel::deleteEntity(Entity entity){
		if (!entity) return;

		EntityRemovedEvent event(entity);
		callback(event);

		if (entity == selectedEntity){
			selectedEntity.release();
		}

		editor->activeScene->destroyEntity(entity);
	}
}