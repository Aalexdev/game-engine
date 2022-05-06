#pragma once

#include "../core.hpp"
#include "Layer.hpp"

namespace engine{
    class ENGINE_API LayerStack{
        public:
            using LayerVec = std::vector<Ref<Layer>>;
            
            LayerStack();
            ~LayerStack();

            void pushLayer(const Ref<Layer>& layer);
            void pushOverlay(const Ref<Layer>& overlay);
            void popLayer(const Ref<Layer>& layer, bool detatch = true);
            void popOverlay(const Ref<Layer>& overlay, bool detatch = true);

			void clear();

            LayerVec::iterator begin() {return layers.begin();}
            LayerVec::iterator end() {return layers.end();}

        private:
            LayerVec layers;
            size_t layersInsertIndex = 0;
    };
}