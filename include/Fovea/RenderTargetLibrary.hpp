#pragma once

#include "RenderTarget.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace Fovea{
	class RenderTargetLibrary{
		public:
			using ID = size_t;
			static constexpr ID invalidID = SIZE_MAX;

			~RenderTargetLibrary();

			void initialize();

			ID push(RenderTargetBuilder *builder, const char *name);
			void erase(ID id);

			ID getIDFromName(const char *name);
			RenderTarget* get(ID id);

			void clear();
		
		private:
			std::vector<RenderTarget*> renderTargets;
			std::unordered_map<std::string, ID> nameToIndexMap;
			uint32_t holes = 0;
	};
}