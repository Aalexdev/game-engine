#include "Fovea/DescriptorSetLibrary.hpp"

namespace Fovea{
	void DescriptorSetLibrary::initialize(){

	}

	DescriptorSetLibrary::~DescriptorSetLibrary(){
		clear();
	}

	DescriptorSetLibrary::ID DescriptorSetLibrary::push(DescriptorSetBuilder *builder, const char *name){
		{
			ID id = getIDFromName(name);
			if (id != invalidID) return id;
		}

		DescriptorSet* set = new DescriptorSet();

		try {
			set->initialize(*builder);
		} catch (const std::exception &e){
			fprintf(stderr, "std exception catch : \n\twhat() : %s\n", e.what());
			delete set;
			return invalidID;
		}

		ID id = 0;
		if (holes == 0){
			id = sets.size();
			sets.push_back(set);
			return id;
		}

		for (; id<sets.size(); id++){
			if (sets[id] == nullptr){
				sets[id] = set;
				break;
			}
		}

		holes--;
		
		return id;
	}

	void DescriptorSetLibrary::erase(ID id){
		auto it = sets.begin() + id;

		for (auto &n : nameToIndexMap){
			if (n.second == id){
				nameToIndexMap.erase(n.first);
			}
		}

		delete *it;
		sets.erase(it);
	}

	DescriptorSetLibrary::ID DescriptorSetLibrary::getIDFromName(const char *name){
		auto it = nameToIndexMap.find(name);
		if (it == nameToIndexMap.end()) return invalidID;
		return it->second;
	}

	DescriptorSet* DescriptorSetLibrary::get(ID id){
		if (id >= sets.size()) return nullptr;
		return sets[id];
	}

	void DescriptorSetLibrary::clear(){
		for (auto &s : sets){
			delete s;
		}

		sets.clear();
		holes = 0;
	}
}