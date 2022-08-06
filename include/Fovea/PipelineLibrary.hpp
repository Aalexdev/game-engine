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

			void initialize();

			ID push(PipelineBuilder *builder, const char *name);
			void erase(ID id);

			ID getIDFromName(const std::string &name);
			Pipeline* get(ID id);
			
		private:
			std::vector<Pipeline*> pipelines;
			std::unordered_map<std::string, ID> nameToIndexMap;
	};
}