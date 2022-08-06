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
		delete *it;
		pipelines.erase(it);
	}
}