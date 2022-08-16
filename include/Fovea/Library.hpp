#pragma once

#include <vector>

typedef unsigned long long size_t;

namespace Fovea{
	template<typename T>
	class Library{
		public:
			using ID = size_t;
			static constexpr ID invalidID = -1;

			~Library(){
				clear();
			}

			ID push(T *t){
				if (holes == 0){
					ID id = ts.size();
					ts.push_back(t);
					return id;
				}

				ID id=0;
				for (; id<ts.size(); id++){
					if (ts[id] == nullptr){
						ts[id] = t;
					}
				}

				holes--;
				return id;
			}

			void erase(ID id){
				delete ts[id];
				ts[id] = nullptr;
			}

			T* get(ID id){
				return ts[id];
			}

			void clear(){
				for (ID id=0; id<ts.size(); id++){
					if (ts[id]) delete ts[id];
				}
				ts.clear();
				holes = 0;
			}
		
		private:
			std::vector<T*> ts;
			uint32_t holes = 0;
	};
}