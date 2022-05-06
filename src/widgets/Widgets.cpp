#include <libs/ImGui/imgui_internal.h>
#include <libs/glm/gtc/type_ptr.hpp>
#include "widgets/Widgets.hpp"

namespace engine{

	void TextureTooltip(const char *text, const Ref<SubTexture2D> &texture, TextureTooltipFlags flags, glm::vec2 size){
		
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		// from imgui_widgets.cpp | line 5566 : ColorTooltip

		ImGuiContext& g = *GImGui;

		ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePreviousTooltip, ImGuiWindowFlags_None);

		const char* text_end = text ? ImGui::FindRenderedTextEnd(text, nullptr) : text;

		if (text_end > text && !(flags & TextureTooltipFlags_NoText)){
			ImGui::TextWrapped(text, text_end);
			ImGui::Separator();
		}

		ImVec2 imSize = {size.x, size.y};

		bool hasTexture = texture ? (texture->getTexture() != nullptr) : false;
		ImTextureID textureID = hasTexture ? reinterpret_cast<ImTextureID>(texture->getTexture()->getTexture()) : 0;

		ImVec2 uv0 = hasTexture ? ImVec2{texture->getStart().x, texture->getStart().y} : ImVec2{0, 0};
		ImVec2 uv1 = hasTexture ? ImVec2{texture->getEnd().x, texture->getEnd().y} : ImVec2{1, 1};

		if (flags & TextureTooltipFlags_VericalFlip) std::swap(uv0.x, uv1.x);
		if (flags & TextureTooltipFlags_HorizontalFlip) std::swap(uv0.y, uv1.y);

		auto p = ImGui::GetCursorScreenPos();
		ImGui::Image(textureID, imSize, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y});

		if (texture && texture->getTexture()){

			auto &rend = texture->getRelativeEnd();
			auto &rstart = texture->getRelativeStart();

			ImVec2 start;
			start.x = p.x + (static_cast<float>(rstart.x) / static_cast<float>(texture->getTexture()->getWidth())) * imSize.x;
			start.y = p.y + (static_cast<float>(rstart.y) / static_cast<float>(texture->getTexture()->getHeight())) * imSize.y;

			ImVec2 end;
			end.x = p.x + (static_cast<float>(rend.x) / static_cast<float>(texture->getTexture()->getWidth())) * imSize.x;
			end.y = p.y + (static_cast<float>(rend.y) / static_cast<float>(texture->getTexture()->getHeight())) * imSize.y;

			ImGui::GetWindowDrawList()->AddRect(start, end, IM_COL32(255, 255, 255, 255), 1, 0, 3);
		}

		ImGui::EndTooltip();
	}

	bool TextureEdit(const char* label, Ref<SubTexture2D> &texture, const Ref<Texture2DLibrary> &textureLibrary){
		ImGui::PushID(label);

		ImVec2 region = ImGui::GetContentRegionAvail();
		bool hasTexture = texture ? texture->getTexture() != nullptr : false;
		std::string textureName = hasTexture ? texture->getTexture()->getName() : "None";
		ImTextureID textureId = (hasTexture ? reinterpret_cast<ImTextureID>(texture->getTexture()->getTexture()) : 0);
		bool pressed = false;


		int width = 0, height = 0;
		if (hasTexture){

			width = texture->getTexture()->getWidth();
			height = texture->getTexture()->getHeight();
			ImGui::Text(textureName.c_str());

			ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 1.f, .15f});
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.25f, 1.f});
			ImGui::SameLine();

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

			if (ImGui::Button("X", ImVec2(lineHeight, lineHeight))){
				texture = nullptr;
			}

			ImGui::PopStyleColor(2);

			if (ImGui::IsItemHovered()){
				ImGui::BeginTooltip();
				ImGui::Text("clear");
				ImGui::EndTooltip();
			}

			ImGui::SameLine();

			ImGui::Text("%d : %d", width, height);

		} else {
			ImGui::TextDisabled(textureName.c_str());
			ImGui::SameLine();
			ImGui::TextDisabled("0 : 0");
		}

		float size = ImGui::GetColumnWidth() - ImGui::GetCursorPosX();;
		
		
		if (hasTexture){
			pressed = ImGui::ImageButton(textureId, {size, size}, {texture->getStart().x, texture->getStart().y}, {texture->getEnd().x, texture->getEnd().y}, 0);
		} else {
			pressed = ImGui::ImageButton(textureId, {size, size}, {0, 0}, {1, 1}, 0);
		}

		if (ImGui::IsItemHovered()){
			TextureTooltip(textureName.c_str(), texture, TextureTooltipFlags_NoText, {150, 150});
		}

		if (ImGui::BeginDragDropTarget()){
			auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

			if (payload){
				// convert the path into string
				std::string path = reinterpret_cast<char*>(payload->Data);
				path.resize(payload->DataSize);

				std::filesystem::directory_entry entry(path);


				if (!path.empty() && !entry.is_directory()){
					if (textureLibrary){
						texture = SubTexture2D::create(textureLibrary->load(path));
					} else {
						texture = SubTexture2D::create(Texture2D::create(path, textureLibrary));
					}
				}

			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
		return pressed;
	}

	bool Vec2Edit(const std::string &label, glm::vec2& values, const glm::vec2& resetValue, float columnWidth){

		ImGuiIO &io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		bool update = false;

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 1.f, .15f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.f));
		ImGui::PushFont(boldFont);
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};

		
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue.x;

		ImGui::PopFont();
		ImGui::SameLine();
		update |= ImGui::DragFloat("##X", &values.x, 0.05f);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.6f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.7f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 1.f, 1.f));

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue.y;
		
		ImGui::PopFont();
		ImGui::SameLine();
		update |= ImGui::DragFloat("##Y", &values.y, 0.05f);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
		return update;
	}

	bool Vec3Edit(const std::string &label, glm::vec3& values, const glm::vec3& resetValue, float columnWidth){
		ImGuiIO &io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		bool update = false;

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 1.f, .15f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.f));
		ImGui::PushFont(boldFont);
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};

		
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue.x;

		ImGui::PopFont();
		ImGui::SameLine();
		update |= ImGui::DragFloat("##X", &values.x, 0.05f);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.6f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.7f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 1.f, 1.f));

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue.y;
		
		ImGui::PopFont();
		ImGui::SameLine();
		update |= ImGui::DragFloat("##Y", &values.y, 0.05f);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.6f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.7f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 1.f, 1.f));

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue.z;
		
		ImGui::PopFont();
		ImGui::SameLine();
		update |= ImGui::DragFloat("##Z", &values.z, 0.05f);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
		return update;
	}

	bool subTextureEdit(Ref<SubTexture2D> &texture, const Ref<Texture2DLibrary> &textureLibrary){
		Ref<Texture2D> before = texture ? texture->getTexture() : nullptr;

		static constexpr const int CORNER_NONE = 0;
		static constexpr const int CORNER_START = 1;
		static constexpr const int CORNER_END = 2;
		static int currentSelectedCorner = CORNER_NONE;

		if (TextureEdit("##Texture", texture, textureLibrary)){
			ImGui::OpenPopup("SubTextureEditPopup");
		}

		if (before != (texture ? texture->getTexture() : nullptr)) return true;
		
		ImGui::SetNextWindowContentSize({600, 375});
		if (ImGui::BeginPopup("SubTextureEditPopup")){

			
			// if the user click outside the popup, the popup get closed
			// if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()){
			// 	ImGui::CloseCurrentPopup();
			// 	ImGui::EndPopup();
			// 	return false;
			// }

			Ref<Texture2D> baseTexture = texture ? texture->getTexture() : nullptr;
			uint32_t textureWidth, textureHeight;
			textureWidth = baseTexture ? baseTexture->getWidth() : 0;
			textureHeight = baseTexture ? baseTexture->getHeight() : 0;

			ImGui::Text("  %s", baseTexture ? baseTexture->getName().c_str() : "None");
			ImGui::SameLine();
			ImGui::Text("%d : %d", textureWidth, textureHeight);

			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnWidth(0, 360);

			glm::ivec2 istart = {0, 0};
			glm::ivec2 iend = {0, 0};

			glm::vec2 normStart = {0, 0};
			glm::vec2 normEnd = {0, 0};

			if (texture && texture->getTexture()){
				istart = {texture->getRelativeStart().x, texture->getRelativeStart().y};
				iend = {texture->getRelativeEnd().x, texture->getRelativeEnd().y};

				glm::vec2 size = {texture->getTexture()->getWidth(), texture->getTexture()->getHeight()};
				normStart = glm::vec2(istart) / size;
				normEnd = glm::vec2(iend) / size;
			}

			// store the start position of the image for the selection circles
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::Image(reinterpret_cast<ImTextureID>(baseTexture ? baseTexture->getTexture() : 0), {350, 350}, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y});

			if (ImGui::IsMouseClicked(0)){
				currentSelectedCorner = CORNER_NONE;
			}

			// draw list

			if (texture && texture->getTexture()){
				ImVec2 start = {p.x + (normStart.x * 350), p.y + (normStart.y * 350)};
				ImVec2 end = {p.x + (normEnd.x * 350), p.y + (normEnd.y * 350)};
				ImVec2 max = {p.x + 350, p.y + 350};

				ImGui::GetWindowDrawList()->AddRectFilled(p, {max.x, start.y}, IM_COL32(50, 50, 50, 150));
				ImGui::GetWindowDrawList()->AddRectFilled({p.x, start.y}, {start.x, end.y}, IM_COL32(50, 50, 50, 150));
				ImGui::GetWindowDrawList()->AddRectFilled({end.x, start.y}, {max.x, end.y}, IM_COL32(50, 50, 50, 150));
				ImGui::GetWindowDrawList()->AddRectFilled({p.x, end.y}, max, IM_COL32(50, 50, 50, 150));

				ImGui::GetWindowDrawList()->AddRect(start, end, IM_COL32(150, 150, 150, 150), 0, 0, 3);
				ImGui::GetWindowDrawList()->AddCircle(start, currentSelectedCorner == CORNER_START ? 13 : 10, IM_COL32(255, 255, 255, 255), 0, currentSelectedCorner == CORNER_START ? 2 : 1); // start
				ImGui::GetWindowDrawList()->AddCircle(end, currentSelectedCorner == CORNER_END ? 13 : 10, IM_COL32(255, 255, 255, 255), 0, currentSelectedCorner == CORNER_END ? 2 : 1); // end
			}

			auto &io = ImGui::GetIO();

			switch (currentSelectedCorner){
				case CORNER_START:{

					// get to mouse coords relative to the image start pos
					auto pos = io.MousePos;
					pos.x -= p.x;
					pos.y -= p.y;

					// normalize the mouse pos
					pos.x /= 350.f;
					pos.y /= 350.f;

					// convert to the texture coords
					pos.x *= static_cast<float>(baseTexture ? baseTexture->getWidth() : 1);
					pos.y *= static_cast<float>(baseTexture ? baseTexture->getHeight() : 1);

					if (texture) texture->setStart(static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y));
					break;
				}

				case CORNER_END:{

					// get to mouse coords relative to the image start pos
					auto pos = io.MousePos;
					pos.x -= p.x;
					pos.y -= p.y;

					// normalize the mouse pos
					pos.x /= 350.f;
					pos.y /= 350.f;

					// convert to the texture coords
					pos.x *= static_cast<float>(baseTexture ? baseTexture->getWidth() : 1);
					pos.y *= static_cast<float>(baseTexture ? baseTexture->getHeight() : 1);

					if (texture) texture->setEnd(static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y));
					break;
				}
			}

			ImGui::NextColumn();
			// ImGui::SetColumnWidth(1, 100);

			if (textureLibrary){
				static char textureNameBuffer[255];

				std::string buttonName = baseTexture ? baseTexture->getName() : "None";
				if (ImGui::Button(buttonName.empty() ? "unnamed" : buttonName.c_str())){
					ImGui::OpenPopup("SubTextureTextureListPopup");
					strcpy_s(textureNameBuffer, baseTexture ? baseTexture->getName().c_str() : "None");
				}

				if (ImGui::BeginPopup("SubTextureTextureListPopup")){
					static int currentItem;

					if (ImGui::InputText("##textureName", textureNameBuffer, sizeof(textureNameBuffer))){
					
					}

					ImVec2 region = ImGui::GetContentRegionAvail();
					region.y = 250;

					if (ImGui::BeginListBox("##SubTextureTextureListBox", region)){
						ImVec2 size = ImGui::GetContentRegionAvail();
						size.y = ImGui::CalcTextSize(" ").y + 3;
						for (auto &text : textureLibrary->getList()){

							
							ImGui::PushID(text->getUUID());
							if (ImGui::Button(text->getName().empty() ? "unamed" : text->getName().c_str(), size)){
								ImGui::CloseCurrentPopup();
								texture = SubTexture2D::create(text);
							}

							if (ImGui::IsItemHovered()){
								ImGui::BeginTooltip();
								ImGui::Image(reinterpret_cast<ImTextureID>(text->getTexture()), {100, 100}, {Texture2D::topCorner().x, Texture2D::topCorner().y}, {Texture2D::bottomCorner().x, Texture2D::bottomCorner().y});
								ImGui::EndTooltip();
							}

							ImGui::PopID();
						}
						ImGui::EndListBox();
					}
					ImGui::EndPopup();
				}
			}

			if (ImGui::BeginChild("##UVpos")){

				ImVec2 btnSize = ImGui::CalcTextSize("start");
				btnSize.x += 5;
				btnSize.y += 3;

				if (ImGui::Button("start", btnSize)){
					currentSelectedCorner = CORNER_START;
				}

				ImGui::SameLine();
				if (ImGui::InputInt2("##start", glm::value_ptr(istart))) {texture->setStart(istart.x, istart.y);}
				ImGui::SameLine();
				ImGui::PushID("start");
				if (ImGui::Button("reset")){
					if (texture) texture->setStart(0, 0);
				}
				ImGui::PopID();

				if (ImGui::Button("end", btnSize)){
					currentSelectedCorner = CORNER_END;
				}

				ImGui::SameLine();
				if (ImGui::InputInt2("##end", glm::value_ptr(iend))) {texture->setEnd(iend.x, iend.y);}

				ImGui::SameLine();
				ImGui::PushID("end");
				if (ImGui::Button("reset")){
					if (texture && texture->getTexture()) texture->setEnd(texture->getTexture()->getWidth(), texture->getTexture()->getHeight());
				}
				ImGui::PopID();
				ImGui::EndChild();
			}

			ImGui::Columns(1);
			ImGui::EndPopup();
		}
		return false;
	}
}