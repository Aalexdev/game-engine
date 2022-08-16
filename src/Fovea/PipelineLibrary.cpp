#include "Fovea/PipelineLibrary.hpp"

namespace Fovea{

	PipelineLibrary::~PipelineLibrary(){
		clear();
	}

	PipelineLibrary::ID PipelineLibrary::push(Pipeline *pipeline){

		if (pipeline->refCount == nullptr){
			if (refHoles == 0){
				refCounts.push_back(1);
				pipeline->refCount = &refCounts.back();
			} else {
				for (size_t ref = 0; ref < refCounts.size(); ref++){
					if (refCounts[ref] == 0){
						pipeline->refCount = &refCounts[ref];
					}
				}
				refHoles--;
			}
		}

		if (holes == 0){
			ID id = pipelines.size();
			pipelines.push_back(pipeline);
			return id;
		}
		

		ID id = 0;
		for (; id<pipelines.size(); id++){
			if (pipelines[id] == nullptr){
				pipelines[id] = pipeline;
			}
		}

		holes--;
		return id;
	}
	
	Pipeline* PipelineLibrary::get(ID id){
		if (id >= pipelines.size()) return nullptr;
		return pipelines[id];
	}

	void PipelineLibrary::erase(ID id){
		Pipeline* pipeline = get(id);

		if (*pipeline->refCount == 1){
			refHoles++;
		}

		delete pipeline;
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