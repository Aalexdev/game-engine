#include "Fovea/PipelineLibrary.hpp"

namespace Fovea{

	PipelineLibrary::~PipelineLibrary(){
		clear();
	}

	PipelineLibrary::ID PipelineLibrary::reserve(){
		if (holes == 0){
			ID id = pipelines.size();
			pipelines.push_back(reinterpret_cast<Pipeline*>(0x1));
			return id;
		}

		ID id=0;
		for (; id<pipelines.size(); id++){
			if (pipelines[id] == nullptr){
				pipelines[id] = reinterpret_cast<Pipeline*>(0x1);
				break;
			}
		}

		holes--;
		return id;
	}

	void PipelineLibrary::set(ID id, Pipeline* pipeline){
		pipelines[id] = pipeline;
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
						break;
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
				break;
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

		if (pipeline != reinterpret_cast<Pipeline*>(0x1)){
			if (*pipeline->refCount == 1){
				refHoles++;
			}

			delete pipeline;
		}

		pipelines[id] = nullptr;
		holes++;
	}

	void PipelineLibrary::clear(){
		for (auto &p : pipelines){
			if (p != nullptr & p != reinterpret_cast<Pipeline*>(0x1)) delete p;
		}

		pipelines.clear();
		holes=0;
	}
}