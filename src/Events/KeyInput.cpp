
#include "engine/Events/Input.hpp"
#include "engine/Events/KeyInput.hpp"
#include "engine/debug/Instrumentor.hpp"

namespace engine{

	static void printKeys(std::string &str, const std::vector<Key> &keys){
		str += "(";
		for (int i=0; i<keys.size(); i++){
			str += "'" + std::string(KeyToStr(keys[i])) + "'";
			if (i != keys.size()-1){
				str += "or ";
			}
		}
		str += ")";
	}

	const std::string KeyInput::toString(){
		ENGINE_PROFILE_FUNCTION();
		std::string str;

		for (int i=0; i<keys.size(); i++){
			printKeys(str, keys[i]);
			if (i != keys.size()-1){
				str += " and ";
			}
		}
		return str;
	}

	void KeyInput::push(std::vector<Key> keys){
		ENGINE_PROFILE_FUNCTION();
		this->keys.push_back(keys);
	}

	// check if there is at least one key down in the given keys vector
	static bool isOneKeyDown(const std::vector<Key> &keys, Input *input){
		bool down = keys.empty(); // if there is no keys, this count as down

		for (const auto &key : keys){
			if (input->isKeyDown(key)){
				down = true;
				break;
			}
		}

		return down;
	}

	// check if there is at least one key up in the given keys vector
	static bool isOneKeyUp(const std::vector<Key> &keys, Input *input){
		bool up = keys.empty(); // if there is no keys, this count as up

		for (const auto &key : keys){
			if (input->isKeyUp(key)){
				up = true;
				break;
			}
		}

		return up;
	}

	bool KeyInput::isDown(Input *input) const{
		ENGINE_PROFILE_FUNCTION();

		for (const auto &key : keys){
			if (!isOneKeyDown(key, input)){
				return false;
			}
		}
		return true;
	}

	bool KeyInput::isUp(Input *input) const{
		ENGINE_PROFILE_FUNCTION();

		for (const auto &key : keys){
			if (!isOneKeyUp(key, input)){
				return false;
			}
		}
		return true;
	}

	bool KeyInput::OnKeyPressed(KeyPressedEvent e, Input *input){
		for (const auto &keyGroup : keys){
			bool down = keyGroup.empty();

			for (const auto &key : keyGroup){

				if (e.getKeyCode() == key){
					down = true;
					break;

				} else if (input->isKeyDown(key)){
					down = true;
					break;
				}
			}

			if (!down) return false;
		}
		return true;
	}
}

namespace YAML{
	Emitter& ENGINE_API operator<<(YAML::Emitter& out, const engine::KeyInput &key){
		out << YAML::Flow;
		out << YAML::BeginSeq;

		for (const auto &i : key.get()){
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (const auto &j : i){
				out << static_cast<int>(j);
			}
			out << YAML::EndSeq;
		}

		out << YAML::EndSeq;
		return out;
	}
}