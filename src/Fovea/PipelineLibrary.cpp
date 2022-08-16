#include "Fovea/PipelineLibrary.hpp"

namespace Fovea{
	void PipelineLibrary::initialize(){

	}

	PipelineLibrary::~PipelineLibrary(){
		clear();
	}

	PipelineLibrary::ID PipelineLibrary::push(PipelineBuilder *builder, const char *name){

		{
			ID id = getIDFromName(name);
			if (id != invalidID) return id;
		}

		Pipeline* pipeline = new Pipeline;

		try {
			pipeline->initialize(builder);
		} catch (const std::exception &e){
			fprintf(stderr, "std exception catch : \n\twhat() : %s\n", e.what());
			delete pipeline;
			return invalidID;
		}

		if (pipeline->refCount == nullptr){
			refCounts.push_back(1);
			pipeline->refCount = &refCounts.back();
		}

		ID id = 0;
		
		if (holes == 0){
			id = pipelines.size();
			pipelines.push_back(pipeline);
			return id;
		}

		for (; id<pipelines.size(); id++){
			if (pipelines[id] == nullptr){
				pipelines[id] = pipeline;
				break;
			}
		}

		holes--;

		return id;
	}

	PipelineLibrary::ID PipelineLibrary::getIDFromName(const std::string &name){
		auto it = nameToIndexMap.find(name);
		if (it == nameToIndexMap.end()) return invalidID;
		return it->second;
	}
	
	Pipeline* PipelineLibrary::get(ID id){
		if (id >= pipelines.size()) return nullptr;
		return pipelines[id];
	}

	void PipelineLibrary::erase(ID id){

		{
			for (auto &n : nameToIndexMap){
				if (n.second == id){
					nameToIndexMap.erase(n.first);
				}
			}
		}

		{
			auto pipeline = pipelines[id];

			if (*pipeline->refCount == 1){
				auto it = refCounts.begin();

				while (it != refCounts.end()){
					if (&*it == pipeline->refCount){
						refCounts.erase(it);
						break;
					}
					it++;
				}
			}
		}

		auto it = pipelines.begin() + id;

		delete *it;
		pipelines[id] = nullptr;
		holes++;
	}

	void PipelineLibrary::clear(){
		for (auto &p : pipelines){
			delete p;
		}

		pipelines.clear();
		holes=0;
	}
}