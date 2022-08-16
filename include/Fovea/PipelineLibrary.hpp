#pragma once

#include "Pipeline.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace Fovea{
	class PipelineLibrary{
		public:
			using ID = size_t;
			static constexpr ID invalidID = SIZE_MAX;

			~PipelineLibrary();

			ID push(Pipeline *pipeline);
			void erase(ID id);
			Pipeline* get(ID id);

			void clear();
			
		private:
			std::vector<Pipeline*> pipelines;
			std::vector<size_t> refCounts;
			uint32_t holes = 0;
			uint32_t refHoles;
	};
}