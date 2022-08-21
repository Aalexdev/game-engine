#pragma once

namespace RainDrop{
	enum class Key;
	void registerEvents();
	void poolEvents();

	const char* KeyToStr(Key key);
}