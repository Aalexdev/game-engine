#include "Fovea/PipelineLibrary.hpp"

namespace Fovea{
	void PipelineLibrary::initialize(){

	}

	PipelineLibrary::~PipelineLibrary(){
		for (auto &p : pipelines){
			delete p;
		}

		pipelines.clear();
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

		ID id = pipelines.size();
		pipelines.push_back(pipeline);
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
		auto it = pipelines.begin() + id;

		{
			for (auto &n : nameToIndexMap){
				if (n.second == id){
					nameToIndexMap.erase(n.first);
				}
			}
		}

		// {
		// 	auto pipeline = *it;

		// 	if (*pipeline->refCount == 1){
		// 		auto refIt = refCounts.begin();

		// 		while (refIt != refCounts.end()){
		// 			if (&*refIt == pipeline->refCount){
		// 				refCounts.erase(refIt);
		// 				break;
		// 			}
		// 			it++;
		// 		}
		// 	}
		// }

		delete *it;
		pipelines.erase(it);
	}
}