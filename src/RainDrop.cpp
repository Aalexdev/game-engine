#include "Odin.hpp"
#include "core.hpp"
#include "Fovea.h"
#include "Gramophone.hpp"
#include "Hermes.hpp"
#include "horreum/Horreum.hpp"
#include "EventManager.hpp"
#include "RainDrop.hpp"

#define RD_TRHOW_EXCEPT(what, why) throw RainDrop::Exception(what, __func__, why);

namespace RainDrop{

	// ==== exception ====
	Exception::Exception(const char *what, const char *where, const char *why) : m_what{what}, m_where{where}, m_why{why}{}
	const char* Exception::what() const {return m_what;}
	const char* Exception::where() const {return m_where;}
	const char* Exception::why() const {return m_why;}

	// === window ===
	void createWindow(){
		Core& instance = getInstance();
		
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		instance.window = SDL_CreateWindow("RainDrop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

		if (!instance.window){
			throw SDL_GetError();
		}
	}

	void shutdownWindow(){
		Core& instance = getInstance();
		
		if (instance.window) SDL_DestroyWindow(instance.window);
		instance.window = nullptr;
		SDL_Quit();
	}

	// === ECS ===

	void initializeECS(){
		ECS::Coordinator& coordinator = getInstance().scene;
		coordinator.Init();
	}

	// === API functions ===

	void RD_API initialize(){
		Core& instance = getInstance();

		try {
			createWindow();
		} catch (const char *err){
			RD_TRHOW_EXCEPT("failed to initialized window", err);
		}

		FoveaInitialize(static_cast<void*>(instance.window));
		Horreum::initialize();
		Gramophone::initialize();
		Odin::initialize();
		Hermes::initialize(150, 1500);

		initializeECS();
		registerEvents();
	}

	void RD_API shutdown(){
		shutdownWindow();
	}

	// ====================================== Window

	void RD_API setWindowSize(const vec2<uint32_t> &size){
		SDL_SetWindowSize(getInstance().window, static_cast<int>(size.x), static_cast<int>(size.y));
		FoveaOnWindowResized(size.x, size.y);
	}

	void RD_API setWindowPosition(const vec2<uint32_t> &pos){
		SDL_SetWindowPosition(getInstance().window, static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	void RD_API setWindowTitle(const char *title){
		SDL_SetWindowTitle(getInstance().window, title);
	}

	void RD_API setWindowFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, SDL_WINDOW_FULLSCREEN);
	}

	void RD_API unsetWindowFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, 0);
	}

	void RD_API setWindowDesktopFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	void RD_API setWindowResizable(bool resizable){
		SDL_SetWindowResizable(getInstance().window, static_cast<SDL_bool>(resizable));
	}

	// events
	EventID RD_API registerEvent(const char* name, uint32_t dataSize){
		return Hermes::registerEvent(name, static_cast<uint16_t>(dataSize));
	}

	EventID RD_API getEventID(const char* name){
		return Hermes::getEventIndex(name);
	}

	void RD_API subscribeEvent(const char* name, bool(*FNcallback)(void*)){
		try{
			Hermes::subscribe(name, FNcallback);
		} catch (const char* err){
			RD_TRHOW_EXCEPT("failed to subscribe to event", err);
		}
	}

	void RD_API subscribeEvent(const char* name, void* instance, bool(*MTcallback)(void*, void*)){
		try{
			Hermes::subscribe(name, instance, MTcallback);
		} catch (const char* err){
			RD_TRHOW_EXCEPT("failed to subscribe to event", err);
		}
	}
	
	void RD_API subscribeEvent(EventID id, bool(*FNcallback)(void*)){
		Hermes::subscribe(static_cast<Hermes::EventID>(id), FNcallback);
	}

	void RD_API subscribeEvent(EventID id, void* instance, bool(*MTcallback)(void*, void*)){
		Hermes::subscribe(static_cast<Hermes::EventID>(id), instance, MTcallback);
	}

	void RD_API unsubscribeEvent(const char* name, bool(*FNcallback)(void*)){
		try{
			Hermes::unsubscribe(name, FNcallback);
		} catch (const char* err){
			RD_TRHOW_EXCEPT("failed to unsubscribe to event", err);
		}
	}

	void RD_API unsubscribeEvent(const char* name, bool(*MTcallback)(void*, void*)){
		try{
			Hermes::unsubscribe(name, MTcallback);
		} catch (const char* err){
			RD_TRHOW_EXCEPT("failed to unsubscribe to event", err);
		}
	}
	
	void RD_API unsubscribeEvent(EventID id, bool(*FNcallback)(void*)){
		Hermes::unsubscribe(static_cast<Hermes::EventID>(id), FNcallback);
	}

	void RD_API unsubscribeEvent(EventID id, bool(*MTcallback)(void*, void*)){
		Hermes::unsubscribe(static_cast<Hermes::EventID>(id), MTcallback);
	}

	uint32_t RD_API getEventDataSize(const char* name){
		return getEventDataSize(getEventID(name));
	}

	uint32_t RD_API getEventDataSize(EventID id){
		return static_cast<uint32_t>(Hermes::getEventDataSize(static_cast<Hermes::EventID>(id)));
	}

	void RD_API triggerEventPtr(EventID id, void* data){
		Hermes::_triggerEvent(static_cast<Hermes::EventID>(id), data);
	}

	void* RD_API __eventAllocStack(uint32_t size){
		return Hermes::allocStack(static_cast<size_t>(size));
	}

	bool RD_API isKeyDown(Key key){
		return getInstance().keyPressed[static_cast<int>(key)];
	}

	bool RD_API isKeyUp(Key key){
		return !getInstance().keyPressed[static_cast<int>(key)];
	}

	vec2<float> RD_API getMousePosition(){
		return getInstance().mousePos;
	}

	bool RD_API isMouseButtonDown(MouseButton button){
		return getInstance().buttonPressed[static_cast<int>(button)];
	}

	bool RD_API isMouseButtonUp(MouseButton button){
		return !getInstance().buttonPressed[static_cast<int>(button)];
	}

	void RD_API updateEvents(){
		poolEvents();
		Hermes::update();
	}


	// ====================================== Render

	void RD_API setClearColor(const Color &color){
		FoveaSetClearColor({color.r, color.g, color.b, color.a});
	}

	void RD_API beginFrame(){
		FoveaBeginFrame();
	}

	void RD_API endFrame(){
		FoveaEndFrame();
	}

	void RD_API setScene(void* vertices, uint32_t vertexCount){
		FoveaSetScene(vertices, vertexCount);
	}

	void RD_API setSceneVertexSize(uint64_t size){
		FoveaSetSceneVertexSize(size);
	}

	void RD_API renderScene(){
		FoveaRenderScene();
	}

	void RD_API setSceneData(uint32_t offset, uint32_t count, void* data){
		FoveaSetSceneData(offset, count, data);
	}

	void RD_API beginScene(){
		auto& instance = getInstance();
		instance.vertexPtr = getSceneBuffer();
		instance.currentRenderBuffer = RenderBuffer::Scene;
		instance.vertexCount = 0;
	}

	void RD_API endScene(){
		auto& instance = getInstance();
		FoveaFlushSceneData(0, instance.vertexCount);
		
		instance.vertexPtr = nullptr;
		instance.currentRenderBuffer = RenderBuffer::None;
	}

	void RD_API renderQuad(void* v0, void* v1, void* v2, void* v3){
		auto& instance = getInstance();
		uint32_t vertexSize = FoveaGetSceneVertexSize();
		char* cptr = static_cast<char*>(instance.vertexPtr);

		memcpy(instance.vertexPtr, v0, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v1, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v2, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v3, vertexSize);
		cptr += vertexSize;
		instance.vertexPtr = cptr;
	}

	void RD_API renderTrigone(void* v0, void* v1, void* v2){
		auto& instance = getInstance();
		uint32_t vertexSize = FoveaGetSceneVertexSize();
		char* cptr = static_cast<char*>(instance.vertexPtr);

		memcpy(instance.vertexPtr, v0, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v1, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v2, vertexSize);
		cptr += vertexSize;
		instance.vertexPtr = cptr;
	}

	void RD_API renderLine(void* v0, void* v1){
		auto& instance = getInstance();
		uint32_t vertexSize = FoveaGetSceneVertexSize();
		char* cptr = static_cast<char*>(instance.vertexPtr);

		memcpy(instance.vertexPtr, v0, vertexSize);
		cptr += vertexSize;
		memcpy(instance.vertexPtr, v1, vertexSize);
		cptr += vertexSize;
		instance.vertexPtr = cptr;
	}

	void RD_API flushSceneData(uint32_t offset, uint32_t count){
		FoveaFlushSceneData(offset, count);
	}

	void RD_API setGeneralUsageVertexSize(uint32_t size){
		FoveaSetGeneralUsageVertexSize(size);
	}

	void RD_API setGeneralUsageVertices(void* vertices, uint32_t vertexCount){
		FoveaSetGeneralUsage(vertices, vertexCount);
	}

	void RD_API setGeneralUsageData(uint32_t offset, uint32_t count, void* data){
		FoveaSetGeneralUsageData(offset, count, data);
	}

	void RD_API flushGeneralUsageData(uint32_t offset, uint32_t count){
		FoveaFlushGeneralUsageData(offset, count);
	}

	void RD_API renderGeneralUsageVertices(){
		FoveaRenderGeneralUsageData();	
	}

	void RD_API setGeneralUsageTopology(Topology topology){
		FoveaSetGeneralUsageTopology(static_cast<FoveaTopology>(topology));
	}

	void RD_API beginSwapChainRenderPass(){
		FoveaBeginSwapChainRenderPass();
	}

	void RD_API endSwapChainRenderPass(){
		FoveaEndSwapChainRenderPass();
	}

	// ====================================== ECS

	Entity RD_API createEntity(){
		return getInstance().scene.CreateEntity();
	}

	void RD_API destroyEntity(Entity entity){
		return getInstance().scene.DestroyEntity(entity.getUID());
	}

	void RD_API entityAddComponent(Entity entity, void* component, uint64_t typeID){
		getInstance().scene.AddComponent(entity.getUID(), component, typeID);
	}

	void RD_API entityRemoveComponent(Entity entity, uint64_t typeID){
		getInstance().scene.RemoveComponent(entity.getUID(), typeID);
	}

	bool RD_API entityHasComponent(Entity entity, uint64_t typeID){
		return getInstance().scene.HasComponent(entity.getUID(), typeID);
	}

	void* RD_API entityGetComponent(Entity entity, uint64_t typeID){
		return getInstance().scene.GetComponent(entity.getUID(), typeID);
	}

	void RD_API registerEntityComponent(uint64_t typeID, uint64_t typeSize){
		getInstance().scene.RegisterComponent(typeID, typeSize);
	}

	void RD_API registerECSSystemPtr(size_t typeID, ECSSystem* system){
		getInstance().scene.RegisterSystem(typeID, reinterpret_cast<ECS::System*>(system));
	}

	void RD_API setECSSystemSignature(size_t typeID, ECSSignature& signature){
		getInstance().scene.SetSystemSignature(typeID, signature);
	}

	uint32_t RD_API getComponentID(uint64_t typeID){
		return getInstance().scene.GetComponentType(typeID);
	}

	// ============================= SHADERID

	ShaderID RD_API createShader(ShaderCreateInfo &info){
		FoveaShaderCreateInfo createInfo;

		createInfo.sample = static_cast<FoveaSample>(info.sample);
		createInfo.type = FoveaShaderType_Graphic;
		createInfo.pushConstantSize = info.pushConstantSize;
		createInfo.target = static_cast<FoveaRenderTarget>(info.renderTargetID);
		createInfo.base = static_cast<FoveaShader>(info.base);
		createInfo.vertexInputSize = info.vertexInputSize;
		createInfo.vertexAttributes = reinterpret_cast<FoveaShaderVertexAttribute*>(info.vertexAttributes);
		createInfo.vertexAttributsCount = info.vertexAttributeCount;
		createInfo.descriptorSets = reinterpret_cast<FoveaDescriptorSet*>(info.descriptorSets);
		createInfo.descriptorSetCount = info.descriptorSetCount;
		createInfo.polygonMode = static_cast<FoveaPolygonMode>(info.polygoneMode);
		createInfo.lineWidth = info.lineWidth;
		createInfo.depthTest = static_cast<FoveaBool>(info.depthTest);
		createInfo.blending = static_cast<FoveaBool>(info.blending);
		createInfo.vertexFilepath = info.vertexFilepath.empty() ? nullptr : info.vertexFilepath.c_str();
		createInfo.fragmentFilepath = info.fragmentFilepath.empty() ? nullptr : info.fragmentFilepath.c_str();
		createInfo.geometryFilepath = info.geometryFilepath.empty() ? nullptr : info.geometryFilepath.c_str();

		ShaderID shaderid;
		try{
			shaderid = static_cast<ShaderID>(FoveaCreateShader(&createInfo));
		} catch (const std::runtime_error &err){
			RD_TRHOW_EXCEPT("failed to create a shaderid", err.what());
		}

		return shaderid;
	}

	void RD_API destroyShader(ShaderID shaderid){
		FoveaDestroyShader(static_cast<FoveaShader>(shaderid));
	}

	void RD_API useShader(ShaderID shaderid, uint32_t *descriptorSetIndices){
		FoveaUseShader(static_cast<FoveaShader>(shaderid), descriptorSetIndices);
	}

	void RD_API setShaderPushConstant(ShaderID shaderid, void* pushConstant){
		FoveaSetShaderPushConstant(static_cast<FoveaShader>(shaderid), pushConstant);
	}

	// ==================================== RENDER TARGET

	RenderTargetID RD_API createRenderTarget(RenderTargetCreateInfo &info){
		FoveaRenderTargetCreateInfo createInfo;

		createInfo.size = *reinterpret_cast<FoveaUIVec2*>(&info.size);
		createInfo.colorAttachmentCount = info.colorAttachmentCount;
		createInfo.depthFormat = static_cast<FoveaImageFormat>(info.depthFormat);
		createInfo.depthStencilEnabled = static_cast<FoveaBool>(info.depthStencilEnabled);
		createInfo.depthClearValue = info.depthClearValue;
		createInfo.stencilClearValue = info.stencilClearValue;
		createInfo.colorAttachments = reinterpret_cast<FoveaRenderTargetAttachment*>(info.colorAttachments);

		RenderTargetID target;
		try{
			target = static_cast<RenderTargetID>(FoveaCreateRenderTarget(&createInfo));
		} catch (const std::runtime_error& err){
			RD_TRHOW_EXCEPT("failed to create render target", err.what());
		}
		return target;
	}

	void RD_API destroyRenderTarget(RenderTargetID renderTargetID){
		FoveaDestroyRenderTarget(static_cast<FoveaRenderTarget>(renderTargetID));
	}

	void RD_API beginRenderTarget(RenderTargetID renderTargetID){
		FoveaBeginRenderTarget(static_cast<FoveaRenderTarget>(renderTargetID));
	}

	void RD_API endRenderTarget(RenderTargetID renderTargetID){
		FoveaEndRenderTarget(static_cast<FoveaRenderTarget>(renderTargetID));
	}

	void RD_API resizeRenderTarget(RenderTargetID renderTargetID, vec2<uint32_t> size){
		FoveaResizeRenderTarget(static_cast<FoveaRenderTarget>(renderTargetID), *reinterpret_cast<FoveaUIVec2*>(&size));
	}

	// =================================== DESCRIPTOR SET

	DescriptorSetID RD_API createDescriptorSet(DescriptorSetCreateInfo& info){
		FoveaDescriptorSetCreateInfo createInfo;

		createInfo.descriptors = reinterpret_cast<FoveaDescriptorInfo*>(info.descriptors);
		createInfo.descriptorCount = info.descriptorCount;
		createInfo.setCount = info.setCount;

		DescriptorSetID set;
		try{
			set = static_cast<DescriptorSetID>(FoveaCreateDescriptorSet(&createInfo));
		} catch (const std::runtime_error& err){
			RD_TRHOW_EXCEPT("failed to create a descriptor set", err.what());
		}

		return set;
	}

	void RD_API destroyDescriptorSet(DescriptorSetID set){
		FoveaDestroyDescriptorSet(static_cast<FoveaDescriptorSet>(set));
	}

	void RD_API writeToDescriptorSetBuffer(DescriptorSetID set, uint32_t setIndex, uint32_t binding, void* data){
		FoveaWriteToDescriptorSetBuffer(static_cast<FoveaDescriptorSet>(set), setIndex, binding, data);
	}

	void RD_API setDescriptorSetTexture(DescriptorSetID set, uint32_t setIndex, uint32_t binding, uint32_t textureIndex, TextureID textureid){
		FoveaSetDescriptorSetTexture(static_cast<FoveaDescriptorSet>(set), static_cast<FoveaTexture>(textureid), setIndex, binding, textureIndex);
	}

	// =================================== TEXTUREID

	void RD_API convertTextureCreateInfo(TextureCreateInfo& info, void* ptr){
		FoveaTextureCreateInfo* createInfo = static_cast<FoveaTextureCreateInfo*>(ptr);

		createInfo->magFilter = static_cast<FoveaImageTiling>(info.magFilter);
		createInfo->minFilter = static_cast<FoveaImageTiling>(info.minFilter);
		createInfo->addressModeX = static_cast<FoveaTextureAddressMode>(info.addressModeX);
		createInfo->addressModeY = static_cast<FoveaTextureAddressMode>(info.addressModeY);
		createInfo->anisotropy = Fovea_False;
		createInfo->normalizedCoords = static_cast<FoveaBool>(info.normalizedCoords);
		createInfo->tiling = static_cast<FoveaImageTiling>(info.tiling);
		createInfo->samples = static_cast<FoveaSample>(info.samples);
	}
	
	TextureID RD_API createTextureFromRenderTarget(RenderTargetID renderTargetID, uint32_t attachment, TextureCreateInfo& info){
		FoveaTextureCreateInfo createInfo;
		convertTextureCreateInfo(info, &createInfo);
		return static_cast<TextureID>(FoveaCreateTextureFromRenderTarget(static_cast<FoveaRenderTarget>(renderTargetID), attachment, &createInfo));
	}

	TextureID RD_API createTextureFromPath(const char* path, TextureCreateInfo& info){
		FoveaTextureCreateInfo createInfo;
		convertTextureCreateInfo(info, &createInfo);
		return static_cast<TextureID>(FoveaCreateTextureFromPath(path, &createInfo));
	}

	TextureID* RD_API createTexturesFromPaths(const char* paths[], TextureCreateInfo* infos, uint32_t textureCount){
		FoveaTextureCreateInfo *createInfos = static_cast<FoveaTextureCreateInfo*>(malloc(sizeof(FoveaTextureCreateInfo) * textureCount));
		if (createInfos == nullptr){
			fprintf(stderr, "MALLOC ERROR");
			exit(1);
		}

		for (uint32_t i=0; i<textureCount; i++){
			convertTextureCreateInfo(infos[i], &createInfos[i]);
		}

		return reinterpret_cast<TextureID*>(FoveaCreateTexturesFromPaths(paths, createInfos, textureCount));
	}

	TextureID RD_API createTextureFromData(TextureFormat format, vec2<uint32_t> size, void* data, TextureCreateInfo& info){
		FoveaTextureCreateInfo createInfo;
		convertTextureCreateInfo(info, &createInfo);
		return static_cast<TextureID>(FoveaCreateTextureFromData(static_cast<FoveaImageFormat>(format), *reinterpret_cast<FoveaUIVec2*>(&size), data, &createInfo));
	}

	void RD_API destroyTexture(TextureID textureid){
		FoveaDestroyTexture(static_cast<FoveaTexture>(textureid));
	}

	// ==========================================================
	// ==                       ASSETS                         ==
	// ==========================================================

	void RD_API registerAssetFactoryPtr(AssetFactory *factory){
		Odin::registerFactoryPtr(reinterpret_cast<Odin::Factory*>(factory));
	}

	void* RD_API getAsset(uint64_t typeID, const char* name, AssetData &data, uint16_t*& count, void (**removeAsset)(const char*)){
		return Odin::getAssetPtr(name, typeID, *reinterpret_cast<Odin::Data*>(&data), count, removeAsset);
	}

	void RD_API clearAssets(){
		Odin::clear();
	}

	// ==========================================================
	// ==                         SOUNDID                        ==
	// ==========================================================

	void RD_API loadMusic(const char* filepath){
		try{
			Gramophone::loadMusic(filepath);
		} catch (const char* err){
			RD_TRHOW_EXCEPT("failed to load music", err);
		}
	}

	void RD_API pauseMusic(){
		Gramophone::pauseMusic();
	}

	void RD_API playMusic(){
		Gramophone::playMusic();
	}

	void RD_API stopMusic(){
		Gramophone::stopMusic();
	}

	void RD_API setMusicSourcePosition(vec3<float> position){
		Gramophone::setMusicPosition(position.x, position.y, position.z);
	}

	SoundID RD_API loadSoundEffect(const char* filepath){
		return static_cast<SoundID>(Gramophone::loadSoundEffect(filepath));
	}

	void RD_API destroySoundEffect(SoundID soundid){
		Gramophone::destroySoundEffect(static_cast<Gramophone::Sound>(soundid));
	}

	SoundSourceID RD_API createSoundSource(){
		return static_cast<SoundSourceID>(Gramophone::createSoundSource());
	}

	void RD_API destroySoundSource(SoundSourceID source){
		Gramophone::destroySoundSource(static_cast<Gramophone::Source>(source));
	}

	void RD_API setSoundSourcePitch(SoundSourceID source, float pitch){
		Gramophone::setSourcePitch(static_cast<Gramophone::Source>(source), pitch);
	}

	void RD_API setSoundSourceGain(SoundSourceID source, float gain){
		Gramophone::setSourceGain(static_cast<Gramophone::Source>(source), gain);
	}

	void RD_API setSoundSourcePosition(SoundSourceID source, vec3<float> position){
		Gramophone::setSourcePosition(static_cast<Gramophone::Source>(source), position.x, position.y, position.y);
	}

	void RD_API setSoundSourceVelocity(SoundSourceID source, vec3<float> velocity){
		Gramophone::setSourceVelocity(static_cast<Gramophone::Source>(source), velocity.x, velocity.y, velocity.z);
	}

	void RD_API setSoundSourceLoop(SoundSourceID source, bool loop){
		Gramophone::setSourceLoop(static_cast<Gramophone::Source>(source), loop);
	}

	void RD_API playSoundSource(SoundSourceID source, SoundID soundid){
		Gramophone::playSource(static_cast<Gramophone::Source>(source), static_cast<Gramophone::Sound>(soundid));
	}

	void RD_API setListenerPosition(vec3<float> position){
		Gramophone::setListenerPosition(position.x, position.y, position.z);
	}

	void RD_API setListenerOrientation(vec3<float> at, vec3<float> up){
		Gramophone::setListenerOrientation(at.x, at.y, at.z, up.x, up.y, up.z);
	}

	void RD_API setListenerGain(float gain){
		Gramophone::setListenerGain(gain);
	}

	void RD_API setListenerPitch(float pitch){
		Gramophone::setListenerPitch(pitch);
	}

	void RD_API setSoundSourceEffects(SoundSourceID source, bool reverb, bool lowpass){
		Gramophone::setSoundSourceEffects(static_cast<Gramophone::Source>(source), reverb, lowpass);
	}

	void RD_API setMusicEffects(bool reverb, bool lowpass){
		Gramophone::setMusicEffects(reverb, lowpass);
	}

	void RD_API setReverbType(ReverbType type){
		Gramophone::setReverbType(static_cast<Gramophone::ReverbType>(type));
	}
}