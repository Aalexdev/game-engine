#include "Odin/Odin.hpp"

Odin& Odin::getInstance(){
	static Odin instance;
	return instance;
}

Odin::Odin(){
	pthread_mutex_init(&lock, nullptr);
}

Odin::~Odin(){
	pthread_mutex_destroy(&lock);
	clear();
}

void Odin::initialize(){
	getInstance();
}

void Odin::clear(){
	Odin& instance = getInstance();

	for (auto &i : instance.nameToAssetMap){
		deleteAssetData(i.second);
	}

	for (auto &i : instance.typeToFactoryMap){
		Factory* f = i.second;
		delete f;
	}
}

void Odin::removeAsset(std::unordered_map<std::string, AssetData>::iterator it){
	Odin& instance = getInstance();

	assert(it != instance.nameToAssetMap.end() && "cannot remove a not registered asset");
	

	deleteAssetData(it->second);
	instance.nameToAssetMap.erase(it);
}

void Odin::deleteAssetData(AssetData &asset){
	Factory* factory = getTypeFactory(asset.hashCode);
	factory->destroy(asset.assetPtr);

	#ifndef NDEBUG
		if (asset.name) HRM_FREE(asset.name);
		if (asset.assetType) HRM_FREE(asset.assetType);
	#endif
}