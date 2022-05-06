#pragma once

#include "../core.hpp"
#include "KeyEvent.hpp"
#include "Keycodes.hpp"
#include <libs/yaml-cpp/yaml.h>

namespace engine{

	class Input;

	class ENGINE_API KeyInput{
		public:
			KeyInput(std::initializer_list<std::vector<Key>> keys) : keys{keys} {}
			KeyInput(){}

			const std::string toString();
			const std::vector<std::vector<Key>>& get() const {return keys;}

			void push(std::vector<Key> keys);

			bool isDown(Input *input) const;
			bool isUp(Input *input) const;

			bool OnKeyPressed(KeyPressedEvent e, Input *input);

			inline void clear() {keys.clear();}

		private:
			std::vector<std::vector<Key>> keys;

	};
}

namespace YAML{

	// [[key1a, key1b, key1c], [key2a], [key3a, key3b]]
	Emitter& ENGINE_API operator<<(YAML::Emitter& out, const engine::KeyInput &key);

	template<>
	struct convert<engine::KeyInput>{
		static Node encode(const engine::KeyInput &key){
			Node node;

			for (const auto &i : key.get()){
				for (const auto &j : i){
					node.push_back(static_cast<int>(j));
				}
			}
			
			return node;
		}

		static bool decode(const Node &node, engine::KeyInput &key){
			if (!node.IsSequence()) return false;

			for (const auto &keyGroup : node){
				if (!keyGroup.IsSequence()) continue;

				std::vector<engine::Key> keys;
				for (const auto &k : keyGroup){
					keys.push_back(static_cast<engine::Key>(k.as<int>()));
				}

				key.push(keys);
			}

			return true;
		}
	};
}