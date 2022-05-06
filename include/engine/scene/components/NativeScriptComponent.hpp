#pragma once

#include "../../engine.hpp"
#include "../ScriptableEntity.hpp"

namespace engine::components{
	struct ENGINE_API NativeScript{
		ScriptableEntity* entity = nullptr;

		ScriptableEntity*(*instantiateFnc)();
		void (*destroyFnc)(NativeScript*);

		template<typename T>
		void bind(){
			instantiateFnc = []() {return static_cast<ScriptableEntity*>(new T());};
			destroyFnc = [](NativeScript* nsc) {delete nsc->instance; nsc->instance = nullptr;};
		}

		NativeScript() = default;
		NativeScript(const NativeScript&) = default;
		NativeScript(ScriptableEntity &entity) : entity{&entity} {}
	};
}