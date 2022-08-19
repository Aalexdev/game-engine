#pragma once

#include <vector>

typedef unsigned long long size_t;
typedef unsigned int uint32_t;

namespace Gramophone{
	template<typename T>
	class Library{
		public:
			using ID = size_t;
			static constexpr ID invalidID = -1;

			~Library(){
				clear();
			}

			ID reserve(){
				if (holes == 0){
					ID id = ts.size();
					ts.push_back(reinterpret_cast<T*>(0x1));
					return id;
				}

				ID id = 0;
				for (; id<ts.size(); id++){
					if (ts[id] == nullptr){
						ts[id] = reinterpret_cast<T*>(0x1);
						break;
					}
				}

				holes--;
				return id;
			}

			void set(ID id, T* t){
				ts[id] = t;
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
						break;
					}
				}

				holes--;
				return id;
			}

			void erase(ID id){
				if (ts[id] != reinterpret_cast<T*>(0x1)) delete ts[id];
				ts[id] = nullptr;
				holes++;
			}

			T* get(ID id){
				return ts[id];
			}

			void clear(){
				for (ID id=0; id<ts.size(); id++){
					if (ts[id] && ts[id] != reinterpret_cast<T*>(0x1)) delete ts[id];
				}
				ts.clear();
				holes = 0;
			}
		
		private:
			std::vector<T*> ts;
			uint32_t holes = 0;
	};
}