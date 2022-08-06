#include "Fovea/RenderTargetLibrary.hpp"
#include <stdio.h>

namespace Fovea{
	void RenderTargetLibrary::initialize(){

	}

	RenderTargetLibrary::~RenderTargetLibrary(){
		for (auto &r : renderTargets){
			delete r;
		}

		renderTargets.clear();
	}

	RenderTargetLibrary::ID RenderTargetLibrary::push(RenderTargetBuilder *builder, const char *name){
		{
			ID id = getIDFromName(name);
			if (id != invalidID) return id;
		}

		RenderTarget* target = new RenderTarget();

		try {
			target->initialize(*builder);
		} catch (const std::exception &e){
			fprintf(stderr, "std exception catch : \n\twhat() : %s\n", e.what());
			delete target;
			return invalidID;
		}

		ID id = renderTargets.size();
		renderTargets.push_back(target);
		return id;
	}

	void RenderTargetLibrary::erase(ID id){
		auto it = renderTargets.begin() + id;

		for (auto &n : nameToIndexMap){
			if (n.second == id){
				nameToIndexMap.erase(n.first);
			}
		}

		delete *it;
		renderTargets.erase(it);
	}

	RenderTargetLibrary::ID RenderTargetLibrary::getIDFromName(const char *name){
		auto it = nameToIndexMap.find(name);
		if (it == nameToIndexMap.end()) return invalidID;
		return it->second;
	}

	RenderTarget* RenderTargetLibrary::get(ID id){
		if (id >= renderTargets.size()) return nullptr;
		return renderTargets[id];
	}
}