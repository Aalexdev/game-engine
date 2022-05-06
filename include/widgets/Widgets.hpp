#pragma once

#include <libs/ImGui/imgui.h>
#include <engine/engine.hpp>

namespace engine{

	enum TextureTooltipFlags{
		TextureTooltipFlags_None = 1 << 0,
		TextureTooltipFlags_NoText = 1 << 2,
		TextureTooltipFlags_CheckerboardAlpha = 1 << 3,
		TextureTooltipFlags_VericalFlip = 1 << 4,
		TextureTooltipFlags_HorizontalFlip = 1 << 5,
	};

	/**
	 * @brief render a tooltipe preview of the given texture drawn at the mouse
	 * 
	 * @param label the tag given to the tooltip
	 * @param texture the textureto draw inside the tooltip
	 */
	void TextureTooltip(const char *text, const Ref<SubTexture2D> &texture,TextureTooltipFlags flags = TextureTooltipFlags_None, glm::vec2 size = {100, 100});
	bool TextureEdit(const char* label, Ref<SubTexture2D> &texture, const Ref<Texture2DLibrary> &textureLibrary = nullptr);

	bool subTextureEdit(Ref<SubTexture2D> &texture, const Ref<Texture2DLibrary> &textureLibrary = nullptr);
	
	/**
	 * @brief draw a vec2 editor
	 * 
	 * @param label the tag of the editor
	 * @param values a glm::vec2 reference to the vector
	 * @param resetValue the value to set when the reset button is clicked
	 * @param columnWidth the with of the button
	 * @return true when the vector has changed
	 */
	bool Vec2Edit(const std::string &label, glm::vec2& values, const glm::vec2& resetValue = {0.f, 0.f}, float columnWidth = 100.f);

	
	/**
	 * @brief draw a vec3 editor
	 * 
	 * @param label the tag of the editor
	 * @param values a glm::vec2 reference to the vector
	 * @param resetValue the value to set when the reset button is clicked
	 * @param columnWidth the with of the button
	 * @return true when the vector has changed
	 */
	bool Vec3Edit(const std::string &label, glm::vec3& values, const glm::vec3& resetValue = {0.f, 0.f, 0.f}, float columnWidth = 100.f);
}