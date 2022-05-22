#pragma once

#include "Panel.hpp"
#include "../Event/EditorEvents.hpp"

namespace engine{
	class SceneHierarchyPanel : public Panel{
		public:
			SceneHierarchyPanel();
			~SceneHierarchyPanel(){}
		
			virtual void OnImGuiRender() override;
			virtual void OnEvent(EditorEvent &e) override;

			virtual void serialize(YAML::Emitter &out) override;
			virtual void deserialize(YAML::Node data) override;

			virtual PanelType getStaticType() const override {return PanelType::SCENE_HIERARCHY_PANEL;}

			static Ref<SceneHierarchyPanel> create();
		
		private:

			enum class SceneHierarchyPanelType{
				PANEL_NONE = 0,
				PANEL_SCENE_HIERARCHY,
				PANEL_ENTITY_COMPONENTS
			};

			// === Events ===
			bool OnEntitySelected(EntitySelectedEvent &e);
			bool OnEngineEvent(EngineEvent &e);
			bool OnKeyPressed(KeyPressedEvent &e);

			bool KeyPressedOnComponentPanel(KeyPressedEvent &e);
			bool KeyPressedOnHierarchyPanel(KeyPressedEvent &e);


			// === ImGui ===
			bool drawEntityTreeNode(Entity entity);
			void drawEntityNode(Entity entity);
			void drawEntityComponents(Entity entity);
			void drawComponentsPanel(); // draw a panel to change the values of the selected entity's components
			void drawAddComponentMenu(Entity entity);

			// components
			void drawSpriteComponent(Entity entity);
			void drawTransformComponent(Entity entity);
			void drawCameraComponent(Entity entity);
			void drawRigidBodyComponent(Entity entity);
			void drawBoxColliderComponent(Entity entity);
			void drawCircleRendererComponent(Entity entity);
			void drawTriangleRendererComponent(Entity entity);
			void drawCircleColliderComponent(Entity entity);
			void drawDistanceJointComponent(Entity entity);
			void drawSpringJointComponent(Entity entity);
			void drawRevoluteJointComponent(Entity entity);
			void drawPrismaticJointComponent(Entity entity);
			void drawFrictionJointComponent(Entity entity);

			// === Members ===
			Entity selectedEntity; // the entity that is currently selected

			// key
			KeyInput newEntityKey = {{Key::KEY_N}, {Key::KEY_RIGHT_CONTROL, Key::KEY_LEFT_CONTROL}};
			KeyInput deleteEntityKey = {{Key::KEY_DELETE, Key::KEY_BACKSPACE}};

			std::string cameraIcon;
			std::string entityIcon;
			// std::string lightIcon;
			std::string downArrowIcon;
			std::string rightArrowIcon;

			SceneHierarchyPanelType currentHoveredPanel; // the panel that is currently hovered by the mouse

			// methodes
			// check if the given entity is currently selected
			bool selected(Entity entity) const {return entity == selectedEntity;}
			void select(Entity entity); // selected the given entity
			void deleteSelectedEntity();
			void deleteEntity(Entity entity);
			Entity createNewEntity();
	};
}