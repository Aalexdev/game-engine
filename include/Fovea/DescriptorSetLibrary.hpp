#pragma once

#include "DescriptorSet.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace Fovea{
	class DescriptorSetLibrary{
		public:
			using ID = size_t;
			static constexpr ID invalidID = SIZE_MAX;

			~DescriptorSetLibrary();

			void initialize();

			ID push(DescriptorSetBuilder *builder, const char *name);
			void erase(ID id);

			ID getIDFromName(const char *name);
			DescriptorSet* get(ID id);

			void clear();
		
		private:
			std::vector<DescriptorSet*> sets;
			std::unordered_map<std::string, ID> nameToIndexMap;
			uint32_t holes = 0;
	};
}