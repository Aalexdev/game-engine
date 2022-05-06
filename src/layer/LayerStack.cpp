#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/layer/LayerStack.hpp"

namespace engine{
    LayerStack::LayerStack(){
		ENGINE_PROFILE_FUNCTION();
	}   

    LayerStack::~LayerStack(){
		ENGINE_PROFILE_FUNCTION();
		clear();
    }

    void LayerStack::pushLayer(const Ref<Layer> &layer){
		ENGINE_PROFILE_FUNCTION();
        layers.emplace(layers.begin() + layersInsertIndex, layer);
        layersInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::pushOverlay(const Ref<Layer> &overlay){
		ENGINE_PROFILE_FUNCTION();
        layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::popLayer(const Ref<Layer> &layer, bool detatch){
		ENGINE_PROFILE_FUNCTION();
        auto it = std::find(layers.begin(), layers.end(), layer);
        if (it != layers.end()){
            if (detatch) layer->OnDetach();
            layers.erase(it);
            layersInsertIndex--;
        }
    }

	void LayerStack::clear(){
		ENGINE_PROFILE_FUNCTION();
		for (const Ref<Layer> &layer : layers)
			layer->OnDetach();
		
		layers.clear();
	}

    void LayerStack::popOverlay(const Ref<Layer> &overlay, bool detatch){
		ENGINE_PROFILE_FUNCTION();
        auto it = std::find(layers.begin(), layers.end(), overlay);
        if (it != layers.end()){
            if (detatch) overlay->OnDetach();
            layers.erase(it);
        }
    }
}