#pragma once

#include "SoundSource.hpp"
#include <vector>
#include <queue>

namespace Gramophone{
	class SoundSourceLibrary{
		public:
			using ID = size_t;
			static constexpr ID invalidID = -1;

			~SoundSourceLibrary(){
				clear();
			}

			ID reserve(){
				if (holes.empty()){
					ID id = ts.size();
					ts.push_back(reinterpret_cast<SoundSource*>(0x1));
					return id;
				}

				ID id = holes.front();
				holes.pop();
				return id;
			}

			void set(ID id, SoundSource* t){
				ts[id] = t;
			}

			ID push(SoundSource *t){
				if (holes.empty()){
					ID id = ts.size();
					ts.push_back(t);
					return id;
				}

				ID id=holes.front();
				holes.pop();
				return id;
			}

			void erase(ID id){
				if (ts[id] != reinterpret_cast<SoundSource*>(0x1)) delete ts[id];
				ts[id] = nullptr;
				holes.push(id);
			}

			SoundSource* get(ID id){
				return ts[id];
			}

			void clear(){
				for (ID id=0; id<ts.size(); id++){
					if (ts[id] && ts[id] != reinterpret_cast<SoundSource*>(0x1)) delete ts[id];
				}
				ts.clear();

				while (!holes.empty()){
					holes.pop();
				}
			}
		
		private:
			std::vector<SoundSource*> ts;
			std::queue<ID> holes = {};
	};
}